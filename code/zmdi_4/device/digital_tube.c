/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "digital_tube.h"

#define PORT_A GPIOB
#define PORT_B GPIOF
#define PORT_C GPIOF
#define PORT_D GPIOA
#define PORT_E GPIOA
#define PORT_F GPIOA
#define PORT_G GPIOB
#define PORT_DP GPIOB
#define PORT_DG1 GPIOA
#define PORT_DG2 GPIOB

#define PIN_A GPIO_Pin_7
#define PIN_B GPIO_Pin_1
#define PIN_C GPIO_Pin_0
#define PIN_D GPIO_Pin_0
#define PIN_E GPIO_Pin_1
#define PIN_F GPIO_Pin_2
#define PIN_G GPIO_Pin_6
#define PIN_DP GPIO_Pin_5
#define PIN_DG1 GPIO_Pin_3
#define PIN_DG2 GPIO_Pin_4

static volatile const uint8_t show_code[13] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x7c,0x00,0xff};//数码管编码
static volatile uint8_t show[2];//显示数字
static volatile uint8_t choice_num;//轮询标志
static volatile uint8_t point[2];//小数点
static void set_bit_num(uint8_t val);

void digital_tube_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = PIN_A;
    GPIO_Init(PORT_A, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PIN_B;
    GPIO_Init(PORT_B, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PIN_C;
    GPIO_Init(PORT_C, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PIN_D;
    GPIO_Init(PORT_D, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PIN_E;
    GPIO_Init(PORT_E, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PIN_F;
    GPIO_Init(PORT_F, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PIN_G;
    GPIO_Init(PORT_G, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PIN_DP;
    GPIO_Init(PORT_DP, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PIN_DG1;
    GPIO_Init(PORT_DG1, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = PIN_DG2;
    GPIO_Init(PORT_DG2, &GPIO_InitStructure);
    
    GPIO_WriteBit(PORT_DG1,PIN_DG1,Bit_SET);
    GPIO_WriteBit(PORT_DG2,PIN_DG2,Bit_SET);
}

static void set_bit_num(uint8_t val) {
    GPIO_WriteBit(PORT_A,PIN_A,       \
                  (BitAction)( ( (val) >> 0)&0x01 ));
    GPIO_WriteBit(PORT_B,PIN_B,       \
                  (BitAction)( ( (val) >> 1)&0x01 ));
    GPIO_WriteBit(PORT_C,PIN_C,       \
                  (BitAction)( ( (val) >> 2)&0x01 ));
    GPIO_WriteBit(PORT_D,PIN_D,       \
                  (BitAction)( ( (val) >> 3)&0x01 ));
    GPIO_WriteBit(PORT_E,PIN_E,       \
                  (BitAction)( ( (val) >> 4)&0x01 ));
    GPIO_WriteBit(PORT_F,PIN_F,       \
                  (BitAction)( ( (val) >> 5)&0x01 ));
    GPIO_WriteBit(PORT_G,PIN_G,       \
                  (BitAction)( ( (val) >> 6)&0x01 ));
    GPIO_WriteBit(PORT_DP,PIN_DP,       \
                  (BitAction)( ( (val) >> 7)&0x01 ));
}

static void motion_dig(uint8_t choice_num) {
    switch(choice_num) {
        case 0:
        GPIO_WriteBit(PORT_DG2,PIN_DG2,Bit_SET);
        GPIO_WriteBit(PORT_DG1,PIN_DG1,Bit_RESET);
        break;
        case 1:
        GPIO_WriteBit(PORT_DG1,PIN_DG1,Bit_SET);
        GPIO_WriteBit(PORT_DG2,PIN_DG2,Bit_RESET);
        break;
    }
}

void digital_tube_refresh(void) {
    if(point[choice_num] == 0) {
        set_bit_num(show_code[show[choice_num]] & 0x7f);
    } else {
        set_bit_num(show_code[show[choice_num]] | 0x80);
    }
    
    motion_dig(choice_num);
    if(choice_num < 1) {
        choice_num++;
    } else {
        choice_num = 0;
    }
}

void digital_tube_set_num(uint8_t bit,uint8_t num) {
    if(bit > 1) {
        bit  = 1;
    }
    show[bit] = num;
}
void igital_tube_toggle_point(uint8_t point_num,uint8_t val) {
    if(point_num > 1) {
        point_num  = 1;
    }
    if(val == 0) {
        point[point_num] = 0;
    } else {
        point[point_num] = 1;
    }
}
