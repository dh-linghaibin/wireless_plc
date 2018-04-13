/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */


#ifndef __INSUGNAL_H
#define __INSUGNAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* 引用头文件 *****************************************************************/
#include "stm32f0xx.h"

typedef enum {
    I_DOWN = 0,
    I_UP = 1,
} ins_sign;

void insignal_init(void);
uint8_t insignal_read(uint8_t ch);

#ifdef __cplusplus
}
#endif

#endif
/***************************************************************END OF FILE****/
