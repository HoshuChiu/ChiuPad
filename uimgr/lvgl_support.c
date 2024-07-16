#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_touch.h"
#include "esp_log.h"

#include "lvgl_support.h"

static SemaphoreHandle_t lvgl_mux = NULL;

void lvgl_tick(void* args){
    lv_tick_inc(LVGL_TICK_MSECS);
}

void lvgl_port_task(void *arg)
{
    ESP_LOGI("Main", "Starting LVGL task");
    while (1) {
        // Lock the mutex due to the LVGL APIs are not thread-safe
        if (lvgl_lock(-1)) {
            
            lv_timer_handler();
            lvgl_unlock();
            vTaskDelay(1);
        }
    }
}

void disp_flush(lv_display_t * disp, const lv_area_t * area, lv_color_t * color_p){
    
    esp_lcd_panel_handle_t* p_handle=(esp_lcd_panel_handle_t*)(lv_display_get_driver_data(disp));
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    //ESP_LOGI("disp_flush","(%d,%d)(%d,%d)",offsetx1,offsety1,offsetx2,offsety2);
    //if(offsety1<LCD_V_PRT)
        //esp_lcd_panel_draw_bitmap(p_handle, 0,0,LCD_H_RES,LCD_V_PRT, color_p);
    //else
        esp_lcd_panel_draw_bitmap(p_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_p);
    //lv_disp_flush_ready(disp);
}

void touchpad_read_cb(lv_indev_t * indev_drv, lv_indev_data_t * data)
{
    uint16_t touchpad_x[1] = {0};
    uint16_t touchpad_y[1] = {0};
    uint8_t touchpad_cnt = 0;
    esp_lcd_touch_handle_t tp_handle=(esp_lcd_touch_handle_t)lv_indev_get_driver_data(indev_drv);
    esp_lcd_touch_read_data(tp_handle);
    bool pressed=esp_lcd_touch_get_coordinates(tp_handle,touchpad_x,touchpad_y,NULL,&touchpad_cnt,1);
    /*`touchpad_is_pressed` and `touchpad_get_xy` needs to be implemented by you*/
    if(pressed&&touchpad_cnt>0) {
        data->point.x=touchpad_x[0];
        data->point.y=touchpad_y[0];
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }

}

bool lvgl_lock(int timeout_ms)
{
    // Convert timeout in milliseconds to FreeRTOS ticks
    // If `timeout_ms` is set to -1, the program will block until the condition is met
    const TickType_t timeout_ticks = (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE;
}

void lvgl_unlock(void)
{
    xSemaphoreGiveRecursive(lvgl_mux);
}

void lvgl_lock_init(void){
    lvgl_mux = xSemaphoreCreateRecursiveMutex();
}

void lv_log_me(lv_log_level_t level, const char *buf){
    ESP_LOGI("LV","%s",buf);
}