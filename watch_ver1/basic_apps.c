/*
 * basic_apps.c
 *
 *  Created on: 26 Apr 2018
 *      Author: dadan
 */
#include "basic_apps.h"
#include <avr/sleep.h>

volatile time_t g_time;	// current time
time_t g_alarm_time;	// alarm is set to this time
uint8_t g_alarm_enabled = 0;

extern volatile uint8_t g_sleep_allowed;

void app_set_time(volatile time_t *time)
{
	time_t new_time;

	new_time.hours = time->hours;
	new_time.minutes = time->minutes;


	epd_reset();			// todo: wasting with power in delay loops
	epd_init_partial(DISPLAY_TEMPERTURE);

	g_dirty_framebuffers = 2;
	epd_clear_frame_memory(COLOR_WHITE);

	// wait until all buttons are depressed
	while (button_pressed());

	int current_digit = 1;
	while (current_digit <= 4)
	{

    	if (g_dirty_framebuffers > 0)				// clean framebuffer when needed
    	{
    		epd_clear_frame_memory(COLOR_WHITE);
    		--g_dirty_framebuffers;
    	}


#ifdef EPD_SIZE_2_13

	    canvas_display_from_flash(&image_buffer, &time_font[new_time.hours / 10], 8, 176, (current_digit == 1) ? 0 : 1);	// fist digit
	    canvas_display_from_flash(&image_buffer, &time_font[new_time.hours % 10], 8, 128, (current_digit == 2) ? 0 : 1);	// second digit

	    canvas_display_from_flash(&image_buffer, &time_font[10], 8, 120, 1);	// delimiter

	    canvas_display_from_flash(&image_buffer, &time_font[new_time.minutes / 10], 8, 72, (current_digit == 3) ? 0 : 1);	// third digit
	    canvas_display_from_flash(&image_buffer, &time_font[new_time.minutes % 10], 8, 24, (current_digit == 4) ? 0 : 1);		// fourth digit
#endif



	    epd_display_frame();

	    // set up idle timer
		sw_timer[SW_TIMER_IDLE] = IDLE_TIME;
		while (!button_pressed())		// wait until any button pressed
		{
			if (sw_timer[SW_TIMER_IDLE] == 0)	// or until time runs out
				return;
		}

		backlight_enable(DEFAULT_BACKLIGHT_TIME);

	    if (button1_state())				// BUTTON 1 - move to next digit
	    {
	    	// check if hours are not more then 23
	    	if ( (current_digit == 1) && ((new_time.hours / 10) == 2) && ((new_time.hours % 10) > 3) )
			{
	    		new_time.hours = 23;
			}
	    	++current_digit;
	    	g_dirty_framebuffers = 1;
	    }

	    if (button2_state())				// BUTTON 2	- cancel
	    {
	    	if (current_digit > 1)			// go one digit left if it is not the first digit
	    	{
	    		--current_digit;
	    		g_dirty_framebuffers = 1;
	    	}
	    	else							// cancel menu
	    	{
	    		// wait until all buttons are depressed
	    		while (button_pressed());

	    		return;
	    	}
	    }

	    if (button3_state())				// BUTTON 3 - increment current digit
	    {
	    	// increment current digit
	    	if (current_digit == 1)		// increment tens of hours
	    	{
	    		if ((new_time.hours / 10) < 2)
	    			new_time.hours += 10;
	    		else
	    			new_time.hours = new_time.hours % 10;
	    	}

	    	if (current_digit == 2)		// increment hours
	    	{
	    		if ((new_time.hours / 10) < 2)		// 00h - 19h
	    		{
	    			if ((new_time.hours % 10) < 9)
	    			{
	    				new_time.hours += 1;
	    			}
	    			else
	    				new_time.hours = (new_time.hours / 10) * 10;
	    		}
	    		else								// 20h - 24h
	    		{
	    			if ((new_time.hours % 10) < 3)
	    				new_time.hours += 1;
	    			else
	    				new_time.hours = (new_time.hours / 10) * 10;
	    		}
	    	}

	    	if (current_digit == 3)		// increment tens of minutes
	    	{
	    		if ((new_time.minutes / 10) < 5)
	    			new_time.minutes += 10;
	    		else
	    			new_time.minutes  = new_time.minutes % 10;
	    	}

	    	if (current_digit == 4)		// increment minutes
	    	{
	    		if ((new_time.minutes % 10) < 9)
	    			new_time.minutes += 1;
	    		else
	    			new_time.minutes  = (new_time.minutes / 10) * 10;
	    	}

	    }

	    if (button4_state())				// BUTTON 4 - decrement current digit
	    {
	    	if (current_digit == 1)		// decrement tens of hours
	    	{
	    		if ((new_time.hours / 10) > 0)
	    			new_time.hours -= 10;
	    		else
	    			new_time.hours = (new_time.hours % 10) + 20;
	    	}

	    	if (current_digit == 2)		// decrement hours
	    	{
	    		if ((new_time.hours / 10) < 2)		// 00h - 19h
	    		{
	    			if ((new_time.hours % 10) > 0)
	    			{
	    				new_time.hours -= 1;
	    			}
	    			else
	    				new_time.hours = ((new_time.hours / 10) * 10 ) + 9;
	    		}
	    		else								// 20h - 24h
	    		{
	    			if ((new_time.hours % 10) > 0)
	    				new_time.hours -= 1;
	    			else
	    				new_time.hours = ((new_time.hours / 10) * 10) + 3;
	    		}
	    	}

	    	if (current_digit == 3)		// decrement tens of minutes
	    	{
	    		if ((new_time.minutes / 10) > 0)
	    			new_time.minutes -= 10;
	    		else
	    			new_time.minutes  = (new_time.minutes % 10) + 50;
	    	}

	    	if (current_digit == 4)		// decrement minutes
	    	{
	    		if ((new_time.minutes % 10) > 0)
	    			new_time.minutes -= 1;
	    		else
	    			new_time.minutes  = ((new_time.minutes / 10) * 10) + 9;
	    	}

	    }

	}

	time->hours = new_time.hours;
	time->minutes = new_time.minutes;
	time->seconds = 0;

	// wait until all buttons are depressed
	while (button_pressed());

}


