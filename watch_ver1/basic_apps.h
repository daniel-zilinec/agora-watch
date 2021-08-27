/*
 * basic_apps.h
 *
 *  Created on: 26 Apr 2018
 *      Author: dadan
 */

#ifndef BASIC_APPS_H_
#define BASIC_APPS_H_

#include <avr/io.h>
#include "timers.h"
#include "canvas.h"
#include "epd_1_54.h"
#include "buttons.h"
#include "battery.h"
#include "time_date.h"
#include "temperature.h"
#include "main.h"



extern volatile time_t g_time;
extern time_t g_alarm_time;
extern uint8_t g_alarm_enabled;

void app_set_time(volatile time_t *time);
void app_status_screen(void);

void backlight_enable(uint8_t timeout);
void backlight_disable();

void alert_enable(uint8_t timeout);
void alert_disable();

void mcu_sleep();



#endif /* BASIC_APPS_H_ */
