/*
 * main.h
 *
 *  Created on: Feb 8, 2019
 *      Author: hello
 */

#ifndef MAIN_H_
#define MAIN_H_

#define FW_VER	"0.2.2-dev"

// --- PCB version ---
#define HW_VER_1_1		// PCB version 1.1
// #define HW_VER_1_0	// PCB version 1.0

// --- DEBUG AND TESTING ---
#define CLEAR_DISPLAY_ON_START
// #define VIBRATE_ON_START
// #define FAST_TIME		// every minute has only 10 seconds
#define INVERTED_COLORS	0	// All colors will be inverted
// #define DEBUG		// watchface with debug variables and only small time

// --- DEVICE CONFIGURATION ---
// #define DEVICE_HAS_ALARM		// device equipped with alarm buzzer/vibration motor
#define BACKLIGHT_ENABLED


#endif /* MAIN_H_ */
