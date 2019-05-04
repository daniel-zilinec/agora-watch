/*
 * temperature.h
 *
 * Temperature measurement using I2C LM75B sensor
 *
 *  Created on: May 4, 2019
 *      Author: hello
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include <avr/power.h>
#include <util/twi.h>


#define TMP75_ADDR			(0x48 << 1)
#define TMP75_TEMP_POINTER	0x00

#define	TMP_ERROR			0
#define TMP_SUCCESS			1

uint16_t g_temperature_raw;

// Enable power for I2C and initialize temperature measurement
void temperature_enable();

// Disable power for I2C module
void temperature_disable();

// void temperature_start_measurement();

uint8_t temperature_get_raw(uint16_t *t);

uint8_t temperature_raw_to_celsius (uint16_t raw);

void temperature_celsius_string(uint16_t raw, char str[], uint8_t len);


#endif /* TEMPERATURE_H_ */
