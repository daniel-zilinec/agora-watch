/*
 * basic_apps.c
 *
 *  Created on: 26 Apr 2018
 *      Author: dadan
 */
#include "basic_apps.h"



volatile time_t g_time;	// current time
time_t g_alarm_time;	// alarm is set to this time
uint8_t g_alarm_enabled = 0;

void app_set_time(volatile time_t *time)
{
	time_t new_time;

	new_time.hours = time->hours;
	new_time.minutes = time->minutes;


	epd_reset();			// todo: wasting with power in delay loops
	epd_init_partial(DISPLAY_TEMPERTURE);

	g_dirty_framebuffers = 2;
	epd_clear_frame_memory(COLOR_WHITE);

	// wait until all buttons are depressed
	while (button_pressed());

	int current_digit = 1;
	while (current_digit <= 4)
	{
    	if (g_dirty_framebuffers > 0)				// clean framebuffer when needed
    	{
    		epd_clear_frame_memory(COLOR_WHITE);
    		--g_dirty_framebuffers;
    	}


#ifdef EPD_SIZE_2_13

	    canvas_display_from_flash(&image_buffer, &time_font[new_time.hours / 10], 8, 176, (current_digit == 1) ? 0 : 1);	// fist digit
	    canvas_display_from_flash(&image_buffer, &time_font[new_time.hours % 10], 8, 128, (current_digit == 2) ? 0 : 1);	// second digit

	    canvas_display_from_flash(&image_buffer, &time_font[10], 8, 120, 1);	// delimiter

	    canvas_display_from_flash(&image_buffer, &time_font[new_time.minutes / 10], 8, 72, (current_digit == 3) ? 0 : 1);	// third digit
	    canvas_display_from_flash(&image_buffer, &time_font[new_time.minutes % 10], 8, 24, (current_digit == 4) ? 0 : 1);		// fourth digit
#endif



	    epd_display_frame();

	    // set up idle timer
		sw_timer[SW_TIMER_IDLE] = IDLE_TIME;
		while (!button_pressed())		// wait until any button pressed
		{
			if (sw_timer[SW_TIMER_IDLE] == 0)	// or until time runs out
				return;
		}

		backlight_enable(DEFAULT_BACKLIGHT_TIME);

	    if (button1_state())				// BUTTON 1 - move to next digit
	    {
	    	// check if hours are not more then 23
	    	if ( (current_digit == 1) && ((new_time.hours / 10) == 2) && ((new_time.hours % 10) > 3) )
			{
	    		new_time.hours = 23;
			}
	    	++ current_digit;
	    }

	    if (button2_state())				// BUTTON 2	- cancel
	    {
	    	// wait until all buttons are depressed
	    	while (button_pressed());

	    	return;
	    }

	    if (button3_state())				// BUTTON 3 - increment current digit
	    {
	    	// increment current digit
	    	if (current_digit == 1)		// increment tens of hours
	    	{
	    		if ((new_time.hours / 10) < 2)
	    			new_time.hours += 10;
	    		else
	    			new_time.hours = new_time.hours % 10;
	    	}

	    	if (current_digit == 2)		// increment hours
	    	{
	    		if ((new_time.hours / 10) < 2)		// 00h - 19h
	    		{
	    			if ((new_time.hours % 10) < 9)
	    			{
	    				new_time.hours += 1;
	    			}
	    			else
	    				new_time.hours = (new_time.hours / 10) * 10;
	    		}
	    		else								// 20h - 24h
	    		{
	    			if ((new_time.hours % 10) < 3)
	    				new_time.hours += 1;
	    			else
	    				new_time.hours = (new_time.hours / 10) * 10;
	    		}
	    	}

	    	if (current_digit == 3)		// increment tens of minutes
	    	{
	    		if ((new_time.minutes / 10) < 5)
	    			new_time.minutes += 10;
	    		else
	    			new_time.minutes  = new_time.minutes % 10;
	    	}

	    	if (current_digit == 4)		// increment minutes
	    	{
	    		if ((new_time.minutes % 10) < 9)
	    			new_time.minutes += 1;
	    		else
	    			new_time.minutes  = (new_time.minutes / 10) * 10;
	    	}

	    }

	    if (button4_state())				// BUTTON 4 - decrement current digit
	    {
	    	if (current_digit == 1)		// decrement tens of hours
	    	{
	    		if ((new_time.hours / 10) > 0)
	    			new_time.hours -= 10;
	    		else
	    			new_time.hours = (new_time.hours % 10) + 20;
	    	}

	    	if (current_digit == 2)		// decrement hours
	    	{
	    		if ((new_time.hours / 10) < 2)		// 00h - 19h
	    		{
	    			if ((new_time.hours % 10) > 0)
	    			{
	    				new_time.hours -= 1;
	    			}
	    			else
	    				new_time.hours = ((new_time.hours / 10) * 10 ) + 9;
	    		}
	    		else								// 20h - 24h
	    		{
	    			if ((new_time.hours % 10) > 0)
	    				new_time.hours -= 1;
	    			else
	    				new_time.hours = ((new_time.hours / 10) * 10) + 3;
	    		}
	    	}

	    	if (current_digit == 3)		// decrement tens of minutes
	    	{
	    		if ((new_time.minutes / 10) > 0)
	    			new_time.minutes -= 10;
	    		else
	    			new_time.minutes  = (new_time.minutes % 10) + 50;
	    	}

	    	if (current_digit == 4)		// decrement minutes
	    	{
	    		if ((new_time.minutes % 10) > 0)
	    			new_time.minutes -= 1;
	    		else
	    			new_time.minutes  = ((new_time.minutes / 10) * 10) + 9;
	    	}

	    }

	}

	time->hours = new_time.hours;
	time->minutes = new_time.minutes;
	time->seconds = 0;

	// wait until all buttons are depressed
	while (button_pressed());

}


