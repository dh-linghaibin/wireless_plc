/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "simulation.h"

#define S_24V_ENADBLE_PORT GPIOC
#define S_24V_ENADBLE_PIN  GPIO_Pin_13

#define S_OUT_ENADBLE_PORT GPIOC
#define S_OUT_ENADBLE_PIN  GPIO_Pin_14

#define S_VOUT_ENADBLE_PORT GPIOC
#define S_VOUT_ENADBLE_PIN  GPIO_Pin_15

#define S_PT100_PORT GPIOC
#define S_PT100_PIN  GPIO_Pin_12

#define S_PT1000_PORT GPIOC
#define S_PT1000_PIN  GPIO_Pin_11

#define S_NTC10K_PORT GPIOC
#define S_NTC10K_PIN  GPIO_Pin_10

void simulation_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz

    GPIO_InitStructure.GPIO_Pin = S_24V_ENADBLE_PIN;
    GPIO_Init(S_24V_ENADBLE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = S_OUT_ENADBLE_PIN;
    GPIO_Init(S_OUT_ENADBLE_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = S_VOUT_ENADBLE_PIN;
    GPIO_Init(S_VOUT_ENADBLE_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = S_PT100_PIN;
    GPIO_Init(S_PT100_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = S_PT1000_PIN;
    GPIO_Init(S_PT1000_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = S_NTC10K_PIN;
    GPIO_Init(S_NTC10K_PORT, &GPIO_InitStructure);
    
    simulation_set_24v(0);
    simulation_set_enab(0);
    simulation_set_venab(0);
    simulation_set_pt100(0);
    simulation_set_pt1000(0);
    simulation_set_ntc10k(0);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_WriteBit(GPIOE,GPIO_Pin_6,0);
}

void simulation_set_24v(uint8_t val) {
    GPIO_WriteBit(S_24V_ENADBLE_PORT,S_24V_ENADBLE_PIN,val);
}

void simulation_set_enab(uint8_t val) {
    GPIO_WriteBit(S_OUT_ENADBLE_PORT,S_OUT_ENADBLE_PIN,val);
}

void simulation_set_venab(uint8_t val) {
    GPIO_WriteBit(S_VOUT_ENADBLE_PORT,S_VOUT_ENADBLE_PIN,val);
}

void simulation_set_pt100(uint8_t val) {
    GPIO_WriteBit(S_PT100_PORT,S_PT100_PIN,val);
}

void simulation_set_pt1000(uint8_t val) {
    GPIO_WriteBit(S_PT1000_PORT,S_PT1000_PIN,val);
}

void simulation_set_ntc10k(uint8_t val) {
    GPIO_WriteBit(S_NTC10K_PORT,S_NTC10K_PIN,val);
}

void simulation_set_ad(uint8_t val) {
    GPIO_WriteBit(GPIOE,GPIO_Pin_6,val);
}
