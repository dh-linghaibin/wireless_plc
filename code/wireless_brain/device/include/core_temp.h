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

#define ADC_TEMP 16 //ͨ��16 �¶ȴ�������ADCͨ��

void Adc_Temperate_Init(void);//�¶ȴ�������ʼ��
short Get_Temprate(void);  //��ȡ�¶�ֵ
    
#ifdef __cplusplus
}
#endif

#endif


