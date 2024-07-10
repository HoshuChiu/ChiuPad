#ifndef _CG_WIDGET_H_
#define _CG_WIDGET_H_

#include "cg_renderer.h"

typedef enum{
    CG_ELEMENT_PIXEL=0,
    CG_ELEMENT_RECTANGLE
} cg_ElementType_e;






typedef struct {
    cg_Element_t* elements;
    uint32_t element_count;
    cg_Widget_t* childs;
    uint32_t child_count;
} cg_Widget_t;

#endif