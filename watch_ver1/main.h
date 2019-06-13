/*
 * main.h
 *
 *  Created on: Feb 8, 2019
 *      Author: hello
 */

#ifndef MAIN_H_
#define MAIN_H_

#define FW_VER	"0.1.100-dev"

#define HW_VER_1_1		// PCB version 1.1
// #define HW_VER_1_0	// PCB version 1.0

// debug and testing
#define CLEAR_DISPLAY_ON_START
// #define VIBRATE_ON_START
// #define FAST_TIME		// every minute has only 10 seconds
#define DEBUG		// watchface with debug variables and only small time

// the device is clock or watch? Only one can be uncommented
#define DEVICE_TYPE_WATCH		// is this device watch
// #define DEVICE_TYPE_CLOCK		// or clock

// when device is CLOCK type
#ifdef DEVICE_TYPE_CLOCK

#endif

// when device is WATCH type
#ifdef DEVICE_TYPE_WATCH

#define BACKLIGHT_ENABLED

#endif

#endif /* MAIN_H_ */
