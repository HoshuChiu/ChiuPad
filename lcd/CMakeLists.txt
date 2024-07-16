set(srcs "i80_drive.c" "lcd_panel.c")
set(include_dirs ".")

idf_component_register(SRCS "touchpad.c" "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       REQUIRES esp_lcd lvgl)