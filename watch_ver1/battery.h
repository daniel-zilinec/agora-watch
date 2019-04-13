/*
 * battery.h
 *
 *  Created on: 10 Nov 2018
 *      Author: dadan
 */

#ifndef BATTERY_H_
#define BATTERY_H_

#include <avr/io.h>

// Values for % of charge calculation
#define BATTERY_VOLTAGE_LOW		3500	// for low battery warning
#define BATTERY_VOLTAGE_DISCHARGED		3350	// todo: going to low power state
#define BATTERY_VOLTAGE_FULL		4100

volatile uint16_t g_battery_voltage;

// initialize ADC
void battery_setup();

// get status of battery charge pin

uint8_t battery_is_charging();

// start ADC conversion for battery voltage
void battery_start_measurement();

// get voltage in millivolts
uint16_t battery_get_voltage();

// get battery state in percents
uint8_t battery_get_percentage();

void battery_get_voltage_string(char str[], uint8_t len);

void battery_get_percentage_string(char str[], uint8_t len);

#endif /* BATTERY_H_ */
