/*
 * canvas.h
 *
 *  Created on: Mar 10, 2018
 *      Author: dadan
 */

#ifndef CANVAS_H_
#define CANVAS_H_

#include <avr/io.h>
#include "time_font.h"
#include "fonts.h"

#define IMAGE_BUFFER_SIZE	1024			// todo: remove this from here!
#define MAX_TEXT_LENGTH 20

typedef struct
{
	uint8_t buffer[IMAGE_BUFFER_SIZE];
	uint16_t width;
	uint16_t height;
} image_buffer_t;

extern image_buffer_t image_buffer;
extern uint8_t g_dirty_framebuffers;

void canvas_setpixel(image_buffer_t *image, uint16_t x, uint16_t y, uint8_t color);
void clear_buffer(image_buffer_t *image, uint8_t color);

void canvas_read_from_flash(image_buffer_t *image, time_font_t *font, const uint8_t inverted);
void canvas_display_from_flash(image_buffer_t *image, time_font_t *font, uint16_t x, uint16_t y, const uint8_t inverted);

void canvas_read_char_from_flash(image_buffer_t *image, font_t *font, uint8_t ascii_char, const uint8_t inverted);
void canvas_display_char_from_flash(image_buffer_t *image, font_t *font, uint8_t ascii_char, uint16_t x, uint16_t y, const uint8_t inverted);	// todo: will by probably not needed in the future

void canvas_read_string_from_flash(image_buffer_t *image, font_t *font, const char * text, const uint8_t inverted);
void canvas_display_text(image_buffer_t *image, font_t *font, const char * text, uint16_t x, uint16_t y, const uint8_t inverted);

#endif /* CANVAS_H_ */
