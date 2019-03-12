/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _CORE_TEMP_H_
#define _CORE_TEMP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"

#define ADC_TEMP 16 //通道16 温度传感器的ADC通道

void Adc_Temperate_Init(void);//温度传感器初始化
short Get_Temprate(void);  //获取温度值
    
#ifdef __cplusplus
}
#endif

#endif


