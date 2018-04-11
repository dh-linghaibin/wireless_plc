/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _TSAK_SET_H_
#define _TSAK_SET_H_

#ifdef __cplusplus
extern "C" {
#endif

//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

typedef enum {
    M_PAGE1_SHOW = 0,
    M_PAGE2_SET_MENU = 1,
    M_PAGE2_SET_ADDR = 2,
    M_PAGE3_SET_BR = 3,
} menu_flg;

typedef enum {
    M_LENE_ADDR = 0,
    M_LINE_BR = 1,
} menu_LINE;

void task_set_init(void);
void task_set_create(void);

#ifdef __cplusplus
}
#endif

#endif


