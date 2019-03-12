/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _RTC_H_
#define _RTC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"

u8 rtc_init(void);//RTC��ʼ��
ErrorStatus rtc_set_time(u8 hour,u8 min,u8 sec,u8 ampm);         //RTCʱ������
ErrorStatus rtc_set_date(u8 year,u8 month,u8 date,u8 week);       //RTC��������
void rtc_set_alarma(u8 week,u8 hour,u8 min,u8 sec);      //��������ʱ��(����������,24Сʱ��)
void rtc_set_wakeup(u32 wksel,u16 cnt);               //�����Ի��Ѷ�ʱ������

#ifdef __cplusplus
}
#endif

#endif


