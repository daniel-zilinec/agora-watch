/*
 * epd_1_54.h
 *
 *  Created on: Mar 10, 2018
 *      Author: dadan
 */

#ifndef EPD_1_54_H_
#define EPD_1_54_H_

#include <avr/io.h>
#include "canvas.h"
#include "main.h"

// Temporary temperature of display
#define DISPLAY_TEMPERTURE 25

// #define VCOM_VALUE	0x9C		// default for long time
// #define VCOM_VALUE	0x7C		// original comment
// #define VCOM_VALUE	0xA8		// original value
 #define VCOM_VALUE	0x50        // -1.5 V

// Display colors
#define COLOR_WHITE		1
#define COLOR_BLACK		0


// set display size
//#define EPD_SIZE_1_54
#define EPD_SIZE_2_13


// Display resolution
#ifdef EPD_SIZE_1_54
#define EPD_WIDTH       200		// 200
#define EPD_HEIGHT      200		// 200
#endif

#ifdef EPD_SIZE_2_13		// 2.13" display with resolution 122x250 (but logical resolution is 128x250)
#define EPD_WIDTH       128
#define EPD_HEIGHT      250
#endif

// EPD1IN54 commands
#define DRIVER_OUTPUT_CONTROL                       0x01
#define BOOSTER_SOFT_START_CONTROL                  0x0C
#define GATE_SCAN_START_POSITION                    0x0F
#define DEEP_SLEEP_MODE                             0x10
#define DATA_ENTRY_MODE_SETTING                     0x11
#define SW_RESET                                    0x12
#define TEMPERATURE_SENSOR_CONTROL                  0x1A
#define MASTER_ACTIVATION                           0x20
#define DISPLAY_UPDATE_CONTROL_1                    0x21
#define DISPLAY_UPDATE_CONTROL_2                    0x22
#define WRITE_RAM                                   0x24
#define WRITE_VCOM_REGISTER                         0x2C
#define WRITE_LUT_REGISTER                          0x32
#define SET_DUMMY_LINE_PERIOD                       0x3A
#define SET_GATE_TIME                               0x3B
#define BORDER_WAVEFORM_CONTROL                     0x3C
#define SET_RAM_X_ADDRESS_START_END_POSITION        0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45
#define SET_RAM_X_ADDRESS_COUNTER                   0x4E
#define SET_RAM_Y_ADDRESS_COUNTER                   0x4F
#define TERMINATE_FRAME_READ_WRITE                  0xFF



void epd_init(const unsigned char* lut, int8_t temperature);
void epd_init_full(int8_t temperature);
void epd_init_partial(int8_t temperature);
void epd_reset(void);
void epd_set_temperature(int8_t celsius);
void epd_clear_frame_memory(uint8_t color);
void epd_set_frame(const image_buffer_t *image, uint16_t start_x, uint16_t start_y, uint16_t width, uint16_t height);
void epd_set_memory_area(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);
void epd_set_memory_pointer(uint16_t x, uint16_t y);
void epd_display_frame(void);
void epd_deep_sleep(void);
void epd_wait_until_idle(void);

#endif /* EPD_1_54_H_ */
