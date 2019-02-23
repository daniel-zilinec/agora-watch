/*
 * epd_1_54.c
 *
 *  Created on: Mar 10, 2018
 *      Author: dadan
 */

#include "epd_1_54.h"
#include "spi.h"
#include <util/delay.h>	// todo: remove this with _delay_ms()

#ifdef EPD_SIZE_1_54

const unsigned char lut_full_update[] =
{
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

const unsigned char lut_partial_update[] =
{
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

#ifdef EPD_SIZE_2_13

const unsigned char lut_full_update[] =
{
    0x22, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x11,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char lut_partial_update[] =
{
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#endif


void epd_init(const unsigned char* lut, int8_t temperature)
{
	// initialize display
	spi_send_command(DRIVER_OUTPUT_CONTROL);
    spi_send_data((EPD_HEIGHT - 1) & 0xFF);
    spi_send_data(((EPD_HEIGHT - 1) >> 8) & 0xFF);
    spi_send_data(0x00);                     // GD = 0; SM = 0; TB = 0;
    spi_send_command(BOOSTER_SOFT_START_CONTROL);
    spi_send_data(0xD7);
    spi_send_data(0xD6);
    spi_send_data(0x9D);

    spi_send_command(WRITE_VCOM_REGISTER);
    spi_send_data(0x9C);                     // VCOM original_comment:7C original 0xA8

    spi_send_command(SET_DUMMY_LINE_PERIOD);
    spi_send_data(0x1A);                     // 4 dummy lines per gate
    spi_send_command(SET_GATE_TIME);
    spi_send_data(0x08);                     // 2us per line
    spi_send_command(DATA_ENTRY_MODE_SETTING);
    spi_send_data(0x03);                     // X increment; Y increment 0x03

    epd_set_temperature(temperature);				// set temperature of display (if not external temp. sensor present)

    // send look-up table for full update mode
    spi_send_command(WRITE_LUT_REGISTER);

    for (int i = 0; i < 30; i++)		// LUT is 30 bytes long
    {
        // spi_send_data(lut_full_update[i]);	// whole display is updated
    	//spi_send_data(lut_partial_update[i]);		// only changed pixels are updated
    	spi_send_data(lut[i]);				// send lookup table
    }
}

void epd_init_full(int8_t temperature)
{
	epd_init(lut_full_update, temperature);
}

void epd_init_partial(int8_t temperature)
{
	epd_init(lut_partial_update, temperature);
}

void epd_reset(void)
{
	// Reset display
	PORTB &= ~(1<<PORTB0);
	_delay_ms(10);		// was 200
	PORTB |= (1<<PORTB0);
	_delay_ms(10);		// was 200
}

void epd_set_temperature(int8_t celsius)
{
	int16_t temperature_16 = celsius * 16;

	uint8_t msb = (temperature_16 >> 8) & 0xFF;
	uint8_t lsb = temperature_16 & 0xF0;

	// uint8_t msb = 0xe7;		// 0x19
	// uint8_t lsb = 0x00;		// 0x00

	spi_send_command(TEMPERATURE_SENSOR_CONTROL);
	spi_send_data(msb);
	spi_send_data(lsb);
}

void epd_clear_frame_memory(uint8_t color)
{
	uint8_t value;
	if (color == COLOR_BLACK)
	{
		value = 0x00;
	}
	else
	{
		value = 0xFF;
	}

#ifdef EPD_SIZE_1_54
	epd_set_memory_area(0, 0, EPD_WIDTH -1, EPD_HEIGHT -1);
	epd_set_memory_pointer(0, 0);
	spi_send_command(WRITE_RAM);

	for (int i=0; i < ((EPD_WIDTH / 8) * EPD_HEIGHT); ++i)	// fill whole display memory with 'val'
	{
		spi_send_data(value);
	}
#endif

#ifdef EPD_SIZE_2_13
	epd_set_memory_area(0, 0, EPD_WIDTH -1, EPD_HEIGHT -1);
	// set the frame memory line by line
	for (int j=0; j < EPD_HEIGHT; ++j)
	{
		epd_set_memory_pointer(0, j);
		spi_send_command(WRITE_RAM);
		for (int i=0; i < (EPD_WIDTH / 8); ++i)
		{
			spi_send_data(value);
		}
	}
#endif

}

void epd_set_memory_pointer(uint16_t x, uint16_t y)
{
	spi_send_command(SET_RAM_X_ADDRESS_COUNTER);
	spi_send_data((x>>3) & 0xFF);		// x point must be the multiple of 8 or the last 3 bits will be ignored

	spi_send_command(SET_RAM_Y_ADDRESS_COUNTER);
	spi_send_data(y & 0xFF);
	spi_send_data((y>>8) & 0xFF);

	epd_wait_until_idle();
}

void epd_set_memory_area(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
{
	spi_send_command(SET_RAM_X_ADDRESS_START_END_POSITION);
	spi_send_data((start_x >> 3) & 0xFF);		// x point must be the multiple of 8 or the last 3 bits will be ignored
	spi_send_data((end_x >> 3) & 0xFF);

	spi_send_command(SET_RAM_Y_ADDRESS_START_END_POSITION);
	spi_send_data(start_y & 0xFF);
	spi_send_data((start_y >> 8) & 0xFF);
	spi_send_data(end_y & 0xFF);
	spi_send_data((end_y >> 8) & 0xFF);

}

void epd_display_frame(void)
{
	spi_send_command(DISPLAY_UPDATE_CONTROL_2);
	spi_send_data(0xC4);
	spi_send_command(MASTER_ACTIVATION);
	spi_send_command(TERMINATE_FRAME_READ_WRITE);
	epd_wait_until_idle();
}

void epd_set_frame(const image_buffer_t *image, uint16_t start_x, uint16_t start_y, uint16_t width, uint16_t height)
{
	uint16_t end_x;
	uint16_t end_y;

	start_x &= 0xF8;	// x point must be the multiple of 8 or the last 3 bits will be ignored
	width &= 0xF8;

	// checking if frame is not bigger than display
	if (start_x + width >= EPD_WIDTH)		// for x-axis
	{
		end_x = image->width - 1;
	}
	else
	{
		end_x = start_x + width - 1;
	}

	if (start_y + height >= EPD_HEIGHT)		// for y-axis
	{
		end_y = image->height - 1;
	}
	else
	{
		end_y = start_y + height - 1;
	}

#ifdef EPD_SIZE_1_54
	epd_set_memory_area(start_x, start_y, end_x, end_y);
	epd_set_memory_pointer(start_x, start_y);
	spi_send_command(WRITE_RAM);

    for (int j = 0; j < end_y - start_y + 1; j++)
    {		// fill display memory with data from frame
        for (int i = 0; i < (end_x - start_x + 1) / 8; i++)
        {
            spi_send_data(image->buffer[i + j * (width / 8)]);
        }
    }
#endif

#ifdef EPD_SIZE_2_13
	epd_set_memory_area(start_x, start_y, end_x, end_y);
	// epd_set_memory_pointer(start_x, start_y);
	// spi_send_command(WRITE_RAM);

    for (int j = start_y; j < end_y; j++)
    {
    	epd_set_memory_pointer(start_x, j);
    	spi_send_command(WRITE_RAM);

        for (int i = (start_x / 8); i <= (end_x / 8); i++)
        {
            spi_send_data(image->buffer[(i - (start_x / 8)) + (j - start_y) * (width / 8)]);
        }
    }
#endif


}

void epd_deep_sleep(void)
{
	epd_reset();
	spi_send_command(DEEP_SLEEP_MODE);
	spi_send_data(0x01);
	// epd_wait_until_idle();
}

void epd_wait_until_idle(void)
{
	while (PIND & (1<<PIND7))	// low-idle / high-busy
	{
		_delay_ms(100);		// todo: remove this
	}
}

