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

extern volatile time_t g_time;

void watchface_show();

#endif /* WATCHFACE_H_ */
