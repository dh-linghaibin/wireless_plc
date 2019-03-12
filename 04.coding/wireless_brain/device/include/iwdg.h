/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _IWDG_H_
#define _IWDG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h" 

void iwdg_init(u8 prer,u16 rlr);
void iwdg_feed(void);

#ifdef __cplusplus
}
#endif

#endif
