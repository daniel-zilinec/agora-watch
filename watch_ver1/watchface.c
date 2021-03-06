/*
 * watchface.c
 *
 *  Created on: 10 Nov 2018
 *      Author: dadan
 */
#include "watchface.h"
#include "battery.h"
#include "temperature.h"
#include "main.h"

void watchface_show(const uint8_t watchface_num)
{

#ifdef EPD_SIZE_1_54

	    canvas_display_from_flash(&image_buffer, &time_font[g_time.hours / 10], 48, 152, 1);	// fist digit
	    canvas_display_from_flash(&image_buffer, &time_font[g_time.hours % 10], 48, 104, 1);	// second digit

	    canvas_display_from_flash(&image_buffer, &time_font[10], 48, 96, 1);	// delimiter

	    canvas_display_from_flash(&image_buffer, &time_font[g_time.minutes / 10], 48, 48, 1);	// third digit
	    canvas_display_from_flash(&image_buffer, &time_font[g_time.minutes % 10], 48, 0, 1);		// fourth digit
#endif

#ifdef EPD_SIZE_2_13

	    if (watchface_num == 1)			// debug watchface
	    {
			// time (hh:mm)
			char watchface_time[6];

			watchface_time[0] = '0' + g_time.hours / 10;
			watchface_time[1] = '0'+ g_time.hours % 10;
			watchface_time[2] = ':';
			watchface_time[3] = '0' + g_time.minutes / 10;
			watchface_time[4] = '0' + g_time.minutes % 10;
			watchface_time[5] = 0;
			canvas_display_text(&image_buffer,&font24, watchface_time, 0, 160, 1);

			// date (dd/mm/yyyy)
			char watchface_date[11];
			watchface_date[0] = '0' + g_time.day / 10;
			watchface_date[1] = '0' + g_time.day % 10;
			watchface_date[2] = '/';
			watchface_date[3] = '0' + g_time.month / 10;
			watchface_date[4] = '0' + g_time.month % 10;
			watchface_date[5] = '/';
			uint16_t year = g_time.year;
			watchface_date[6] = '0' + year / 1000;
			year %= 1000;
			watchface_date[7] = '0' + year / 100;
			year %= 100;
			watchface_date[8] = '0' + year / 10;
			watchface_date[9] = '0' + year % 10;
			watchface_date[10] = 0;
			canvas_display_text(&image_buffer,&font24, watchface_date, 24, 76, 1);


			// show battery voltage
			char s_battery_voltage[6];
			battery_get_voltage_string(s_battery_voltage, 6);
			canvas_display_text(&image_buffer,&font24, s_battery_voltage, 0, 0, 1);

			// show battery voltage ADC value
//			char s_battery_adcval[5];
//			uint16_t battery_voltage = g_battery_voltage;
//			s_battery_adcval[0] = '0' + battery_voltage / 1000;
//			battery_voltage -= (battery_voltage / 1000) * 1000;
//
//			s_battery_adcval[1] = '0' + battery_voltage / 100;
//			battery_voltage -= (battery_voltage / 100) * 100;
//
//			s_battery_adcval[2] = '0' + battery_voltage / 10;
//			battery_voltage -= (battery_voltage / 10) * 10;
//
//			s_battery_adcval[3] = '0' + battery_voltage;
//			s_battery_adcval[4] = 0;
//
//			canvas_display_text(&image_buffer,&font24, s_battery_adcval, 48, 0, 1);

			// show battery charge status in %
			char s_battery_percentage[5];
			battery_get_percentage_string(s_battery_percentage, 5);
			canvas_display_text(&image_buffer,&font24, s_battery_percentage, 24, 0, 1);

			// show uptime from last charge
			uint8_t hours;
			uint16_t days;
			char s_uptime[10];

			days = g_uptime.hours_from_last_charge / 24;
			hours = g_uptime.hours_from_last_charge % 24;

			s_uptime[0] = '0' + days / 1000;		// days
			days -= (days/1000) * 1000;
			s_uptime[1] = '0' + days / 100;
			days -= (days/100) * 100;
			s_uptime[2] = '0' + days / 10;
			days -= (days/10) * 10;
			s_uptime[3] = '0' + days % 10;
			s_uptime[4] = 'd';
			s_uptime[5] = ' ';
			s_uptime[6] = '0' + hours / 10;			// hours
			s_uptime[7] = '0' + hours % 10;
			s_uptime[8] = 'h';
			s_uptime[9] = 0;

			canvas_display_text(&image_buffer,&font24, s_uptime, 48, 92, 1);

			// show total uptime
			days = g_uptime.hours_total / 24;
			hours = g_uptime.hours_total % 24;

			s_uptime[0] = '0' + days / 1000;		// days
			days -= (days/1000) * 1000;
			s_uptime[1] = '0' + days / 100;
			days -= (days/100) * 100;
			s_uptime[2] = '0' + days / 10;
			days -= (days/10) * 10;
			s_uptime[3] = '0' + days % 10;
			s_uptime[4] = 'd';
			s_uptime[5] = ' ';
			s_uptime[6] = '0' + hours / 10;			// hours
			s_uptime[7] = '0' + hours % 10;
			s_uptime[8] = 'h';
			s_uptime[9] = 0;

			canvas_display_text(&image_buffer,&font24, s_uptime, 72, 92, 1);

			// show temperature
			char s_temperature[8];
			temperature_celsius_string(g_temperature_raw, s_temperature, 8);
			canvas_display_text(&image_buffer,&font24, s_temperature, 96, 0, 1);

	    }

	    else				// default watchface
	    {
		    //----- BIG DIGITS ---------
		    canvas_display_from_flash(&image_buffer, &time_font[g_time.hours / 10], 8, 176, 1);	// fist digit
		    canvas_display_from_flash(&image_buffer, &time_font[g_time.hours % 10], 8, 128, 1);	// second digit

		    canvas_display_from_flash(&image_buffer, &time_font[10], 8, 120, 1);	// delimiter

		    canvas_display_from_flash(&image_buffer, &time_font[g_time.minutes / 10], 8, 72, 1);	// third digit
		    canvas_display_from_flash(&image_buffer, &time_font[g_time.minutes % 10], 8, 24, 1);		// fourth digit
	    }

#endif
}
