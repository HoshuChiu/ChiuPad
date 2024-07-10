#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"
#include "i80_drive.h"
#include "lcd_panel.h"

static const char *TAG = "ChiuPad_V1";
extern SemaphoreHandle_t xMutex;
static bool example_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    xSemaphoreGiveFromISR(xMutex,NULL);
    return false;
}

void init_i80_bus(esp_lcd_panel_io_handle_t *io_handle, void *user_ctx)
{
    ESP_LOGI(TAG, "Initialize Intel 8080 bus");
    esp_lcd_i80_bus_handle_t i80_bus = NULL;
    esp_lcd_i80_bus_config_t bus_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .dc_gpio_num = BUS_I80_PIN_NUM_DC,
        .wr_gpio_num = BUS_I80_PIN_NUM_WR,
        .data_gpio_nums = {
            BUS_I80_PIN_NUM_D0,
            BUS_I80_PIN_NUM_D1,
            BUS_I80_PIN_NUM_D2,
            BUS_I80_PIN_NUM_D3,
            BUS_I80_PIN_NUM_D4,
            BUS_I80_PIN_NUM_D5,
            BUS_I80_PIN_NUM_D6,
            BUS_I80_PIN_NUM_D7,
#if BUS_I80_WIDTH > 8
            BUS_I80_PIN_NUM_D8,
            BUS_I80_PIN_NUM_D9,
            BUS_I80_PIN_NUM_D10,
            BUS_I80_PIN_NUM_D11,
            BUS_I80_PIN_NUM_D12,
            BUS_I80_PIN_NUM_D13,
            BUS_I80_PIN_NUM_D14,
            BUS_I80_PIN_NUM_D15,
#endif
        },
        .bus_width = BUS_I80_WIDTH,
        .max_transfer_bytes = LCD_H_RES * LCD_V_RES * sizeof(uint16_t),
        .psram_trans_align = PSRAM_DATA_ALIGNMENT,
        .sram_trans_align = 4,
    };
    ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &i80_bus));

    esp_lcd_panel_io_i80_config_t io_config = {
        .cs_gpio_num = BUS_I80_PIN_NUM_CS,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .trans_queue_depth = 10,
        .dc_levels = {
            .dc_idle_level = 0,
            .dc_cmd_level = 0,
            .dc_dummy_level = 0,
            .dc_data_level = 1,
        },
        .flags = {
            .swap_color_bytes = !LV_COLOR_16_SWAP, // Swap can be done in LvGL (default) or DMA
        },
        .on_color_trans_done = example_notify_lvgl_flush_ready,
        .user_ctx = user_ctx,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, io_handle));
}