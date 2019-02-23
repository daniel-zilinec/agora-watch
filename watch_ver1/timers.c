/*
 * timers.c
 *
 *  Created on: 22 Oct 2018
 *      Author: dadan
 */
#include "timers.h"

volatile uint8_t sw_timer[SW_TIMER_COUNT];

void timers_reset()
{
	for (uint8_t i = 0; i < SW_TIMER_COUNT; ++i)
	{
		sw_timer[i] = 0;
	}
}
