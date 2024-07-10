#ifndef _CG_RENDERER_H_
#define _CG_RENDERER_H_

#include "cg_common.h"

#if CG_PEN_STYLE_ON == 1
typedef struct
{
    /* data */
} cg_PenStyle_t;

cg_Pixel_t penShader(cg_Coordinate_t coord,cg_PenStyle_t* style);

#endif

#if CG_BRUSH_STYLE_ON == 1
typedef struct{


} cg_BrushStyle_t;

cg_Pixel_t BrushShader(cg_Coordinate_t coord,cg_BrushStyle_t* style);
#endif

typedef void* cg_ElementArgs_p;

typedef cg_Element_t;

/**
 * cg_PenImplement_f
 * 该类型定义了图元的边线绘制回调函数
 */
typedef void (*cg_PenImplement_f)(cg_Element_t* element);

/**
 * cg_BrushImplement_f
 * 该类型定义了图元的填充绘制回调函数
 */
typedef void (*cg_BrushImplement_f)(cg_Element_t* element);

/**
 * cg_Element_t
 * 该类型定义了图元类
 */
typedef struct {
    cg_BrushImplement_f brush;
#if CG_BRUSH_STYLE_ON == 1
    cg_BrushStyle_t brush_style;
#endif
    cg_PenImplement_f pen;
#if CG_PEN_STYLE_ON == 1
    cg_PenStyle_t pen_style;
#endif
    cg_Coordinate_t coord;
    cg_ElementArgs_p args;
} cg_Element_t;

#endif