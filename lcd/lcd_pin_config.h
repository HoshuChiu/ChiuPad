#ifndef _LCD_PIN_CONFIG_H_
#define _LCD_PIN_CONFIG_H_

#define BUS_I80_WIDTH               8
#define BUS_I80_PIN_NUM_D0          17
#define BUS_I80_PIN_NUM_D1          18
#define BUS_I80_PIN_NUM_D2          8
#define BUS_I80_PIN_NUM_D3          19
#define BUS_I80_PIN_NUM_D4          20
#define BUS_I80_PIN_NUM_D5          3
#define BUS_I80_PIN_NUM_D6          46
#define BUS_I80_PIN_NUM_D7          9
#if BUS_I80_WIDTH > 8
#define BUS_I80_PIN_NUM_D8          14
#define BUS_I80_PIN_NUM_D9          15
#define BUS_I80_PIN_NUM_D10         16
#define BUS_I80_PIN_NUM_D11         17
#define BUS_I80_PIN_NUM_D12         18
#define BUS_I80_PIN_NUM_D13         19
#define BUS_I80_PIN_NUM_D14         20
#define BUS_I80_PIN_NUM_D15         21
#endif
#define BUS_I80_PIN_NUM_WR             16
#define BUS_I80_PIN_NUM_CS             7
#define BUS_I80_PIN_NUM_DC             15
#define BUS_I80_PIN_NUM_RST            6

#define LCD_PANEL_PIN_NUM_BK_LIGHT       10
#define LCD_PANEL_PIN_IM1           5
#define LCD_PANEL_PIN_IM2           4
#define LCD_PANEL_PIN_LEDK1             13
#define LCD_PANEL_PIN_LEDK2             14
#define LCD_PANEL_PIN_LEDK3             21
#define LCD_PANEL_PIN_LEDK4             47

#endif