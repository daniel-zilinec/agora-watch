/*
 * spi.h
 *
 *  Created on: Mar 10, 2018
 *      Author: dadan
 */

#ifndef SPI_H_
#define SPI_H_

#include "avr/io.h"

void spi_init(void);
void spi_send_command(uint8_t command);
void spi_send_data(uint8_t data);

#endif /* SPI_H_ */
