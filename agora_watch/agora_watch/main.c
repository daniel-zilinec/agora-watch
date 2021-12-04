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
 *      LOW = 0xE2
 *
 *      - Brown-out detection disabled
 *      - Internal 8 MHz RC oscillator & CKDIV8 disabled -> F_CPU = 8 MHz
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "watchface.h"
#include "battery.h"
#include "buttons.h"
#include "menu.h"
#include "time_font.h"
#include "fonts.h"
#include "spi.h"
#include "epd_1_54.h"
#include "EPD_2in13_B72.h"
#include "canvas.h"
#include "basic_apps.h"
#include "timers.h"
#include "temperature.h"

#include <avr/pgmspace.h>
#include "time_font.h"

volatile uint8_t display_refresh_needed;
volatile uint16_t g_battery_voltage;
volatile uint8_t g_battery_charging;
volatile uint8_t g_temperature_wkup;		// flag if temperature sensor wakeup is needed
volatile uint8_t g_sleep_allowed;			// disable sleep mode for MCU when non-zero; alarm buzzer is controlled by TC1 which is not running in sleep mode

uint8_t g_battery_low_flag, g_battery_discharged_flag;

image_buffer_t image_buffer;

ISR(TIMER2_OVF_vect);
ISR(TIMER1_COMPA_vect);

int main(void)
{
	// reset software timers
	timers_reset();

	// initiate port as output for vibration motor and backlight
	DDRD |= (1<<DDD5) | (1<<DDD6);

	// Hold LDO enabled even without VIN voltage (LDE_EN). Only for PCB v1.0, this pin is connected nowhere on PCB v1.1
	DDRD |= (1<<DDD4);
	PORTD |= (1<<PORTD4);
	
	DDRC |= (1<<DDC3);

	// vibrate and blink with backlight on startup - say hello
	backlight_enable(DEFAULT_BACKLIGHT_TIME);

#ifdef VIBRATE_ON_START
	alert_enable(1);
    _delay_ms(500);
    alert_disable();
#endif


    // initialize Timer/Counter 2 in asynchronous mode with external 32.768 kHz crystal
    ASSR = (1<<AS2);				// enable asynchronous operation - this bit must be set before TCCR2x
    TCCR2A = 0;						// WGM mode 0 - normal operation
    TCCR2B = (1<<CS20);				// no prescaler - 128 Hz
    TIMSK2 = (1<<TOIE2);			// enable overflow interrupt
	
	sei();

	while (1);
}
	
ISR(TIMER2_OVF_vect)
{
	// PORTD ^= (1<<PORTD6);		// toggle LED pin
	PORTC ^= (1<<PORTC3);			// toggle pin at frequency 128 Hz -> output is 64 Hz
}