/*****************************************************************************
 * Modified Waveshare arduino library.
 *
 *
 * Original message:
  | File      	:	EPD_1in54.c
  | Author      :   Waveshare team
  | Function    :   Electronic paper driver
  | Info        :
  ----------------
  |	This version:   V2.0
  | Date        :   2018-11-14
  | Info        :
  1.Remove:ImageBuff[EPD_HEIGHT * EPD_WIDTH / 8]
  2.Change:EPD_Display(uint8_t *Image)
    Need to pass parameters: pointer to cached data
  3.Change:
    EPD_RST -> EPD_RST_PIN
    EPD_DC -> EPD_DC_PIN
    EPD_CS -> EPD_CS_PIN
    EPD_BUSY -> EPD_BUSY_PIN
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
#include "EPD_2in13.h"
#include "spi.h"
#include "basic_apps.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

static const unsigned char lut_v3_full_update[]= {
	0x80,	0x4A,	0x40,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x40,	0x4A,	0x80,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x80,	0x4A,	0x40,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x40,	0x4A,	0x80,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xF,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0xF,	0x0,	0x0,	0xF,	0x0,	0x0,	0x2,
	0xF,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x1,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
	0x22,	0x22,	0x22,	0x22,	0x22,	0x22,	0x0,	0x0,	0x0,
	0x22,	0x17,	0x41,	0x0,	0x32,	0x36
};

static const unsigned char lut_v3_partial_update[]= {
	0x0,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x14,0x0,0x0,0x0,0x0,0x0,0x0,
	0x1,0x0,0x0,0x0,0x0,0x0,0x0,
	0x1,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x0,
	0x22,0x17,0x41,0x00,0x32,0x36,
};

/******************************************************************************
  function :	Software reset
  parameter:
******************************************************************************/
static void EPD_Reset(void)
{
//    DEV_Digital_Write(EPD_RST_PIN, 1);
//    DEV_Delay_ms(200);
//    DEV_Digital_Write(EPD_RST_PIN, 0);
//    DEV_Delay_ms(200);
//    DEV_Digital_Write(EPD_RST_PIN, 1);
//    DEV_Delay_ms(200);

	PORTB |= (1<<PORTB0);
	_delay_ms(200);		// was 200
	PORTB &= ~(1<<PORTB0);
	_delay_ms(200);		// was 200
	PORTB |= (1<<PORTB0);
	_delay_ms(200);		// was 200
}

/******************************************************************************
  function :	send command
  parameter:
     Reg : Command register
******************************************************************************/
static void EPD_SendCommand(uint8_t Reg)
{
//    DEV_Digital_Write(EPD_DC_PIN, 0);
//    DEV_Digital_Write(EPD_CS_PIN, 0);
//    DEV_SPI_WriteByte(Reg);
//    DEV_Digital_Write(EPD_CS_PIN, 1);

	spi_send_command(Reg);
}

/******************************************************************************
  function :	send data
  parameter:
    Data : Write data
******************************************************************************/
static void EPD_SendData(uint8_t Data)
{
//    DEV_Digital_Write(EPD_DC_PIN, 1);
//    DEV_Digital_Write(EPD_CS_PIN, 0);
//    DEV_SPI_WriteByte(Data);
//    DEV_Digital_Write(EPD_CS_PIN, 1);

	spi_send_data(Data);
}

/******************************************************************************
  function :	Wait until the busy_pin goes LOW
  parameter:
******************************************************************************/
void EPD_WaitUntilIdle(void)
{
//    Debug("e-Paper busy\r\n");
//    while (DEV_Digital_Read(EPD_BUSY_PIN) == 1) {     //LOW: idle, HIGH: busy
//        DEV_Delay_ms(100);
//    }
//    Debug("e-Paper busy release\r\n");

	while (PIND & (1<<PIND7))	// low-idle / high-busy
	{
		mcu_sleep();
	}
}

/******************************************************************************
  function :	Turn On Display
  parameter:
******************************************************************************/
void EPD_TurnOnDisplay(void)
{
    EPD_SendCommand(0x22);
    EPD_SendData(0xC7);
    //EPD_SendData(0x0c);
    EPD_SendCommand(0x20);
    EPD_WaitUntilIdle();
}

