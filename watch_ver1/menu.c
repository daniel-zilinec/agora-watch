/*
 * menu.c
 *
 *  Created on: 22 Oct 2018
 *      Author: dadan
 */
#include "menu.h"



void menu_show()
{
	// wait until all buttons are depressed
	while (button_pressed());



	// re-initialize display
	epd_reset();			// todo: wasting with power in delay loops
	epd_init_partial(DISPLAY_TEMPERTURE);
	//epd_init_full(DISPLAY_TEMPERTURE);

	// clear display
	epd_clear_frame_memory(COLOR_BLACK);
	epd_display_frame();

	//epd_init_partial(DISPLAY_TEMPERTURE);

	const uint8_t menu_item_count = MENU_ITEM_COUNT;
	uint8_t menu_current_item = 0;

	// CLOCK menu loop
#ifdef DEVICE_TYPE_WATCH
	while (1)
	{
		epd_clear_frame_memory(COLOR_WHITE);

		canvas_display_text(&image_buffer,&font24, "Set alarm", 0, 10, !(menu_current_item == MENU_ITEM_ALARM_TIME));			// menu item 0

		if (g_alarm_enabled)
		{
			canvas_display_text(&image_buffer,&font24, "Alarm:  on", 24, 10, !(menu_current_item == MENU_ITEM_ALARM_ENABLED));	// menu item 1
		}
		else
		{
			canvas_display_text(&image_buffer,&font24, "Alarm: off", 24, 10, !(menu_current_item == MENU_ITEM_ALARM_ENABLED));	// menu item 1
		}
		canvas_display_text(&image_buffer,&font24, "Set time", 48, 10, !(menu_current_item == MENU_ITEM_SET_TIME));				// menu item 2
		canvas_display_text(&image_buffer,&font24, "Show info", 72, 10, !(menu_current_item == MENU_ITEM_SHOW_INFO));			// menu item 3
		// canvas_display_text(&image_buffer,&font24, "Bluetooth", 96, 10, !(menu_current_item == MENU_ITEM_BLUETOOTH));			// menu item 3

		epd_display_frame();

		sw_timer[SW_TIMER_IDLE] = IDLE_TIME;

		while (!button_pressed())		// wait until any button pressed
		{
			if (sw_timer[SW_TIMER_IDLE] == 0)	// when idle too long jump out from menu
				return;
		}

		backlight_enable(DEFAULT_BACKLIGHT_TIME);

		// Button 1 - select menu item
		if (button1_state())
		{
			// run application
			switch (menu_current_item)
			{
				case MENU_ITEM_ALARM_TIME:
					// set alarm time
					app_set_time(&g_alarm_time);
					break;

				case MENU_ITEM_ALARM_ENABLED:
					if (g_alarm_enabled)
					{
						g_alarm_enabled = 0;
					}
					else
					{
						g_alarm_enabled = 1;
					}
					break;

				case MENU_ITEM_SET_TIME:
					app_set_time(&g_time);
					break;

				case MENU_ITEM_SHOW_INFO:
					app_status_screen();
					break;

				case MENU_ITEM_BLUETOOTH:
					app_bluetooth();
					break;

			}
		}
#endif

#ifdef DEVICE_TYPE_CLOCK
		while (1)
		{
			epd_clear_frame_memory(COLOR_WHITE);

			canvas_display_text(&image_buffer,&font24, "Set time", 0, 10, !(menu_current_item == MENU_ITEM_SET_TIME));				// menu item 2
			canvas_display_text(&image_buffer,&font24, "Show info", 24, 10, !(menu_current_item == MENU_ITEM_SHOW_INFO));			// menu item 3
			// canvas_display_text(&image_buffer,&font24, "Bluetooth", 96, 10, !(menu_current_item == MENU_ITEM_BLUETOOTH));			// menu item 3

			epd_display_frame();

			sw_timer[SW_TIMER_IDLE] = IDLE_TIME;

			while (!button_pressed())		// wait until any button pressed
			{
				if (sw_timer[SW_TIMER_IDLE] == 0)	// when idle too long jump out from menu
					return;
			}

			backlight_enable(DEFAULT_BACKLIGHT_TIME);

			// Button 1 - select menu item
			if (button1_state())
			{
				// run application
				switch (menu_current_item)
				{
					case MENU_ITEM_SET_TIME:
						app_set_time(&g_time);
						break;

					case MENU_ITEM_SHOW_INFO:
						app_status_screen();
						break;

				}
			}
#endif

		// Button 2 - quit menu
		if (button2_state())
		{
			return;
		}

		// Button 3 - Move cursor up
		if (button3_state())
		{
			if (menu_current_item > 0)
			{
				--menu_current_item;
			}
		}

		// Button 4 - Move cursor down
		if (button4_state())
		{
			if (menu_current_item < (menu_item_count-1))
			{
				++menu_current_item;
			}
		}

	}

	// wait until all buttons are depressed
	while (button_pressed());
}

