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

uint8_t timedate_get_days_of_month(time_t time);

#endif /* TIME_DATE_H_ */