void app_status_screen(void)
{
	epd_reset();			// todo: wasting with power in delay loops
	epd_init_partial(DISPLAY_TEMPERTURE);
	epd_clear_frame_memory(COLOR_WHITE);

	// wait until all buttons are depressed
	while (button_pressed());

//	canvas_display_text(&image_buffer,&font24, "Battery: 3.86V", 0, 10, 1);
//	canvas_display_text(&image_buffer,&font24, "60%", 24, 10, 1);
//	canvas_display_text(&image_buffer,&font24, "Temp:     24 C", 48, 10, 1);
//	canvas_display_text(&image_buffer,&font24, "Uptime:02d 03h", 72, 10, 1);
//	canvas_display_text(&image_buffer,&font24, "Charge:01d 02h", 96, 10, 1);

	char text[6];

	battery_get_voltage_string(text, 6);							// display battery voltage
	canvas_display_text(&image_buffer,&font24, text, 0, 10, 1);
	canvas_display_text(&image_buffer,&font24, "Battery:", 0, 112, 1);

	battery_get_percentage_string(text, 6);							// battery charge %
	canvas_display_text(&image_buffer,&font24, text, 24, 10, 1);

	epd_display_frame();

	sw_timer[SW_TIMER_IDLE] = IDLE_TIME;
	while (!button_pressed())		// wait until any button pressed
	{
		if (sw_timer[SW_TIMER_IDLE] == 0)	// or until time runs out
			return;
	}

	// wait until all buttons are depressed
	while (button_pressed());
}


void app_bluetooth()
{

}

void backlight_enable(uint8_t timeout)
{
	#ifdef BACKLIGHT_ENABLED

	PORTD |= (1<<PORTD6);
	sw_timer[SW_TIMER_BACKLIGHT] = timeout;

	#endif
}

void backlight_disable()
{
	PORTD &= ~(1<<PORTD6);
}

void alert_enable(uint8_t timeout)
{
	PORTD |= (1<<PORTD5);
	sw_timer[SW_TIMER_ALERT] = timeout;
}

void alert_disable()
{
	PORTD &= ~(1<<PORTD5);
}

