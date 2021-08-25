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

void temperature_sleep()
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) ; // send start condition

	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_START)
	{
		// send address
		TWDR = TMP75_ADDR | TW_WRITE;		// send TWI address (write operation)
		TWCR = (1 << TWINT) | (1 << TWEN);	// start transmission
	}
	else
		return;

	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_MT_SLA_ACK)		// if Master Write / slave ACK
	{
		TWDR = 0x01;	// Set pointer to 1 - config register
		TWCR = (1 << TWINT) | (1 << TWEN);	//
	}
	else
		return;

	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_MT_DATA_ACK)		// if Master Write / slave ACK
	{
		TWDR = 0x01;	// Write 0x01 to configuration register - put device to shutdown mode
		TWCR = (1 << TWINT) | (1 << TWEN);	// start transmission
	}
	else
		return;

	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_MT_DATA_ACK)		// if Master Write / slave ACK
	{
		TWCR = (1<< TWSTO) | (1 << TWINT) | (1 << TWEN);	// send STOP bit
	}
	else
		return;
}


void temperature_wakeup()
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) ; // send start condition

	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_START)
	{
		// send address
		TWDR = TMP75_ADDR | TW_WRITE;		// send TWI address (write operation)
		TWCR = (1 << TWINT) | (1 << TWEN);	// start transmission
	}
	else
		return;

	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_MT_SLA_ACK)		// if Master Write / slave ACK
	{
		TWDR = 0x01;	// Set pointer to 1 - config register
		TWCR = (1 << TWINT) | (1 << TWEN);	//
	}
	else
		return;

	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_MT_DATA_ACK)		// if Master Write / slave ACK
	{
		TWDR = 0x00;	// Write 0x00 to configuration register - wake up from shutdown mode
		TWCR = (1 << TWINT) | (1 << TWEN);	// start transmission
	}
	else
		return;

	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_MT_DATA_ACK)		// if Master Write / slave ACK
	{
		TWCR = (1<< TWSTO) | (1 << TWINT) | (1 << TWEN);	// send STOP bit
	}
	else
		return;
}


void temperature_set_pointer(uint8_t ptr)
{

}

uint8_t temperature_get_raw(uint16_t *t)
{
	uint8_t msb, lsb;

	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) ; // send start condition


	// ---- DEVICE ADDRESS ----
	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_START)
	{
		// send address
		TWDR = TMP75_ADDR | TW_WRITE;		// send TWI address (read operation)
		TWCR = (1 << TWINT) | (1 << TWEN);	// start transmission
	}
	else
		return TMP_ERROR;


	// ---- WRITE POINTER REGISTER ----
	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_MT_SLA_ACK)		// if Master Write / slave ACK
	{
		TWDR = 0x00;	// Set pointer to 0 - temperature register
		TWCR = (1 << TWINT) | (1 << TWEN);	// send data
	}
	else
		return TMP_ERROR;

	// ---- SEND REPEATED START ----
	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_MT_DATA_ACK)		// if Master Transmit data ACK
	{
		TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN);	// send repeated start bit
	}
	else
		return TMP_ERROR;


	// ---- DEVICE ADDRESS ----
	while (!(TWCR & (1 << TWINT)));		// wait for TWI flag

	if ((TWSR & 0xF8) == TW_REP_START)		// repeated start transmitted
	{
		// send address
		TWDR = TMP75_ADDR | TW_READ;		// send TWI address (read operation)
		TWCR = (1 << TWINT) | (1 << TWEN);	// start transmission
	}
	else
		return TMP_ERROR;

	// ---- READ DATA ----

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

int16_t temperature_raw_to_celsius (uint16_t raw)
{
	return (raw >> 8);
}

void temperature_celsius_string(uint16_t raw, char str[], uint8_t len)
{
    int16_t temperature;
    uint8_t negative;

    // for test
    // raw = 0x0c8 << 5;		// 25
    // raw = 0x3F8 << 5;		// 127
    // raw = 0x7ff << 5;		// -0.125
    // raw = 0x649 << 5;		// -54.875

    if (len < 8)		// array is too short
    	return;

    // novy vypocet x = (raw*10)/256
    if ((raw & 0x8000) == 0)
    {
    	temperature = ((uint32_t) raw * 10) / 256;		// in tenths of celsius degree 25°C = 250
    }
    else
    {
    	temperature = (~( raw - 0x01)) * -1;
    	temperature = (((int32_t) temperature) * 10 ) / 256;	// in tenths of celsius degree 25°C = 250
    }

    if (temperature < 0)				// if the temperature is negative change make absolute value and mark it as negative
    {
    	negative = 1;
    	temperature = temperature * -1;
    }
    else
    {
    	negative = 0;
    }

    if (negative)
    {
    	str[0] = '-';
    }
    else
    {
    	str[0] = ' ';
    }

    str[1] = '0' + (uint8_t) (temperature/ 1000);
    temperature -= (temperature / 1000) * 1000;
    if (str[1] == '0')		// remove leading zero
    {
    	str[1] = ' ';
    }

    str[2] = '0' + (uint8_t) (temperature / 100);
    temperature -= (temperature / 100) * 100;
    if ((str[2] == '0') && (str[1] == ' '))		// remove leading zero
    	str[2] = ' ';

    str[3] = '0' + (uint8_t) (temperature / 10);
    temperature -= (temperature / 10) * 10;

    str[4] = '.';

    str[5] = '0' + (uint8_t) (temperature % 10);

    str[6] = 'C';

    str[7] = 0;
}
