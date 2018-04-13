/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "button.h"

#define BUT_ADD_PORT GPIOA
#define BUT_SUB_PORT GPIOB

#define BUT_ADD_PIN GPIO_Pin_4
#define BUT_SUB_PIN GPIO_Pin_3

void button_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    
    GPIO_InitStructure.GPIO_Pin = BUT_ADD_PIN;
    GPIO_Init(BUT_ADD_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = BUT_SUB_PIN;
    GPIO_Init(BUT_SUB_PORT, &GPIO_InitStructure);
}

button_code button_read(void) {
    if( (GPIO_ReadInputDataBit(BUT_ADD_PORT,BUT_ADD_PIN) == 0) &&  \
        (GPIO_ReadInputDataBit(BUT_SUB_PORT,BUT_SUB_PIN) == 0) ) {
        return B_ALL;
    } else if (GPIO_ReadInputDataBit(BUT_ADD_PORT,BUT_ADD_PIN) == 0) {
        return B_ADD;
    } else if (GPIO_ReadInputDataBit(BUT_SUB_PORT,BUT_SUB_PIN) == 0) {
        return B_SUB;
    } else {
        return B_NULL;
    }
}
