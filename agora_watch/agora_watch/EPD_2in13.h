/*****************************************************************************
* Modified Waveshare arduino library.
*
*
* Original message:
* | File      	:	EPD_2in13.h
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V2.0
* | Date        :   2018-11-14
* | Info        :   
* 1.Remove:ImageBuff[EPD_HEIGHT * EPD_WIDTH / 8]
* 2.Change:EPD_Display(uint8_t *Image)
*   Need to pass parameters: pointer to cached data
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef _EPD2IN13_H
#define _EPD2IN13_H

#include <avr/io.h>

// Display resolution
#define EPD_B72_WIDTH       122
#define EPD_B72_HEIGHT      250

#define FULL_UPDATE 0
#define PART_UPDATE 1

uint8_t EPD_Init(uint8_t update);
void EPD_Clear(void);
void EPD_TurnOnDisplay(void);
void EPD_Display(uint8_t *Image);
void EPD_DisplayWindows(uint8_t *Image, uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
void EPD_DisplayPart(uint8_t *Image);
void EPD_DisplayPartWindows(uint8_t *Image, uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
void EPD_Sleep(void);
void EPD_Lut(const unsigned char *lut);

#endif
