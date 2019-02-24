/*
 * time_date.h
 *
 *  Created on: Feb 8, 2019
 *      Author: hello
 */

#ifndef TIME_DATE_H_
#define TIME_DATE_H_

#include <avr/io.h>

typedef struct
{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t day;
	uint8_t month;
	uint16_t year;
} time_t;

typedef struct
{
	uint16_t hours_from_last_charge;
	uint16_t hours_total;				// uint16_t = 65535 hours -> 7.58 years - enough for debug
} uptime_t;

volatile uptime_t g_uptime;

uint8_t timedate_get_days_of_month(time_t time);
void time_date_incremet_uptime(volatile uptime_t *uptime);

#endif /* TIME_DATE_H_ */
