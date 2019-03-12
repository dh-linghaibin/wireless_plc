/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "ec11.h"

#define E_BUTTON_PORT GPIOA
#define E_BUTTON_PIN  GPIO_Pin_0

#define E_A_PORT GPIOA
#define E_A_PIN  GPIO_Pin_2

#define E_B_PORT GPIOA
#define E_B_PIN  GPIO_Pin_1

void ec11_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    
    GPIO_InitStructure.GPIO_Pin = E_BUTTON_PIN;
    GPIO_Init(E_BUTTON_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = E_A_PIN;
    GPIO_Init(E_A_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = E_B_PIN;
    GPIO_Init(E_B_PORT, &GPIO_InitStructure);
}

ec11_flag ec11_get(void) {
    static uint8_t a_flag = 0;
    uint8_t b_flag = E_NULL;
    if(GPIO_ReadInputDataBit(E_A_PORT,E_A_PIN) == 1) {
        if(a_flag == 0) {
            a_flag = 1;
        }
    } else {
        if(a_flag == 1) {
            a_flag = 0;
            if(GPIO_ReadInputDataBit(E_B_PORT,E_B_PIN) == 0) {
                return E_RIGHT;
            } else {
                return E_LEFT;
            }
        }
    }
    return b_flag;
}
