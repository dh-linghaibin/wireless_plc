/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "tlc7226.h"

#define T_WR1_PORT GPIOC
#define T_WR1_PIN  GPIO_Pin_9

#define T_WR2_PORT GPIOC
#define T_WR2_PIN  GPIO_Pin_8

#define T_A0_PORT GPIOD
#define T_A0_PIN  GPIO_Pin_13

#define T_A1_PORT GPIOC
#define T_A1_PIN  GPIO_Pin_7

#define T_A2_PORT GPIOD
#define T_A2_PIN  GPIO_Pin_12

#define T_A3_PORT GPIOC
#define T_A3_PIN  GPIO_Pin_6

#define T_DB0_PORT GPIOA
#define T_DB0_PIN  GPIO_Pin_8

#define T_DB1_PORT GPIOA
#define T_DB1_PIN  GPIO_Pin_9

#define T_DB2_PORT GPIOA
#define T_DB2_PIN  GPIO_Pin_10

#define T_DB3_PORT GPIOB
#define T_DB3_PIN  GPIO_Pin_3

#define T_DB4_PORT GPIOB
#define T_DB4_PIN  GPIO_Pin_4

#define T_DB5_PORT GPIOE
#define T_DB5_PIN  GPIO_Pin_2

#define T_DB6_PORT GPIOE
#define T_DB6_PIN  GPIO_Pin_3

#define T_DB7_PORT GPIOE
#define T_DB7_PIN  GPIO_Pin_4

static void tlc7226_sel(uint8_t chx);
static void tlc7226_set_outport(uint8_t chx);
static void tlc7226_set_data(uint8_t val);

void tlc7226_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����

    GPIO_InitStructure.GPIO_Pin = T_WR1_PIN;
    GPIO_Init(T_WR1_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_WR2_PIN;
    GPIO_Init(T_WR2_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_A0_PIN;
    GPIO_Init(T_A0_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_A1_PIN;
    GPIO_Init(T_A1_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_A2_PIN;
    GPIO_Init(T_A2_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_A3_PIN;
    GPIO_Init(T_A3_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_A3_PIN;
    GPIO_Init(T_A3_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_DB0_PIN;
    GPIO_Init(T_DB0_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_DB1_PIN;
    GPIO_Init(T_DB1_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_DB2_PIN;
    GPIO_Init(T_DB2_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_DB3_PIN;
    GPIO_Init(T_DB3_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_DB4_PIN;
    GPIO_Init(T_DB4_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_DB5_PIN;
    GPIO_Init(T_DB5_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_DB6_PIN;
    GPIO_Init(T_DB6_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = T_DB7_PIN;
    GPIO_Init(T_DB7_PORT, &GPIO_InitStructure);
    
    tlc7226_sel(0);
    tlc7226_set_data(0);
    
    
//    GPIO_SetBits(T_A0_PORT,T_A0_PIN);
//    GPIO_SetBits(T_A1_PORT,T_A1_PIN);
//    
//    GPIO_SetBits(T_A2_PORT,T_A2_PIN);
//    GPIO_SetBits(T_A3_PORT,T_A3_PIN);
//    while(1);
}

void tlc7226_set(uint8_t dev,uint8_t chx,uint8_t val) {
    tlc7226_set_outport(chx);
    tlc7226_set_data(val);
    tlc7226_sel(dev);
}

static void tlc7226_sel(uint8_t chx) {
    switch(chx) {
        case 0: { /* �ر� */
            GPIO_SetBits(T_WR1_PORT,T_WR1_PIN);
            GPIO_SetBits(T_WR2_PORT,T_WR2_PIN);
        } break;
        case 1: { /* ѡ���һ��оƬ */
            GPIO_ResetBits(T_WR1_PORT,T_WR1_PIN);
            GPIO_SetBits(T_WR2_PORT,T_WR2_PIN);
        } break;
        case 2: { /* ѡ��ڶ���оƬ */
            GPIO_SetBits(T_WR1_PORT,T_WR1_PIN);
            GPIO_ResetBits(T_WR2_PORT,T_WR2_PIN);
        } break;
        case 3: { /* һ�� */
            GPIO_ResetBits(T_WR1_PORT,T_WR1_PIN);
            GPIO_ResetBits(T_WR2_PORT,T_WR2_PIN);
        } break;
        default: {
            GPIO_SetBits(T_WR1_PORT,T_WR1_PIN);
            GPIO_SetBits(T_WR2_PORT,T_WR2_PIN);
        } break;
    }
}

static void tlc7226_set_outport(uint8_t chx) {
    switch(chx) {
        case 0: {
            GPIO_ResetBits(T_A0_PORT,T_A0_PIN);
            GPIO_ResetBits(T_A1_PORT,T_A1_PIN);
            
            GPIO_ResetBits(T_A2_PORT,T_A2_PIN);
            GPIO_ResetBits(T_A3_PORT,T_A3_PIN);
        } break;
        case 1: {
            GPIO_SetBits(T_A0_PORT,T_A0_PIN);
            GPIO_ResetBits(T_A1_PORT,T_A1_PIN);
            
            GPIO_SetBits(T_A2_PORT,T_A2_PIN);
            GPIO_ResetBits(T_A3_PORT,T_A3_PIN);
        } break;
        case 2: {
            GPIO_ResetBits(T_A0_PORT,T_A0_PIN);
            GPIO_SetBits(T_A1_PORT,T_A1_PIN);
            
            GPIO_ResetBits(T_A2_PORT,T_A2_PIN);
            GPIO_SetBits(T_A3_PORT,T_A3_PIN);
        } break;
        case 3: {
            GPIO_SetBits(T_A0_PORT,T_A0_PIN);
            GPIO_SetBits(T_A1_PORT,T_A1_PIN);
            
            GPIO_SetBits(T_A2_PORT,T_A2_PIN);
            GPIO_SetBits(T_A3_PORT,T_A3_PIN);
        } break;
    }
}

static void tlc7226_set_data(uint8_t val) {
    GPIO_WriteBit(T_DB0_PORT,T_DB0_PIN,       \
                  (BitAction)( ( (val) >> 0)&0x01 ));
    GPIO_WriteBit(T_DB1_PORT,T_DB1_PIN,       \
                  (BitAction)( ( (val) >> 1)&0x01 ));
    GPIO_WriteBit(T_DB2_PORT,T_DB2_PIN,       \
                  (BitAction)( ( (val) >> 2)&0x01 ));
    GPIO_WriteBit(T_DB3_PORT,T_DB3_PIN,       \
                  (BitAction)( ( (val) >> 3)&0x01 ));
    GPIO_WriteBit(T_DB4_PORT,T_DB4_PIN,       \
                  (BitAction)( ( (val) >> 4)&0x01 ));
    GPIO_WriteBit(T_DB5_PORT,T_DB5_PIN,       \
                  (BitAction)( ( (val) >> 5)&0x01 ));
    GPIO_WriteBit(T_DB6_PORT,T_DB6_PIN,       \
                  (BitAction)( ( (val) >> 6)&0x01 ));
    GPIO_WriteBit(T_DB7_PORT,T_DB7_PIN,       \
                  (BitAction)( ( (val) >> 7)&0x01 ));
}