/******************************************************************************
  function :	Initialize the e-Paper register
  parameter:
******************************************************************************/
uint8_t EPD_Init(uint8_t update)
{
    EPD_Reset();

    if (update == FULL_UPDATE) 
	{
        EPD_WaitUntilIdle();
        EPD_SendCommand(0x12); // soft reset
        EPD_WaitUntilIdle();

        EPD_SendCommand(0x01); //Driver output control
        EPD_SendData(0xF9);
        EPD_SendData(0x00);
        EPD_SendData(0x00);

        EPD_SendCommand(0x11); //data entry mode
        EPD_SendData(0x03);

        EPD_SendCommand(0x44); //set Ram-X address start/end position

        EPD_SendCommand(0x3C); //BorderWavefrom
        EPD_SendData(0x05);

		EPD_SendCommand(0x3C); //BorderWavefrom
		EPD_SendData(0x05);

		EPD_SendCommand(0x21); //  Display update control
		EPD_SendData(0x00);
		EPD_SendData(0x80);

		EPD_SendCommand(0x18); //Read built-in temperature sensor
		EPD_SendData(0x80);


        EPD_WaitUntilIdle();
		EPD_Lut(lut_v3_full_update);
    } 
	else 
	{
		EPD_Lut(lut_v3_partial_update);
		
		EPD_SendCommand(0x37);
		EPD_SendData(0x00);
		EPD_SendData(0x00);
		EPD_SendData(0x00);
		EPD_SendData(0x00);
		EPD_SendData(0x00);
		EPD_SendData(0x40);
		EPD_SendData(0x00);
		EPD_SendData(0x00);
		EPD_SendData(0x00);
		EPD_SendData(0x00);
		
		EPD_SendCommand(0x3C);	//BorderWavefrom
		EPD_SendData(0x80);		
		
		EPD_SendCommand(0x22); //Display Update Sequence Option
		EPD_SendData(0xC0);    // Enable clock and  Enable analog
		EPD_SendCommand(0x20);  //Activate Display Update Sequence
		EPD_WaitUntilIdle();
    }
	
    return 0;
}

/**
    @brief: private function to specify the memory area for data R/W
*/
static void EPD_SetWindows(int x_start, int y_start, int x_end, int y_end)
{
    EPD_SendCommand(0x44);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    EPD_SendData((x_start >> 3) & 0xFF);
    EPD_SendData((x_end >> 3) & 0xFF);
    EPD_SendCommand(0x45);
    EPD_SendData(y_start & 0xFF);
    EPD_SendData((y_start >> 8) & 0xFF);
    EPD_SendData(y_end & 0xFF);
    EPD_SendData((y_end >> 8) & 0xFF);
}

/**
    @brief: private function to specify the start point for data R/W
*/
static void EPD_SetCursor(int x, int y)
{
    EPD_SendCommand(0x4E);
    EPD_SendData((x >> 3) & 0xFF);		// x point must be the multiple of 8 or the last 3 bits will be ignored
	
    EPD_SendCommand(0X4F);
    EPD_SendData(y & 0xFF);
    EPD_SendData((y >> 8) & 0xFF);
	
    EPD_WaitUntilIdle();
}

/******************************************************************************
  function :	Clear screen
  parameter:
******************************************************************************/
void EPD_Clear(void)
{
    uint16_t Width, Height;
    Width = (EPD_B72_WIDTH % 8 == 0) ? (EPD_B72_WIDTH / 8 ) : (EPD_B72_WIDTH / 8 + 1);
    Height = EPD_B72_HEIGHT;
    EPD_SendCommand(0x24);
    for (uint16_t j = 0; j < Height; j++) 
	{
        for (uint16_t i = 0; i < Width; i++) 
		{
            EPD_SendData(INVERTED_COLORS ? 0xFF : 0);
        	// EPD_SendData(0X00);
        }
    }
	
    EPD_TurnOnDisplay();
}

/******************************************************************************
  function :	Sends the image buffer in RAM to e-Paper and displays
  parameter:
******************************************************************************/
void EPD_Display(uint8_t *Image)
{
    uint16_t Width, Height;
    Width = (EPD_B72_WIDTH % 8 == 0) ? (EPD_B72_WIDTH / 8 ) : (EPD_B72_WIDTH / 8 + 1);
    Height = EPD_B72_HEIGHT;

    EPD_SendCommand(0x24);
    for (uint16_t j = 0; j < Height; j++) 
	{
        for (uint16_t i = 0; i < Width; i++)
		 {
            EPD_SendData(pgm_read_byte(&Image[i + j * Width])  ^ (INVERTED_COLORS ? 0xFF : 0));
        }
    }
	
    EPD_TurnOnDisplay();
}

