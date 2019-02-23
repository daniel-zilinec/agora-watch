/*
 * buttons.c
 *
 *  Created on: Mar 17, 2018
 *      Author: dadan
 */


#include <avr/io.h>

void button_init(void)
{
	DDRD &= ~ ((1<<DDD2) | (1<<DDD3) );		// set PORTD.2 .3 as inputs
	DDRC &= ~ ((1<<DDC1) | (1<<DDC2) );		// set PORTC.1 .2 as inputs
	PORTD |= (1<<PORTD2) | (1<<PORTD3);		// enable pull-ups on D.2 .3
	PORTC |= (1<<PORTC1) | (1<<PORTC2);		// enable pull-ups on D.2 .3
}

uint8_t button1_state(void)
{
	if (PIND & (1<<PIND3))
		return 0;
	else
		return 1;
}


uint8_t button2_state(void)
{
	if (PINC & (1<<PINC1))
		return 0;
	else
		return 1;
}

uint8_t button3_state(void)
{
	if (PINC & (1<<PINC2))
		return 0;
	else
		return 1;
}

uint8_t button4_state(void)
{
	if (PIND & (1<<PIND2))
		return 0;
	else
		return 1;
}

// return 1 if is any button pressed
uint8_t button_pressed(void)
{
	return (button1_state() || button2_state() || button3_state() || button4_state());
}
