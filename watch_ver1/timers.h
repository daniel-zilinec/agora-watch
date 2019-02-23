/*
 * timers.h
 *
 *  Created on: 10 Oct 2018
 *      Author: dadan
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include <avr/io.h>

#define DEFAULT_BACKLIGHT_TIME	4		// enable backlight for 4 seconds
#define DEFAULT_ALERT_TIME		14		// enable alert for 14 seconds
#define IDLE_TIME		8		// after this time stop application and jump back to main screen with time


// Software timers
#define SW_TIMER_COUNT		3
#define SW_TIMER_BACKLIGHT 	0
#define SW_TIMER_ALERT		1
#define SW_TIMER_IDLE		2

extern volatile uint8_t sw_timer[SW_TIMER_COUNT];

void timers_reset();

#endif /* TIMERS_H_ */
