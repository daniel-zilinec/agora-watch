/*
 * spi.c
 *
 *  Created on: Mar 10, 2018
 *      Author: dadan
 */

#include "spi.h"

void spi_init(void)
{
	// Initialize SPI interface
	// nCS - Chip select - PORT B.2
	// nRST - Reset - PORT B.0
	// DC - Data / nCommand - PORT B.1
	// BUSY - PORT D.7 (input)
	// MOSI - PORT B.3
	// MISO - PORT B.4 (overridden as input when SPI enabled)
	// SCK - PORT B.5
	// don't use SS pin (PORT B.2) as input when SPI enabled

	// set pin directions
	DDRB |= (1<<DDB5) |(1<<DDB4) |(1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
	DDRD &= ~(1<<DDD7);
	// CS - disable
	PORTB |= (1<<PORTB2);

	// SPI mode 0, MSB bit first, clock 500 kHz (1 MHz F_CPU / 2 SPI_prescaler)
	 SPCR = (1<<SPE) | (1<<MSTR);
	 SPSR = (1<< SPI2X);

	// SPI mode 0, MSB bit first, clock 500 kHz (8 MHz F_CPU / 16 SPI_prescaler)
	// SPCR = (1<<SPE) | (1<<MSTR);
	//SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) |(1<<SPR1);

}

void spi_send_command(uint8_t command)
{
	// set DC pin to command mode
	PORTB &= ~ (1<<PORTB1);

	// enable Chip select - CS low
	PORTB &= ~(1<<PORTB2);

	// send command to SPI
	SPDR = command;

	// wait until data are sent
	while ( !(SPSR & (1<<SPIF) ) );

	// disable Chip select - CS high
	PORTB |= (1<<PORTB2);

}

void spi_send_data(uint8_t data)
{
	// set DC pin to data mode
	PORTB |=  (1<<PORTB1);

	// enable Chip select - CS low
	PORTB &= ~(1<<PORTB2);

	// send command to SPI
	SPDR = data;

	// wait until data are sent
	while ( !(SPSR & (1<<SPIF) ) );

	// disable Chip select - CS high
	PORTB |= (1<<PORTB2);

}

