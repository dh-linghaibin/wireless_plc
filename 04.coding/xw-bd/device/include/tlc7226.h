/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _TLC7226_H_
#define _TLC7226_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h" 

void tlc7226_init(void);
void tlc7226_set(uint8_t dev,uint8_t chx,uint8_t val);

#ifdef __cplusplus
}
#endif

#endif


