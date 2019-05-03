/*
 * battery.c
 *
 *  Created on: 10 Nov 2018
 *      Author: dadan
 */

#include "battery.h"

void battery_enable_adc()
{
	// enable power for ADC
	power_adc_enable();

	// enable ADC
	ADCSRA = (1<<ADEN);

	// ADC reference - AVCC with external capacitor
	ADMUX |= (1<<REFS0);

	// ADC6
	ADMUX |= (1<<MUX1) | (1<<MUX2);

	// enable ADC interrupt
	ADCSRA |= (1<<ADIE);

	// ADC prescaler 8 -> 1MHz/8 = 125 kHz
	ADCSRA |= (1<<ADPS0) | (1<<ADPS1);

	// set battery charge status pin as input
	DDRC &= ~(1<<0);

	// disable pull-up for battery charge status pin
	PORTC &= ~(1<<0);

	// enable pin-change interrupt for charge status pin
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT8);
}

void battery_disable_adc()
{
	// disable ADC before shutting it down
	ADCSRA &= ~(1<<ADEN);

	power_adc_disable();
}

uint8_t battery_is_charging()
{
	if (PINC & (1<<0))
		return 1;
	else
		return 0;
}

void battery_start_measurement()
{
	// start ADC conversion
	ADCSRA |= (1<< ADSC);
}

uint16_t battery_get_voltage()
{
	uint16_t voltage = (g_battery_voltage * (uint32_t) 6600 ) / 1024;
	return voltage;
	/*
	if (ADCSRA & (1<<ADIF))
	{
		return ADC;
	}
	else return ADC;
	*/
}

uint8_t battery_get_percentage()
{
	// charge state in percent is calculated only from battery voltage

	uint16_t battery_voltage = battery_get_voltage();
	if (battery_voltage >= BATTERY_VOLTAGE_FULL)
		return 100;

	if (battery_voltage <= BATTERY_VOLTAGE_LOW)
		return 0;

	return ((battery_voltage - BATTERY_VOLTAGE_LOW) * 100) / (BATTERY_VOLTAGE_FULL - BATTERY_VOLTAGE_LOW);
}

void battery_get_voltage_string(char str[], uint8_t len)
{
    uint16_t battery_voltage;

    if (len < 6)		// array is too short
    	return;

    battery_voltage = battery_get_voltage();

    str[0] = '0' + battery_voltage / 1000;
    battery_voltage -= (battery_voltage / 1000) * 1000;

    str[1] = '.';

    str[2] = '0' + battery_voltage / 100;
    battery_voltage -= (battery_voltage / 100) * 100;

    str[3] = '0' + battery_voltage / 10;
    battery_voltage -= (battery_voltage / 10) * 10;

    str[4] = 'V';
    str[5] = 0;
}

void battery_get_percentage_string(char str[], uint8_t len)
{
    uint8_t battery_percentage;

    if (len < 4)		// array is too short
    	return;


    battery_percentage = battery_get_percentage();

    str[0] = '0' + battery_percentage/ 100;
    battery_percentage -= (battery_percentage / 100) * 100;
    if (str[0] == '0')		// remove leading zero
    	str[0] = ' ';

    str[1] = '0' + battery_percentage / 10;
    battery_percentage -= (battery_percentage / 10) * 10;
    if ((str[1] == '0') && (str[0] == ' '))		// remove leading zero
    	str[1] = ' ';

    str[2] = '0' + battery_percentage % 10;

    str[3] = '%';

    str[4] = 0;
}
