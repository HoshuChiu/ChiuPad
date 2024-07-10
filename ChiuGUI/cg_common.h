#ifndef _CG_COMMON_H_
#define _CG_COMMON_H_

#include <stdint.h>
#include "chiu_gui_config.h"

#if CG_COLOR_MODE==CG_COLOR_MODE_RGB565
    typedef uint16_t cg_Pixel_t;
#else
    typedef uint8_t cg_Pixel_t;
#endif

typedef struct{
    cg_SpaceScalar_t x;
    cg_SpaceScalar_t y;
} cg_Coordinate_t;

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    cg_Pixel_t pixel;
} cg_Color_t;

#endif