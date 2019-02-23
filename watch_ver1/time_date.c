/*
 * time_date.c
 *
 *  Created on: Feb 8, 2019
 *      Author: hello
 */
#include "time_date.h"


uint8_t timedate_get_days_of_month(time_t time)
{
	uint8_t month = time.month;

	if (month == 1)		// January
		return 31;

	if (month == 2)		// February
	{
		// is it a leap year?
		if ((time.year % 4) == 0)		// dividable by 4?
		{
			if ((time.year % 400) == 0)	// dividable by 400?
			{
				return 29;				// it's a leap year
			}

			if ((time.year % 100) == 0)	// dividable by 100?
			{
				return 28;			// not a leap year
			}

			else
				return 29;			// it's a leap year
		}
		else
			return 28;				// not a leap year
	}

	if (month == 3)					// March
		return 31;

	if (month == 4)					// April
		return 30;

	if (month == 5)					// May
		return 31;

	if (month == 6)					// June
		return 30;

	if (month == 7)					// July
		return 31;

	if (month == 8)					// August
		return 31;

	if (month == 9)					// September
		return 30;

	if (month == 10)				// October
		return 31;

	if (month == 11)				// November
		return 30;

	if (month == 12)				// December
		return 31;

	return 31;						// shouldn't get here
}
