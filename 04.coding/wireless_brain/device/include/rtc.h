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

u8 rtc_init(void);//RTC初始化
ErrorStatus rtc_set_time(u8 hour,u8 min,u8 sec,u8 ampm);         //RTC时间设置
ErrorStatus rtc_set_date(u8 year,u8 month,u8 date,u8 week);       //RTC日期设置
void rtc_set_alarma(u8 week,u8 hour,u8 min,u8 sec);      //设置闹钟时间(按星期闹铃,24小时制)
void rtc_set_wakeup(u32 wksel,u16 cnt);               //周期性唤醒定时器设置

#ifdef __cplusplus
}
#endif

#endif


