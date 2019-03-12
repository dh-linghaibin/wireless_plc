/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _READ_AD_H_
#define _READ_AD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h" 
    
void read_ad_init(void);
uint16_t read_ad_get(void);

#ifdef __cplusplus
}
#endif

#endif
