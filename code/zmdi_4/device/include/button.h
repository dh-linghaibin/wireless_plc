/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */


#ifndef __BUTTON_H
#define __BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

/* 引用头文件 *****************************************************************/
#include "stm32f0xx.h"

typedef enum {
    B_NULL = 0,
    B_ADD = 1,
    B_SUB = 2,
    B_ALL = 3,
} button_code;

void button_init(void);
button_code button_read(void);

#ifdef __cplusplus
}
#endif

#endif
/***************************************************************END OF FILE****/
