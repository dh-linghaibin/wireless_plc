/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _EC11_H_
#define _EC11_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h" 

typedef enum {
    E_NULL = 0,
    E_BUTTON = 1,
    E_RIGHT = 2,
    E_LEFT = 3
} ec11_flag;

void ec11_init(void);
ec11_flag ec11_get(void);

#ifdef __cplusplus
}
#endif

#endif


