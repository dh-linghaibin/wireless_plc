/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _XPT2046_H_
#define _XPT2046_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"
#include "../lv_hal/lv_hal_indev.h"

void xpt2046_init(void);
uint16_t xpt2046_get_x(void);
uint16_t xpt2046_get_y(void);
bool mouse_read(lv_indev_data_t * data);
void xpt2046_loop(void);
    
#ifdef __cplusplus
}
#endif

#endif


