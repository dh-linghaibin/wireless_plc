/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _UI_DEBUG_H_
#define _UI_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../lvgl.h"

void ui_debug_create(void);
void ui_debug_set_show(const char * text,lv_color_t color);

#ifdef __cplusplus
}
#endif

#endif
