/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */
 
#include "button.h"

void button_init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t button_read(void) {
    static uint8_t count[3];
    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9) == 0) {
        if(count[0] < 10) {
            count[0]++;
        } else {
            if(count[0] == 10) {
                count[0]++;
                return 0x01;
            }
        }
    } else {
        count[0] = 0;
    }
    return 0x00;
}
