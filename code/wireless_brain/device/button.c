/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */
 
#include "button.h"

void button_init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//���ù������   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
}
