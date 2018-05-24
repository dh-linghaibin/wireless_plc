/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "iwdg.h"

void iwdg_init(u8 prer,u16 rlr) {
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对IWDG->PR IWDG->RLR的写
    IWDG_SetPrescaler(prer); //设置IWDG分频系数
    IWDG_SetReload(rlr);   //设置IWDG装载值
    IWDG_ReloadCounter(); //reload
    IWDG_Enable();       //使能看门狗
}

void iwdg_feed(void) {
    IWDG_ReloadCounter();//reload
}
