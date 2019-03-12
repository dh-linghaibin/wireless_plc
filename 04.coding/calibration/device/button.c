/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "button.h"

#define B_UP_PORT GPIOC
#define B_UP_PIN  GPIO_Pin_4

#define B_DOWN_PORT GPIOA
#define B_DOWN_PIN  GPIO_Pin_5

#define B_LEFT_PORT GPIOA
#define B_LEFT_PIN  GPIO_Pin_7

#define B_RIGHT_PORT GPIOA
#define B_RIGHT_PIN  GPIO_Pin_6

#define B_CEN_PORT GPIOA
#define B_CEN_PIN  GPIO_Pin_4

void button_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输出模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    
    GPIO_InitStructure.GPIO_Pin = B_UP_PIN;
    GPIO_Init(B_UP_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = B_DOWN_PIN;
    GPIO_Init(B_DOWN_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = B_LEFT_PIN;
    GPIO_Init(B_LEFT_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = B_RIGHT_PIN;
    GPIO_Init(B_RIGHT_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = B_CEN_PIN;
    GPIO_Init(B_CEN_PORT, &GPIO_InitStructure);
}

button_flag button_get(void) {
    static uint8_t flag = 0;
    if(GPIO_ReadInputDataBit(B_UP_PORT,B_UP_PIN) == 0) {
        if(flag != B_UP) {
            flag = B_UP;
            return B_UP;
        }
    } else if(GPIO_ReadInputDataBit(B_DOWN_PORT,B_DOWN_PIN) == 0) {
        if(flag != B_DOWN) {
            flag = B_DOWN;
            return B_DOWN;
        }
    } else if(GPIO_ReadInputDataBit(B_LEFT_PORT,B_LEFT_PIN) == 0) {
        if(flag != B_LEFT) {
            flag = B_LEFT;
            return B_LEFT;
        }
    } else if(GPIO_ReadInputDataBit(B_RIGHT_PORT,B_RIGHT_PIN) == 0) {
        if(flag != B_RIGHT) {
            flag = B_RIGHT;
            return B_RIGHT;
        }
    } else if(GPIO_ReadInputDataBit(B_CEN_PORT,B_CEN_PIN) == 0) {
        if(flag != B_CEN) {
            flag = B_CEN;
            return B_CEN;
        }
    } else {
        flag = B_NULL;
        return B_NULL;
    }
}
