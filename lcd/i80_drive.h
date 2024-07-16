#ifndef _I80_DRIVE_H_
#define _I80_DRIVE_H_

#include "lcd_pin_config.h"

#define PSRAM_DATA_ALIGNMENT   64
#define LCD_PIXEL_CLOCK_HZ     (30* 1000 * 1000)

void init_i80_bus(esp_lcd_panel_io_handle_t *io_handle, void *user_ctx);

#endif