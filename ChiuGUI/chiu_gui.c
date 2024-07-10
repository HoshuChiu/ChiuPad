#include <assert.h>
#include <string.h>
#include "chiu_gui.h"

static uint32_t cg_TickMainCounter;
static cg_Pixel_t* cg_GraphicMem;
static uint8_t cg_PixelByte;
static uint32_t cg_Width;
static uint32_t cg_Height;

void cg_Init(cg_InitConfig_t* config){
    cg_TickMainCounter=0;
    cg_GraphicMem=config->gmem;
    cg_Width=config->width;
    cg_Height=config->height;
    assert((void*)cg_GraphicMem!=(void* )0);
    assert(cg_Width!=0);
    assert(cg_Height!=0);
}

void cg_TimerHandler(){

}

void cg_Update(uint32_t delta_ms){
    
}