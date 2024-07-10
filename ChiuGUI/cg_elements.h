#ifndef _CG_ELEMENTS_H_
#define _CG_ELEMENTS_H_

#include "cg_renderer.h"

/**
 * cg_RectangleArgs_t
 * 该类型定义了长方形的绘制参数
 */
typedef struct
{
    cg_SpaceScalar_t width;
    cg_SpaceScalar_t height;
    cg_SpaceScalar_t radius;
    cg_Color_t color;
} cg_RectangleArgs_t;

#endif