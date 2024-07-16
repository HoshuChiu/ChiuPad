set(srcs "ui_task.c" "lvgl_support.c")
set(include_dirs ".")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       REQUIRES lvgl esp_lcd esp_lcd_touch)