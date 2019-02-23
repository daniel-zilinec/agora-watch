/*
 * menu.h
 *
 *  Created on: 22 Oct 2018
 *      Author: dadan
 */

#ifndef MENU_H_
#define MENU_H_

#include <avr/io.h>
#include "timers.h"
#include "basic_apps.h"
#include "canvas.h"
#include "epd_1_54.h"
#include "buttons.h"

// #define MENU_ITEM_COUNT			5		// with bluetooth
#define MENU_ITEM_COUNT			4		// without bluetooth

#define	MENU_ITEM_ALARM_TIME	0
#define	MENU_ITEM_ALARM_ENABLED	1
#define	MENU_ITEM_SET_TIME		2
#define	MENU_ITEM_SHOW_INFO		3
#define MENU_ITEM_BLUETOOTH		4

void menu_show();

#endif /* MENU_H_ */
