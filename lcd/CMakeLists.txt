set(srcs "i80_drive.c" "lcd_panel.c" "touchpad.c")
set(include_dirs ".")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       REQUIRES esp_lcd lvgl esp_lcd_touch_gt911 esp_lcd_touch)