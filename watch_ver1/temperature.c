/*
 * temperature.c
 *
 *  Created on: May 4, 2019
 *      Author: hello
 */

#include "temperature.h"

void temperature_enable()
{
	power_twi_enable();

	TWCR = (1 << TWEN);		// TWI enable and TWI interrupt enable

	// for 1 MHz CPU clock set maximum TWI speed (62,5 kHz)
	// for 8 MHZ CPU is maximum speed 500 kHz
	TWBR = 0;		// bit rate register
	TWSR = 0x00;		// prescaler 4

//	TWBR = 123;		// bit rate register
//	TWSR = 0x01;		// prescaler 4
}

void temperature_disable()
{
	// disable TWI
	TWCR = 0x00;

	power_twi_disable();
}

uint8_t temperature_get_raw(uint16_t *t)
{
	uint8_t msb, lsb;

	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) ; // send start condition

	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_START)
	{
		// send address
		TWDR = TMP75_ADDR | TW_READ;		// send TWI address (read operation)
		TWCR = (1 << TWINT) | (1 << TWEN);	// start transmission
	}
	else
		return TMP_ERROR;



	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_MR_SLA_ACK)		// if Master Read / slave ACK
	{
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);	// send ACK
	}
	else
		return TMP_ERROR;

	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_MR_DATA_ACK)		// 	Master data read with ACK
	{
		// read MSB byte
		msb = TWDR;

		TWCR = (1 << TWINT) | (1 << TWEN);	// send NACK
	}
	else
		return TMP_ERROR;

	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_MR_DATA_NACK)
	{
		// read LSB byte
		lsb = TWDR;

		// send stop
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	}
	else
		return TMP_ERROR;


	*t = (msb << 8) | lsb;
	return TMP_SUCCESS;
}

uint8_t temperature_raw_to_celsius (uint16_t raw)
{
	return (raw >> 8);
}

void temperature_celsius_string(uint16_t raw, char str[], uint8_t len)
{
    uint8_t temperature;

    if (len < 5)		// array is too short
    	return;

    temperature = temperature_raw_to_celsius(raw);

    str[0] = '0' + temperature/ 100;
    temperature -= (temperature / 100) * 100;
    if (str[0] == '0')		// remove leading zero
    	str[0] = ' ';

    str[1] = '0' + temperature / 10;
    temperature -= (temperature / 10) * 10;
    if ((str[1] == '0') && (str[0] == ' '))		// remove leading zero
    	str[1] = ' ';

    str[2] = '0' + temperature % 10;

    str[3] = 'C';

    str[4] = 0;
}