void app_status_screen(void)
{
	epd_init_partial(DISPLAY_TEMPERTURE);
	epd_clear_frame_memory(COLOR_WHITE);

	// wait until all buttons are depressed
	while (button_pressed());

	char text[8];

	// display battery voltage
	battery_get_voltage_string(text, 6);
	canvas_display_text(&image_buffer,&font24, text, 0, 10, 1);
	canvas_display_text(&image_buffer,&font24, "Battery:", 0, 112, 1);

	// battery charge %
	battery_get_percentage_string(text, 6);
	canvas_display_text(&image_buffer,&font24, text, 24, 10, 1);

	// show temperature
	// char s_temperature[8];
	temperature_celsius_string(g_temperature_raw, text, 8);
	canvas_display_text(&image_buffer,&font24, text, 24, 128, 1);


	// show uptime from last charge
	uint16_t days;
	char s_uptime[6];

	days = g_uptime.hours_from_last_charge / 24;
	s_uptime[0] = '0' + days / 1000;		// days
	days -= (days/1000) * 1000;
	s_uptime[1] = '0' + days / 100;
	days -= (days/100) * 100;
	s_uptime[2] = '0' + days / 10;
	days -= (days/10) * 10;
	s_uptime[3] = '0' + days % 10;
	s_uptime[4] = 'd';
	s_uptime[5] = 0;

	canvas_display_text(&image_buffer,&font24, "Charge:", 48, 128, 1); // 124 (malo)
	canvas_display_text(&image_buffer,&font24, s_uptime, 48, 10, 1);

	// show total uptime
	days = g_uptime.hours_total / 24;

	s_uptime[0] = '0' + days / 1000;		// days
	days -= (days/1000) * 1000;
	s_uptime[1] = '0' + days / 100;
	days -= (days/100) * 100;
	s_uptime[2] = '0' + days / 10;
	days -= (days/10) * 10;
	s_uptime[3] = '0' + days % 10;
	s_uptime[4] = 'd';
	s_uptime[5] = 0;

	canvas_display_text(&image_buffer,&font24, "Uptime:", 72, 128, 1); // 124 (malo)
	canvas_display_text(&image_buffer,&font24, s_uptime, 72, 10, 1);

	canvas_display_text(&image_buffer,&font24, "FW:", 96, 196, 1);
	canvas_display_text(&image_buffer,&font24, FW_VER, 96, 10, 1);

	epd_display_frame();

	sw_timer[SW_TIMER_IDLE] = IDLE_TIME;
	while (!button_pressed())		// wait until any button pressed
	{
		if (sw_timer[SW_TIMER_IDLE] == 0)	// or until time runs out
			return;
	}

	// wait until all buttons are depressed
	while (button_pressed());
}

void backlight_enable(uint8_t timeout)
{
	#ifdef BACKLIGHT_ENABLED

	PORTD |= (1<<PORTD6);
	sw_timer[SW_TIMER_BACKLIGHT] = timeout;

	#endif
}

void backlight_disable()
{
	PORTD &= ~(1<<PORTD6);
}

void alert_enable(uint8_t timeout)
{
	g_sleep_allowed = 0;

	// Initiate TC1 in CTC (Clear Timer on Compare match) mode
	power_timer1_enable();
	OCR1A = 0x2000;				// count to this number, 0xFFFF is cca 1 second
	TCCR1B |= (1<WGM12) | (1 << CS11) | (1<<CS10);		// CTC mode | clk_io / 64
	TCNT1 = 0;
	TIMSK1 |=  (1 << OCIE1A);


	sw_timer[SW_TIMER_ALERT] = timeout;
}

void alert_disable()
{
	TCCR1B = 0;		// stop TC1
	TCNT1 = 0;
	power_timer1_disable();

	PORTD &= ~(1<<PORTD5);	// turn off buzzer
	g_sleep_allowed = 1;
}

void mcu_sleep(void)
{
	if (g_sleep_allowed)
	{
		set_sleep_mode(SLEEP_MODE_PWR_SAVE);
		sleep_mode();			// Enter sleep mode - POWER SAVE mode
	}
}

