/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"
#include "sdkconfig.h"
    
void TIM3_Int_Init(u16 arr,u16 psc);

#ifdef __cplusplus
}
#endif

#endif


