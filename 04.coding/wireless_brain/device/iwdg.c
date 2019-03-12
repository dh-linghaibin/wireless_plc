/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "iwdg.h"

void iwdg_init(u8 prer,u16 rlr) {
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶ�IWDG->PR IWDG->RLR��д
    IWDG_SetPrescaler(prer); //����IWDG��Ƶϵ��
    IWDG_SetReload(rlr);   //����IWDGװ��ֵ
    IWDG_ReloadCounter(); //reload
    IWDG_Enable();       //ʹ�ܿ��Ź�
}

void iwdg_feed(void) {
    IWDG_ReloadCounter();//reload
}
