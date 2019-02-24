/*
 * main.c
 *
 *  Created on: Mar 8, 2018
 *      Author: dadan
 *
 *      Atmega328p
 *
 *      Fuses:
 *      EXT = 0xFF
 *      HIGH = 0xD9
 *      LOW = 0x62	(0xE2 with CKDIV8 disabled)
 *
 *      - Brown-out detection disabled
 *      - Internal 8 MHz RC oscillator & CKDIV8 enabled -> F_CPU = 1 MHz
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>

#include "main.h"
#include "watchface.h"
#include "battery.h"
#include "buttons.h"
#include "menu.h"
#include "time_font.h"
#include "fonts.h"
#include "spi.h"
#include "epd_1_54.h"
#include "canvas.h"
#include "basic_apps.h"
#include "timers.h"




// TODO: move backlight enabling after every keypress into interrupt
// TODO: alarm - snooze & disable vibrating alarm
// TODO: Add FW version from SVN



volatile uint8_t display_refresh_needed;
volatile uint16_t g_battery_voltage;
volatile uint8_t g_battery_charging;

uint8_t g_battery_low_flag, g_battery_discharged_flag;


image_buffer_t image_buffer;

// ISR(TIMER1_COMPA_vect);
ISR(TIMER2_OVF_vect);



int main(void)
{
	// -- Initialize e-paper display --
	spi_init();
	epd_reset();
	epd_init_full(DISPLAY_TEMPERTURE);
	//epd_init_partial(DISPLAY_TEMPERTURE);

	// reset software timers
	timers_reset();

	// initiate port as output for vibration motor and backlight
	DDRD |= (1<<DDD5) | (1<<DDD6);

	// Hold LDO enabled even without VIN voltage (LDE_EN)
	DDRD |= (1<<DDD4);
	PORTD |= (1<<PORTD4);

	// vibrate and blink with backlight on startup - say hello
	backlight_enable(DEFAULT_BACKLIGHT_TIME);

#ifdef VIBRATE_ON_START
	alert_enable(1);
    _delay_ms(500);
    alert_disable();
#endif

    // clear image on display
	// todo: improve this
#ifdef CLEAR_DISPLAY_ON_START

	// clear one framebuffer and do full-refresh to physically clear display; second framebuffer will be cleared later
    epd_clear_frame_memory(COLOR_WHITE);
    epd_display_frame();
    // epd_clear_frame_memory(COLOR_WHITE);
    // epd_display_frame();
#endif

    // initialize Timer/Counter 2 in asynchronous mode with external 32.768 kHz crystal
    ASSR = (1<<AS2);				// enable asynchronous operation - this bit must be set before TCCR2x
    TCCR2A = 0;						// WGM mode 0 - normal operation
    TCCR2B = (1<<CS22) | (1<<CS20);	// prescaler /128 - one overflow per second
    TIMSK2 = (1<<TOIE2);			// enable overflow interrupt

    // enable pin change interrupt for buttons
    PCICR = (1<<PCIE2) | (1<<PCIE1);
    PCMSK2 |= (1<<PCINT19) | (1<<PCINT18);
    PCMSK1 |= (1<<PCINT10) | (1<<PCINT9);

	// initialize time
	g_time.hours = 10;
	g_time.minutes = 48;
	g_time.seconds = 0;

	// initialize date
	g_time.day = 24;
	g_time.month = 2;
	g_time.year = 2019;

	// set default time for alarm and disable it
	g_alarm_time.hours = 05;
	g_alarm_time.minutes = 35;
	g_alarm_time.seconds = 0;
	g_alarm_enabled = 0;

	// reset uptime counters
	g_uptime.hours_from_last_charge = 0;
	g_uptime.hours_total = 0;

	g_dirty_framebuffers = 2;		// content of both framebuffers is undefined after startup
	display_refresh_needed = 1;

	set_sleep_mode(SLEEP_MODE_PWR_SAVE);

	// power_adc_disable();
	power_timer0_disable();
	power_timer1_disable();
	power_twi_disable();
	power_usart0_disable();


	// Enable pull-up on all unused pins to define voltage on pin (decrease power consumption)
//	PORTB |= (1<<PORTB6) | (1<<PORTB7);
//	PORTC |= 0xFF;
//	PORTD |= (1<<PORTD6) | (1<<PORTD5) | (1<<PORTD4) | (1<<PORTD3) | (1<<PORTD2) | (1<<PORTD1) | (1<<PORTD0);

	g_battery_voltage = 0;
	g_battery_low_flag = 0;
	g_battery_discharged_flag = 0;
	g_battery_charging = 0;
	battery_setup();
   	battery_start_measurement();
	button_init();		// initialize buttons

	sei();		// enable global interrupts


	while (1)
	{
	 	battery_start_measurement();
    	// battery_get_voltage();
    	epd_reset();			// todo: wasting with power in delay loops
    	epd_init_partial(DISPLAY_TEMPERTURE);
    	//epd_init_full(DISPLAY_TEMPERTURE);

    	// is battery charging?
    	// todo: partially moved into pin change interrupt, need to be improved
	    if (battery_is_charging())
	    {
			g_battery_charging = 1;
		}
	    else if (g_battery_charging)
	    {
	    	g_dirty_framebuffers = 2;	// clean framebuffers from text residues
	    	g_battery_charging = 0;
	    }

	    // check battery voltage
	    if (battery_get_voltage() < BATTERY_VOLTAGE_LOW)
	    {
			g_battery_low_flag = 1;
		}
	    else if (g_battery_low_flag)
	    {
	    	g_dirty_framebuffers = 2;	// clean framebuffers from text residues
	    	g_battery_low_flag = 0;
	    }


    	if (g_dirty_framebuffers > 0)				// clean framebuffer when needed
    	{
    		epd_clear_frame_memory(COLOR_WHITE);
    		--g_dirty_framebuffers;
    	}


    	watchface_show();			// put time into framebuffer

    	if (g_battery_charging)
    	{
    		char text[6];
    		battery_get_voltage_string(text, 6);							// display battery voltage
    		canvas_display_text(&image_buffer,&font24, text, 96, 8, 0);
    		canvas_display_text(&image_buffer,&font24, "charging ", 96, 90, 0);
    	}

    	else if (g_battery_low_flag)		// show warning if battery is low
	    {
	    	canvas_display_text(&image_buffer,&font24, " low battery ", 96, 13, 0);
	    }

	    epd_display_frame();		// display framebuffer on display
	    display_refresh_needed = 0;

	    epd_deep_sleep();		// put display to sleep mode to conserve energy

	    while (!display_refresh_needed)		// loop until display refresh needed
	    {
	    	//PORTD &= ~(1<<PORTD6);		// disable backlight
	    	// PORTD |= (1<<PORTD6);		// enable backlight for faster battery discharging
	    	sleep_mode();			// Enter sleep mode - IDLE mode
	    	// PORTD |= (1<<PORTD6);		// enable backlight


	    	if (button_pressed())
	    	{
	    		backlight_enable(DEFAULT_BACKLIGHT_TIME);
	    	}

	    	// Button 3 & 4 - Enter menu

	    	if (button3_state() && button4_state())
	    	{

	    		menu_show();

	    		// clear screen - paint it all black
	    		epd_clear_frame_memory(COLOR_BLACK);
	    		epd_display_frame();

	    		display_refresh_needed = 1;
	    		g_dirty_framebuffers = 2;
	    	}

	    	// Button 1 & 2 - refresh screen
	    	if(button1_state() && button2_state())
	    	{
	    		backlight_disable();		// nothing interesting to see when refreshing display

	    		epd_reset();
	    		epd_init_full(DISPLAY_TEMPERTURE);
	    		epd_clear_frame_memory(COLOR_WHITE);
	    	    epd_display_frame();
	    		// epd_clear_frame_memory(COLOR_WHITE);		// todo: second cleaning should be not needed, but when changing apps the memory is not cleared
	    	    // epd_display_frame();
	    		display_refresh_needed = 1;
	    		g_dirty_framebuffers = 2;
	    	}
	    }
	}
}

ISR(TIMER2_OVF_vect)
{
	for (uint8_t i = 0; i < SW_TIMER_COUNT; ++i)
	{
		if (sw_timer[i] > 0)
			{
			--sw_timer[i];
			}
	}

	++g_time.seconds;

#ifdef FAST_TIME
	if (g_time.seconds > 10)
#endif
#ifndef FAST_TIME
	if (g_time.seconds > 59)
#endif
	{
		g_time.seconds = 0;
		++g_time.minutes;
		display_refresh_needed = 1;

		if (g_time.minutes > 59)
		{
			g_time.minutes = 0;
			++g_time.hours;
			time_date_incremet_uptime(&g_uptime);		// increment uptime - it doesn't account manual time change, but it doesn't matter

			if (g_time.hours >= 24)		// next day
			{
				g_time.hours = 0;
				g_time.day += 1;
				if (g_time.day > timedate_get_days_of_month(g_time))	// next month
				{
					g_time.day = 1;
					++g_time.month;
					if(g_time.month > 12)		// next year
					{
						g_time.month = 1;
						++g_time.year;
					}
				}

			}
		}
	}

	// Alarm
	if (g_alarm_enabled)
	{
		if ((g_alarm_time.hours == g_time.hours) && (g_alarm_time.minutes == g_time.minutes) && (g_time.seconds == 0))
		{
			alert_enable(DEFAULT_ALERT_TIME);
			backlight_enable(DEFAULT_ALERT_TIME);
		}
	}

	if (sw_timer[SW_TIMER_ALERT] == 0)
	{
		alert_disable();
	}
	else
	{
		if (g_time.seconds % 2)		// make interrupted vibrations every second
		{
			alert_disable();
		}
		else
		{
			alert_enable(sw_timer[SW_TIMER_ALERT]);
		}
	}

	if (sw_timer[SW_TIMER_BACKLIGHT] == 0)
	{
		backlight_disable();
	}

}

ISR (PCINT1_vect)
{
	if (g_battery_charging != battery_is_charging())
	{
		display_refresh_needed = 1;
	}
}

ISR (PCINT2_vect)
{
}

ISR (ADC_vect)
{
	g_battery_voltage = ADC;
}


