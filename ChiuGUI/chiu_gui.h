#ifndef _CHIU_GUI_H_
#define _CHIU_GUI_H_

#include "cg_common.h"

typedef struct {
    cg_Pixel_t* gmem;
    uint32_t width;
    uint32_t height;
} cg_InitConfig_t;

void cg_Init(cg_InitConfig_t* config);
void cg_TimerHandler();

#endif