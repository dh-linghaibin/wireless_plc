/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h" 

typedef enum {
    B_NULL = 0,
    B_UP = 1,
    B_DOWN = 2,
    B_LEFT = 3,
    B_RIGHT = 4,
    B_CEN = 5,
} button_flag;

void button_init(void);
button_flag button_get(void);

#ifdef __cplusplus
}
#endif

#endif