void EPD_DisplayWindows(uint8_t *Image, uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    uint16_t Width, Height;
    Width = ((Xend - Xstart) % 8 == 0) ? ((Xend - Xstart) / 8 ) : ((Xend - Xstart) / 8 + 1);
    Height = Yend - Ystart;

    EPD_SetWindows(Xstart, Ystart, Xend, Yend);
	
    uint16_t i, j;
    for (j = 0; j < Height; j++) 
	{
        EPD_SetCursor(Xstart, Ystart + j);
        EPD_SendCommand(0x24);
        for (i = 0; i < Width; i++) 
		{
            EPD_SendData(Image[i + j * Width] ^ (INVERTED_COLORS ? 0xFF : 0));
        }
    }
}

void EPD_DisplayPart(uint8_t *Image)
{
    uint16_t Width, Height;
	
    Width = (EPD_B72_WIDTH % 8 == 0) ? (EPD_B72_WIDTH / 8 ) : (EPD_B72_WIDTH / 8 + 1);
    Height = EPD_B72_HEIGHT;
    EPD_SendCommand(0x24);
	
    for (uint16_t j = 0; j < Height; j++) 
	{
        for (uint16_t i = 0; i < Width; i++) 
		{
            EPD_SendData(Image[i + j * Width] ^ (INVERTED_COLORS ? 0xFF : 0));
        }
    }

    EPD_SendCommand(0x26);   // Write Black and White image to RAM
	
    for (uint16_t j = 0; j < Height; j++) 
	{
        for (uint16_t i = 0; i < Width; i++) 
		{
            EPD_SendData(~Image[i + j * Width] ^ (INVERTED_COLORS ? 0xFF : 0));
        }
    }
	
    EPD_TurnOnDisplay();
}

void EPD_DisplayPartWindows(uint8_t *Image, uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    uint16_t Width, Height;
    Width = ((Xend - Xstart) % 8 == 0) ? ((Xend - Xstart) / 8 ) : ((Xend - Xstart) / 8 + 1);
    Height = Yend - Ystart;

    EPD_SetWindows(Xstart, Ystart, Xend, Yend);
	
    uint16_t i, j;
    for (j = 0; j < Height; j++) 
	{
        EPD_SetCursor(Xstart, Ystart + j);
        EPD_SendCommand(0x24);
        for (i = 0; i < Width; i++) 
		{
            EPD_SendData(Image[i + j * Width] ^ (INVERTED_COLORS ? 0xFF : 0));
        }
    }

    for (j = 0; j < Height; j++) 
	{
        EPD_SetCursor(Xstart, Ystart + j);
        EPD_SendCommand(0x26);
        for (i = 0; i < Width; i++) 
		{
            EPD_SendData(~Image[i + j * Width] ^ (INVERTED_COLORS ? 0xFF : 0));
        }
    }
}

/******************************************************************************
  function :	Enter sleep mode
  parameter:
******************************************************************************/
void EPD_Sleep(void)
{
    EPD_SendCommand(0x22); //POWER OFF
    EPD_SendData(0xC3);
    EPD_SendCommand(0x20);

    EPD_SendCommand(0x10); //enter deep sleep
    EPD_SendData(0x01);
    // DEV_Delay_ms(100);
    _delay_ms(100);
}

/******************************************************************************
function :	Send lut data and configuration
parameter:	
    lut :   lut data
******************************************************************************/
void EPD_Lut(const unsigned char *lut)
{
	unsigned char count;
	EPD_SendCommand(0x32);
	for(count = 0; count < 153; count++) 
	{
		EPD_SendData(lut[count]);
	}

	EPD_SendCommand(0x3f);
	EPD_SendData(*(lut+153));
	
	EPD_SendCommand(0x03);		// gate voltage
	EPD_SendData(*(lut+154));
	
	EPD_SendCommand(0x04);		// source voltage
	EPD_SendData(*(lut+155));	// VSH
	EPD_SendData(*(lut+156));	// VSH2
	EPD_SendData(*(lut+157));	// VSL
	
	EPD_SendCommand(0x2c);		// VCOM
	EPD_SendData(*(lut+158));
}
