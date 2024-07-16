#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_dma_utils.h"
#include "esp_task_wdt.h"
#include "i80_drive.h"
#include "lcd_panel.h"
#include "i2c_driver.h"

#include "lvgl.h"

typedef struct{
    esp_lcd_panel_handle_t panel_handle;
    uint16_t* gmem;
} display_drv_t;

#define LVGL_TICK_MSECS 2

static int haha;
static int heihei;
static lv_obj_t *btn;
//SemaphoreHandle_t xMutex;
static SemaphoreHandle_t lvgl_mux = NULL;
bool example_lvgl_lock(int timeout_ms)
{
    // Convert timeout in milliseconds to FreeRTOS ticks
    // If `timeout_ms` is set to -1, the program will block until the condition is met
    const TickType_t timeout_ticks = (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE;
}

void example_lvgl_unlock(void)
{
    xSemaphoreGiveRecursive(lvgl_mux);
}
void disp_flush(lv_display_t * disp, const lv_area_t * area, lv_color_t * color_p);
static void lvgl_port_task(void *arg)
{
    ESP_LOGI("Main", "Starting LVGL task");
    while (1) {
        // Lock the mutex due to the LVGL APIs are not thread-safe
        if (example_lvgl_lock(-1)) {
            
            lv_timer_handler();
            example_lvgl_unlock();
            vTaskDelay(1);
        }
        /*
        if (task_delay_ms > EXAMPLE_LVGL_TASK_MAX_DELAY_MS) {
            task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
        } else if (task_delay_ms < EXAMPLE_LVGL_TASK_MIN_DELAY_MS) {
            task_delay_ms = EXAMPLE_LVGL_TASK_MIN_DELAY_MS;
        }*/
        
    }
}

void disp_flush(lv_display_t * disp, const lv_area_t * area, lv_color_t * color_p){
    
    esp_lcd_panel_handle_t* p_handle=(esp_lcd_panel_handle_t*)(lv_display_get_driver_data(disp));
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    //ESP_LOGI("disp_flush","(%d,%d)(%d,%d)",offsetx1,offsety1,offsetx2,offsety2);
    esp_lcd_panel_draw_bitmap(p_handle, 0,0,LCD_H_RES,LCD_V_RES, color_p);
    //lv_disp_flush_ready(disp);
}

void lvgl_tick(void* args){
    lv_tick_inc(LVGL_TICK_MSECS);
        //btn = lv_btn_create(scr);
        //lv_obj_invalidate((lv_obj_t *)args);
}

void lv_log_me(lv_log_level_t level, const char *buf){
    ESP_LOGI("LV","%s",buf);
}

void app_main(void)
{
    
    //初始化背光
    lcd_backlight_init();
    //vTaskDelay(200);

    //初始化显存
    uint16_t* gmem=NULL;
    //ESP_ERROR_CHECK(esp_dma_malloc(LCD_H_RES * LCD_V_RES * sizeof(uint16_t), 0, (void *)&gmem, NULL));
    gmem=(uint16_t*)heap_caps_calloc(sizeof(uint16_t),LCD_H_RES * LCD_V_RES,MALLOC_CAP_INTERNAL);
    if(gmem){
        ESP_LOGI("[GMem Allocated]","%p",gmem);
    }else{
        ESP_LOGE("[GMem not Allocated]","%p",gmem);
        return;
    }
    
    //初始化LVGL
    lv_init();
    lv_display_t *display = lv_display_create(LCD_H_RES, LCD_V_RES);

    //初始化I8080总线
    esp_lcd_panel_io_handle_t io_handle = NULL;
    init_i80_bus(&io_handle, display);

    //初始化ST7789
    esp_lcd_panel_handle_t panel_handle = NULL;
    lcd_panel_init(io_handle, &panel_handle);
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    //初始化IIC总线

    //设置LVGL
    lv_display_set_driver_data(display,panel_handle);
    lv_display_set_user_data(display,gmem);
    lv_display_set_buffers(display, gmem, NULL, LCD_H_RES * LCD_V_RES * sizeof(uint16_t),LV_DISPLAY_RENDER_MODE_DIRECT);
    lv_display_set_flush_cb(display, disp_flush);
    //lv_log_register_print_cb(lv_log_me);
    
    //初始化定时器
    ESP_LOGI("main", "Install gui tick timer");
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &lvgl_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_MSECS * 1000));

    //初始化互斥锁
    //xMutex=xSemaphoreCreateBinary();

    //启动任务
    //display_drv_t driver={
    //    .gmem=gmem,
    //    .panel_handle=panel_handle
    //};
    
    lv_obj_t *scr = lv_disp_get_scr_act(display);
        btn = lv_btn_create(lv_scr_act());
        lv_obj_set_width(btn,20);
        lv_obj_set_height(btn,200);
        lv_obj_set_pos(btn,20,20);
    
    //TaskHandle_t xHandle = xTaskGetHandle("IDLE0");
    //esp_task_wdt_delete(xHandle);
        
        //lv_obj_invalidate(btn);
    lvgl_mux = xSemaphoreCreateRecursiveMutex();
    //lv_disp_flush_ready(display);
    xTaskCreate(lvgl_port_task, "GUI", (8 * 1024), (void*)display, 2, NULL);
    lcd_backlight_on();
    int x=0,y=0;
    while(1){
        if (example_lvgl_lock(-1)) {
        //example_lvgl_demo_ui(disp);
        // Release the mutex
        //lv_obj_t *scr = lv_disp_get_scr_act(display);
        //btn = lv_btn_create(scr);
        //lv_obj_invalidate(scr);
        
        lv_obj_set_pos(btn,x,y);
        if(x<100)x++;else x=0;
        if(y<100)y++;else y=0;
        example_lvgl_unlock();
        vTaskDelay(1);
        }
    }
    
    ESP_LOGI("ok","ok");
}
