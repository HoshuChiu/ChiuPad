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
#include "i80_drive.h"
#include "lcd_panel.h"

typedef struct{
    esp_lcd_panel_handle_t panel_handle;
    uint16_t* gmem;
} display_drv_t;

static int haha;
static int heihei;
SemaphoreHandle_t xMutex;

void draw_hw(void* args){
    heihei=0;
    uint16_t hehe=0;
    uint16_t sign=1;
    display_drv_t* drv=(display_drv_t*)args;
    uint16_t* gmem=drv->gmem;
    esp_lcd_panel_handle_t p_handle=drv->panel_handle;
    uint16_t color=0x1230;
    while(1){
        //更新画面
        
        hehe=sign?hehe+1:hehe-1;
        //memset(gmem,color+hehe,LCD_H_RES * LCD_V_RES * sizeof(uint16_t));
        for(int i=0;i<LCD_H_RES*LCD_V_RES;i++){
            gmem[i]=color;
        }
        uint32_t xxx=0;
        color++;
        if(hehe==0xf){
        sign=0;
        //todo:计算fps等
        }else{
            if(hehe==0x0){
                sign=1;
            }
        }
        //todo:睡眠直到应当开始传输
        vTaskDelay(1);
        //todo:计算fps等
        haha=0;
        //传输画面
            xSemaphoreGive(xMutex);
            xSemaphoreTake(xMutex,100);
        esp_lcd_panel_draw_bitmap(p_handle, 0, 0, 170, 320, gmem);
        //等待画面传输完毕
        while(xSemaphoreTake(xMutex,100)==pdFALSE);
        ESP_LOGI("debug","%d",heihei);
        heihei=0;
    }
}

void gui_tick(void* args){
    haha++;
    heihei++;
    //ESP_LOGI("gui_tick","hahaa");
}

void app_main(void)
{
    
    //初始化背光
    lcd_backlight_init();
    //vTaskDelay(200);

    //初始化I8080总线
    esp_lcd_panel_io_handle_t io_handle = NULL;
    init_i80_bus(&io_handle, NULL);

    //初始化ST7789
    esp_lcd_panel_handle_t panel_handle = NULL;
    lcd_panel_init(io_handle, &panel_handle);
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    //初始化显存
    uint16_t* gmem=NULL;
    uint32_t malloc_flags = 0;
    //ESP_ERROR_CHECK(esp_dma_malloc(LCD_H_RES * LCD_V_RES * sizeof(uint16_t), malloc_flags, (void *)&gmem, NULL));
    gmem=(uint16_t*)malloc(LCD_H_RES * LCD_V_RES * sizeof(uint16_t));
    memset(gmem,0x00,LCD_H_RES * LCD_V_RES * sizeof(uint16_t));
    ESP_LOGI("[GMem Allocated]","%p",gmem);

    //初始化定时器
    ESP_LOGI("main", "Install gui tick timer");
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &gui_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, 1 * 1000));

    //初始化互斥锁
    xMutex=xSemaphoreCreateBinary();

    //启动任务
    display_drv_t driver={
        .gmem=gmem,
        .panel_handle=panel_handle
    };
    xTaskCreate(draw_hw, "GUI", (4 * 1024), &driver, 2, NULL);
    lcd_backlight_on();
    uint16_t x=0;
    
    printf("ok\n");
}
