/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _TSAK_GUI_H_
#define _TSAK_GUI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"

void task_gui_init(void);
void task_gui_create(void);
void task_gui_set(uint8_t cmd);
uint8_t task_gui_get_sleep_flag(void);

#ifdef __cplusplus
}
#endif

#endif


