#ifndef _LCD_PANEL_H_
#define _LCD_PANEL_H_

#include "lcd_pin_config.h"

#define LCD_H_RES              320
#define LCD_V_RES              170
#define LCD_X_OFFSET           35
#define LCD_Y_OFFSET           0

#define LCD_I80_CONTROLLER_ST7789 1

#if LCD_I80_CONTROLLER_ST7789
#define LCD_CMD_BITS           8
#define LCD_PARAM_BITS         8
#elif CONFIG_EXAMPLE_LCD_I80_CONTROLLER_NT35510
#define LCD_CMD_BITS           16
#define LCD_PARAM_BITS         16
#elif CONFIG_EXAMPLE_LCD_I80_CONTROLLER_ILI9341
#define LCD_CMD_BITS           8
#define LCD_PARAM_BITS         8
#endif

#define LV_COLOR_16_SWAP        0

#define LCD_BK_LIGHT_ON_LEVEL  1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL

void lcd_backlight_init();
void lcd_backlight_on();
void lcd_backlight_off();
void lcd_panel_init(esp_lcd_panel_io_handle_t io_handle, esp_lcd_panel_handle_t *panel);

#endif