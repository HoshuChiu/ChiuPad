#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "lvgl_support.h"
#include "uimgr.h"

static lv_obj_t *btn;
static lv_obj_t *label;
void btn_click(lv_event_t * e){
    //ESP_LOGI("btn","clk");
    lv_area_t area;
    lv_obj_get_coords((lv_obj_t *)(e->original_target),&area);
    char x[30];
    sprintf(x,"x:%d,y:%d",(int)area.x1+1,(int)area.y1+1);
UI_MODIFY
    lv_obj_set_pos(label,area.x1+1,area.y1+1);
    lv_obj_set_pos(btn,area.x1+1,area.y1+1);
UI_MODIFY_END

}
typedef struct{
    lv_obj_t* canvas1;
    lv_obj_t* screen_spinner;
    lv_color_t* buf_screen_canvas_1;
    //lv_line_create
} ui_init_device_cb_t;
static ui_init_device_cb_t ui_init_device_ele;



#define UI_INIT_BK_COLOR 0xcfcfcf
void ui_init_device(lv_obj_t * screen,lv_display_t* display){
    lv_obj_set_style_bg_color(screen,lv_color_hex(UI_INIT_BK_COLOR),0);
    ui_init_device_ele.canvas1 = lv_canvas_create(screen);
    ui_init_device_ele.buf_screen_canvas_1=heap_caps_malloc(120*120*sizeof(uint16_t),MALLOC_CAP_SPIRAM);

	lv_canvas_set_buffer(ui_init_device_ele.canvas1, ui_init_device_ele.buf_screen_canvas_1, 120, 120,LV_COLOR_FORMAT_RGB565);
	lv_canvas_fill_bg(ui_init_device_ele.canvas1, lv_color_hex(UI_INIT_BK_COLOR), 255);

    lv_layer_t layer;
    lv_canvas_init_layer(ui_init_device_ele.canvas1, &layer);

	//Canvas draw rectangle
	lv_draw_rect_dsc_t screen_canvas_1_rect_dsc_0;
	lv_draw_rect_dsc_init(&screen_canvas_1_rect_dsc_0);
	screen_canvas_1_rect_dsc_0.radius = 5;
	screen_canvas_1_rect_dsc_0.bg_opa = 255;
	screen_canvas_1_rect_dsc_0.bg_color = lv_color_hex(0xefefef);
	screen_canvas_1_rect_dsc_0.bg_grad.dir = LV_GRAD_DIR_NONE;
	screen_canvas_1_rect_dsc_0.border_width = 0;
	screen_canvas_1_rect_dsc_0.border_opa = 255;
	screen_canvas_1_rect_dsc_0.border_color = lv_color_hex(0x000000);
    lv_area_t coords = {0, 0, 20, 80};
    lv_draw_rect(&layer, &screen_canvas_1_rect_dsc_0, &coords);

	//Canvas draw rectangle
	lv_draw_rect_dsc_t screen_canvas_1_rect_dsc_1;
	lv_draw_rect_dsc_init(&screen_canvas_1_rect_dsc_1);
	screen_canvas_1_rect_dsc_1.radius = 5;
	screen_canvas_1_rect_dsc_1.bg_opa = 255;
	screen_canvas_1_rect_dsc_1.bg_color = lv_color_hex(0xefefef);
	screen_canvas_1_rect_dsc_1.bg_grad.dir = LV_GRAD_DIR_NONE;
	screen_canvas_1_rect_dsc_1.border_width = 0;
	screen_canvas_1_rect_dsc_1.border_opa = 255;
	screen_canvas_1_rect_dsc_1.border_color = lv_color_hex(0x000000);
    lv_area_t coords1 = {100, 0, 120, 80};
	lv_draw_rect(&layer, &screen_canvas_1_rect_dsc_1,&coords1);

	//Canvas draw rectangle
	lv_draw_rect_dsc_t screen_canvas_1_rect_dsc_2;
	lv_draw_rect_dsc_init(&screen_canvas_1_rect_dsc_2);
	screen_canvas_1_rect_dsc_2.radius = 3;
	screen_canvas_1_rect_dsc_2.bg_opa = 255;
	screen_canvas_1_rect_dsc_2.bg_color = lv_color_hex(0xa1a1a1);
	screen_canvas_1_rect_dsc_2.bg_grad.dir = LV_GRAD_DIR_NONE;
	screen_canvas_1_rect_dsc_2.border_width = 0;
	screen_canvas_1_rect_dsc_2.border_opa = 255;
	screen_canvas_1_rect_dsc_2.border_color = lv_color_hex(0x000000);
    lv_area_t coords2 = {23, 10, 97, 70};
	lv_draw_rect(&layer, &screen_canvas_1_rect_dsc_2,&coords2);

	//Canvas draw rectangle
	lv_draw_rect_dsc_t screen_canvas_1_rect_dsc_3;
	lv_draw_rect_dsc_init(&screen_canvas_1_rect_dsc_3);
	screen_canvas_1_rect_dsc_3.radius = 4;
	screen_canvas_1_rect_dsc_3.bg_opa = 255;
	screen_canvas_1_rect_dsc_3.bg_color = lv_color_hex(0x636363);
	screen_canvas_1_rect_dsc_3.bg_grad.dir = LV_GRAD_DIR_NONE;
	screen_canvas_1_rect_dsc_3.border_width = 0;
	screen_canvas_1_rect_dsc_3.border_opa = 255;
	screen_canvas_1_rect_dsc_3.border_color = lv_color_hex(0x000000);
    lv_area_t coords3 = {31, 18, 89, 62};
	lv_draw_rect(&layer, &screen_canvas_1_rect_dsc_3,&coords3);

	//Canvas draw rectangle
	lv_draw_rect_dsc_t screen_canvas_1_rect_dsc_4;
	lv_draw_rect_dsc_init(&screen_canvas_1_rect_dsc_4);
	screen_canvas_1_rect_dsc_4.radius = 3;
	screen_canvas_1_rect_dsc_4.bg_opa = 255;
	screen_canvas_1_rect_dsc_4.bg_color = lv_color_make(0xda,0x92,0x2f);
	screen_canvas_1_rect_dsc_4.bg_grad.dir = LV_GRAD_DIR_NONE;
	screen_canvas_1_rect_dsc_4.border_width = 0;
	screen_canvas_1_rect_dsc_4.border_opa = 255;
	screen_canvas_1_rect_dsc_4.border_color = lv_color_hex(0x2F92DA);
    lv_area_t coords4 = {5, 12, 15, 22};
	lv_draw_rect(&layer, &screen_canvas_1_rect_dsc_4,&coords4);

	//Canvas draw rectangle
	lv_draw_rect_dsc_t screen_canvas_1_rect_dsc_5;
	lv_draw_rect_dsc_init(&screen_canvas_1_rect_dsc_5);
	screen_canvas_1_rect_dsc_5.radius = 3;
	screen_canvas_1_rect_dsc_5.bg_opa = 255;
	//screen_canvas_1_rect_dsc_5.bg_color = lv_color_hex(0x2f09fb);
    screen_canvas_1_rect_dsc_5.bg_color = lv_color_make(0x2f,0x09,0xfb);
	screen_canvas_1_rect_dsc_5.bg_grad.dir = LV_GRAD_DIR_NONE;
	screen_canvas_1_rect_dsc_5.border_width = 0;
	screen_canvas_1_rect_dsc_5.border_opa = 255;
	screen_canvas_1_rect_dsc_5.border_color = lv_color_hex(0x000000);
    lv_area_t coords5 = {105, 58, 115, 68};
	lv_draw_rect(&layer, &screen_canvas_1_rect_dsc_5,&coords5);

    lv_canvas_finish_layer(ui_init_device_ele.canvas1, &layer);

	lv_obj_set_pos(ui_init_device_ele.canvas1, 180, 100);
	lv_obj_set_size(ui_init_device_ele.canvas1, 120, 120);
	lv_obj_set_scrollbar_mode(ui_init_device_ele.canvas1, LV_SCROLLBAR_MODE_OFF);
    //btn=lv_btn_create(screen);
    //lv_obj_set_size(btn,120,120);
    //lv_obj_set_pos(btn,0,0);
    //lv_obj_set_style_color_filter_dsc
    //lv_obj_center(buf_screen_canvas_1);

    //Write codes screen_spinner_1
	ui_init_device_ele.screen_spinner = lv_spinner_create(screen);
    
    lv_spinner_set_anim_params(ui_init_device_ele.screen_spinner, 1500, 300);
	lv_obj_set_pos(ui_init_device_ele.screen_spinner, 222, 210);
	lv_obj_set_size(ui_init_device_ele.screen_spinner, 36, 36);

	//Write style for screen_spinner_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_pad_top(ui_init_device_ele.screen_spinner, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui_init_device_ele.screen_spinner, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui_init_device_ele.screen_spinner, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui_init_device_ele.screen_spinner, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui_init_device_ele.screen_spinner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_width(ui_init_device_ele.screen_spinner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui_init_device_ele.screen_spinner, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_spinner_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_arc_width(ui_init_device_ele.screen_spinner, 3, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_opa(ui_init_device_ele.screen_spinner, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_arc_color(ui_init_device_ele.screen_spinner, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR|LV_STATE_DEFAULT);
}
void ui_task(void* arg){
    int x=0,y=0;
    lv_indev_t* indev=((uimgr_arg_t*)arg)->indev;
    lv_display_t* display=((uimgr_arg_t*)arg)->display;
    UI_MODIFY
    ui_init_device(lv_scr_act(),display);
    UI_MODIFY_END
    while (1)
    {
        UI_MODIFY
        //lv_label_set_text_fmt(label,"x:%d,y:%d",x,y);
        UI_MODIFY_END
        vTaskDelay(1);
    }
    
}