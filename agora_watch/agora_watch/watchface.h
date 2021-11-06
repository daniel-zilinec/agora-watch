/*
 * watchface.h
 *
 *  Created on: 10 Nov 2018
 *      Author: dadan
 */

#ifndef WATCHFACE_H_
#define WATCHFACE_H_

#include "epd_1_54.h"
#include "basic_apps.h"

#define WATCHFACE_COUNT	2				// number of available watchfaces (1 or more)

extern volatile time_t g_time;
uint8_t g_current_watchface;

void watchface_show(const uint8_t num);

#endif /* WATCHFACE_H_ */
