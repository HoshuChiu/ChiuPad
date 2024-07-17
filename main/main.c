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
#include "nvs_flash.h"
#include "nvs.h"

#include "i80_drive.h"
#include "lcd_panel.h"
#include "touchpad.h"

#include "lvgl.h"
#include "lvgl_support.h"
#include "uimgr.h"


static void IRAM_ATTR isr_handler(void* arg) {
    xTaskResumeFromISR((TaskHandle_t)arg);
}

void app_main(void)
{
    
    //初始化背光
    lcd_backlight_init();

    //初始化显存
    uint16_t* gmem=NULL;
    gmem=(uint16_t*)heap_caps_calloc(sizeof(uint16_t),LCD_H_RES * LCD_V_PRT,MALLOC_CAP_DMA);
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
        
        //lv_obj_invalidate(btn);
    lvgl_lock_init();
    //lv_disp_flush_ready(display);
    xTaskCreate(lvgl_port_task, "LVGL_DRV", (8 * 1024), (void*)display, 2, NULL);

    TaskHandle_t lvgl_task = xTaskGetHandle("LVGL_DRV");
    
    //同步中断
    gpio_config_t sync_gpio_config = {
        .mode = GPIO_MODE_INPUT,
        .intr_type=GPIO_INTR_POSEDGE,
        .pin_bit_mask = 1ULL << LCD_PIN_NUM_TE_SYNC,
        .pull_down_en=0,
        .pull_up_en=1
    };
    ESP_ERROR_CHECK(gpio_config(&sync_gpio_config));
    gpio_install_isr_service(0);
    gpio_isr_handler_add(LCD_PIN_NUM_TE_SYNC,isr_handler,lvgl_task);

    uimgr_arg_t* uimgr_arg=malloc(sizeof (uimgr_arg_t));
    uimgr_arg->indev=indev;
    uimgr_arg->display=display;
    xTaskCreate(ui_task, "GUI", (4 * 1024), uimgr_arg, 1, NULL);
    lcd_backlight_on();
    
    ESP_LOGI("ok","ok");
}
