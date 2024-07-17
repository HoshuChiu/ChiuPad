#ifndef _UI_MGR_H_
#define _UI_MGR_H_

typedef struct{
    lv_indev_t * indev;
    lv_display_t * display;
} uimgr_arg_t;

void ui_task(void* arg);

#endif