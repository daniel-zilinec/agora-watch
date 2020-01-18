/*
 * canvas.c
 *
 *  Created on: Mar 10, 2018
 *      Author: dadan
 */

#include "canvas.h"
#include <avr/pgmspace.h>
#include "epd_1_54.h"
#include "EPD_2in13_B72.h"

uint8_t g_dirty_framebuffers;


void canvas_setpixel(image_buffer_t *image, uint16_t x, uint16_t y, uint8_t color)
{
	if ((x > image->width) | (y > image->height))		// check if pixel fits into buffer
	{
		return;
	}

    if (color) 			// set - white; not set - black
    {
        image->buffer[(x + y * image->width) / 8] |= 0x80 >> (x % 8);
    }
    else
    {
        image->buffer[(x + y * image->width) / 8] &= ~(0x80 >> (x % 8));
    }

}

void clear_buffer(image_buffer_t *image, uint8_t color)
{
/*    for (int x = 0; x < EPD_WIDTH; x++) {
        for (int y = 0; y < EPD_HEIGHT; y++) {
            set_pixel(image, x, y, color);
        }
    }*/
	for (int i=0; i < IMAGE_BUFFER_SIZE; ++i)
	{
		if (color)
		{
			image->buffer[i] = 0xFF;
		}
		else
		{
			image->buffer[i] = 0x00;
		}
	}
}

void canvas_read_from_flash(image_buffer_t *image, time_font_t *font, const uint8_t inverted)	// todo: change to "const font_t"
{
	image->width = font->width;
	image->height = font->height;

	uint16_t canvas_size = font->width * font->height / 8;	// calculate size of picture in bytes

    for (int i=0; i < canvas_size; ++i)
    {
    	if (inverted)
    	{
    		image->buffer[i] = ~(pgm_read_byte(&font->table[i]));		// invert colors
    	}
    	else
    	{
    		image->buffer[i] = pgm_read_byte(&font->table[i]);		// copy data from flash
    	}
    }
}

void canvas_display_from_flash(image_buffer_t *image, time_font_t *font, uint16_t x, uint16_t y, const uint8_t inverted)
{
	canvas_read_from_flash(image, font, inverted);

	// old display
	// epd_set_frame(image, x, y, image->width, image->height);

	// B72 display
	EPD_DisplayPartWindows(image->buffer, x, y, x + image->width, y + image->height);
}

void canvas_read_char_from_flash(image_buffer_t *image, font_t *font, uint8_t ascii_char, const uint8_t inverted)
{
	// image->width = font->width;
	// image->height = font->height;
	image->width = font->height; 	// 24
	image->height = font->width;	// 17

	 //uint16_t canvas_size = image->height * image->width / 8;	// calculate size of picture in bytes
	 uint16_t canvas_size = font->width * (font->height / 8 + (font->height % 8 ? 1 : 0)); // 3*17

	for (int i=0; i < canvas_size; ++i)
    {
    	uint8_t data = pgm_read_byte(&font->table[i+ascii_char*canvas_size]);

    	if (inverted)
    	{
    		image->buffer[i] = ~(data);		// invert colors
    	}
    	else
    	{
    		image->buffer[i] = data;		// copy data from flash
    	}
    }

}

void canvas_display_char_from_flash(image_buffer_t *image, font_t *font, uint8_t ascii_char, uint16_t x, uint16_t y, const uint8_t inverted)
{
	canvas_read_char_from_flash(image, font, ascii_char, inverted);

	// old display
	// epd_set_frame(image, x, y, image->width, image->height);

	// B72 display
	EPD_DisplayPartWindows(image->buffer, x, y, x + image->width, y + image->height);
}

void canvas_read_string_from_flash(image_buffer_t *image, font_t *font, const char * text, const uint8_t inverted)
{
	const char *pt = text;

	// get size of text
	uint8_t text_length = 0;
	while (*(pt+text_length) != 0)
	{
		++text_length;
		if (text_length > MAX_TEXT_LENGTH)
		{
			return;
		}
	}

	image->width = font->height;
	image->height = text_length * font->width;

	uint16_t character_size = font->width * (font->height / 8 + (font->height % 8 ? 1 : 0)); // 3*17
	uint8_t character_position;

	for (character_position = 0; character_position < text_length; ++character_position)
	{
		for (int i=0; i < character_size; ++i)
	    {
			char current_character = (*(pt+character_position)) - ' ';
	    	uint8_t data = pgm_read_byte(&font->table[i + current_character * character_size]);

	    	if (inverted)
	    	{
	    		image->buffer[i+(text_length-character_position-1)*character_size] = ~(data);		// invert colors
	    	}
	    	else
	    	{
	    		image->buffer[i+(text_length-character_position-1)*character_size] = data;		// copy data from flash
	    	}
	    }
	}

}

void canvas_display_text(image_buffer_t *image, font_t *font, const char * text, uint16_t x, uint16_t y, const uint8_t inverted)
{
	canvas_read_string_from_flash(image, font, text, inverted);

	// old display
	// epd_set_frame(image, x, y, image->width, image->height);

	// B72 display
	EPD_DisplayPartWindows(image->buffer, x, y, x + image->width, y + image->height);
}
