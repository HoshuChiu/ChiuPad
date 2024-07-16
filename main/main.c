#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_touch.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_dma_utils.h"
#include "esp_task_wdt.h"
#include "i80_drive.h"
#include "lcd_panel.h"
#include "touchpad.h"

#include "lvgl.h"
#include "lvgl_support.h"

typedef struct{
    esp_lcd_panel_handle_t panel_handle;
    uint16_t* gmem;
} display_drv_t;

static lv_obj_t *btn;
static lv_obj_t *label;

void app_main(void)
{
    
    //初始化背光
    lcd_backlight_init();

    //初始化显存
    uint16_t* gmem=NULL;
    gmem=(uint16_t*)heap_caps_calloc(sizeof(uint16_t),LCD_H_RES * LCD_V_PRT,MALLOC_CAP_INTERNAL);
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

    //初始化触摸
    esp_lcd_touch_handle_t touchpad_handle=NULL;
    touchpad_init(&touchpad_handle);

    //设置LVGL显示设备
    lv_display_set_driver_data(display,panel_handle);
    lv_display_set_user_data(display,gmem);
    lv_display_set_buffers(display, gmem, NULL, LCD_H_RES * LCD_V_PRT * sizeof(uint16_t),LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(display, disp_flush);

    //设置LVGL触摸板设备
    lv_indev_t * indev = lv_indev_create();           /*Create an input device*/
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);  /*Touch pad is a pointer-like device*/
    lv_indev_set_driver_data(indev,touchpad_handle);
    lv_indev_set_read_cb(indev, touchpad_read_cb);
    
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

    //创建ui控件
    lv_obj_t *scr = lv_disp_get_scr_act(display);
    btn = lv_btn_create(lv_scr_act());
    lv_obj_set_width(btn,20);
    lv_obj_set_height(btn,200);
    lv_obj_set_pos(btn,20,20);
    label=lv_label_create(lv_scr_act());
    lv_obj_set_size(label,200,20);
    lv_obj_set_pos(label,200,200);
    lv_label_set_text(label,"Show TP data");
        
        //lv_obj_invalidate(btn);
    lvgl_lock_init();
    //lv_disp_flush_ready(display);
    xTaskCreate(lvgl_port_task, "GUI", (8 * 1024), (void*)display, 2, NULL);
    lcd_backlight_on();
    int x=0,y=0;
    while(1){
        if (lvgl_lock(-1)) {
        //example_lvgl_demo_ui(disp);
        // Release the mutex
        //lv_obj_t *scr = lv_disp_get_scr_act(display);
        //btn = lv_btn_create(scr);
        //lv_obj_invalidate(scr);
        
        lv_obj_set_pos(btn,x,y);
        //if(x<100)x++;else x=0;
        //if(y<100)y++;else y=0;
        //esp_lcd_touch_read_data(touchpad_handle);
        uint16_t touchpad_x[5]={0,0,0,0,0};
        uint16_t touchpad_y[5]={0,0,0,0,0};
        uint8_t touchpad_cnt = 0;
        //esp_lcd_touch_get_coordinates(touchpad_handle,touchpad_x,touchpad_y,NULL,&touchpad_cnt,5);
        char touch_data[100];
        lv_point_t point={.x=0,.y=0};
        lv_indev_get_point(indev,&point);
        sprintf(touch_data,"试试x1:%d,y1:%d,x2:%d,y2:%d,x3:%d,y3:%d",(int)point.x,(int)point.y,touchpad_x[1],touchpad_y[1],touchpad_x[2],touchpad_y[2]);
        lv_label_set_text(label,touch_data);
        lvgl_unlock();
        vTaskDelay(1);
        }
    }
    
    ESP_LOGI("ok","ok");
}
