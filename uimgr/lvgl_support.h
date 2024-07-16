#ifndef _LVGL_SUPPORT_H_
#define _LVGL_SUPPORT_H_

#include "lvgl.h"

#define LVGL_TICK_MSECS 2

void touchpad_read_cb(lv_indev_t * indev_drv, lv_indev_data_t * data);
void disp_flush(lv_display_t * disp, const lv_area_t * area, lv_color_t * color_p);

void lvgl_tick(void* args);
void lvgl_lock_init(void);
bool lvgl_lock(int timeout_ms);
void lvgl_unlock(void);
void lvgl_port_task(void *arg);

#endif