/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "insignal.h"

#define INSI_PORT_1 GPIOB
#define INSI_PORT_2 GPIOB
#define INSI_PORT_3 GPIOA
#define INSI_PORT_4 GPIOA

#define INSI_PIN_1 GPIO_Pin_1
#define INSI_PIN_2 GPIO_Pin_0
#define INSI_PIN_3 GPIO_Pin_7
#define INSI_PIN_4 GPIO_Pin_6

void insignal_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(INSI_PORT_1, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(INSI_PORT_2, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(INSI_PORT_3, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(INSI_PORT_4, &GPIO_InitStructure);
}

uint8_t insignal_read(uint8_t ch) {
    static uint8_t val = 0;
    GPIO_TypeDef*   port;
    uint16_t        pin;
    switch(ch) {
        case 0: {
            port = INSI_PORT_1;
            pin = GPIO_Pin_1;
        } break;
        case 1: {
            port = INSI_PORT_2;
            pin = GPIO_Pin_2;
        } break;
        case 2: {
            port = INSI_PORT_3;
            pin = GPIO_Pin_3;
        } break;
        case 3: {
            port = INSI_PORT_4;
            pin = GPIO_Pin_4;
        } break;
        case 8: {
            val = 0x00;
            port = INSI_PORT_1;
            pin = GPIO_Pin_1;
            if( GPIO_ReadInputDataBit(port,pin) ) {
                val |= (1 << 0);
            } else {
                val &= ~(1 << 0);
            }
            port = INSI_PORT_2;
            pin = GPIO_Pin_2;
            if( GPIO_ReadInputDataBit(port,pin) ) {
                val |= (1 << 1);
            } else {
                val &= ~(1 << 1);
            }
            port = INSI_PORT_3;
            pin = GPIO_Pin_3;
            if( GPIO_ReadInputDataBit(port,pin) ) {
                val |= (1 << 2);
            } else {
                val &= ~(1 << 2);
            }
            port = INSI_PORT_4;
            pin = GPIO_Pin_4;
            if( GPIO_ReadInputDataBit(port,pin) ) {
                val |= (1 << 3);
            } else {
                val &= ~(1 << 3);
            }
            return val;
        } break;
        default: {
            return I_UP;
        } break;
    }
    if( GPIO_ReadInputDataBit(port,pin) ) {
        val |= (1 << ch);
        return I_DOWN;
    } else {
        val &= ~(1 << ch);
        return I_UP;
    }
}

