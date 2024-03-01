/*
 * time_font.h
 *
 *  Created on: Mar 9, 2018
 *      Author: dadan
 */

#ifndef TIME_FONT_H_
#define TIME_FONT_H_

#include <stdint.h>

typedef struct
{
  const uint8_t *table;
  uint16_t width;
  uint16_t height;
} time_font_t;

//extern const uint8_t time_font[];
extern time_font_t time_font[];


#endif /* TIME_FONT_H_ */
