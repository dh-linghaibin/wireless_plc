/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "simulation.h"

#define AO1_OUT_PORT GPIOA
#define AO1_OUT_PIN  GPIO_Pin_3

#define AO2_OUT_PORT GPIOA
#define AO2_OUT_PIN  GPIO_Pin_2

#define AO3_OUT_PORT GPIOA
#define AO3_OUT_PIN  GPIO_Pin_1

#define AO4_OUT_PORT GPIOA
#define AO4_OUT_PIN  GPIO_Pin_0

void simulation_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz

    GPIO_InitStructure.GPIO_Pin = AO1_OUT_PIN;
    GPIO_Init(AO1_OUT_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = AO2_OUT_PIN;
    GPIO_Init(AO2_OUT_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = AO3_OUT_PIN;
    GPIO_Init(AO3_OUT_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = AO4_OUT_PIN;
    GPIO_Init(AO4_OUT_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_WriteBit(AO1_OUT_PORT,AO1_OUT_PIN,1);
    GPIO_WriteBit(AO2_OUT_PORT,AO2_OUT_PIN,1);
    GPIO_WriteBit(AO3_OUT_PORT,AO3_OUT_PIN,1);
    GPIO_WriteBit(AO4_OUT_PORT,AO4_OUT_PIN,1);
}
