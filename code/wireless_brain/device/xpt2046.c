/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "xpt2046.h"
#include "spi.h"

#define XPT_CS_SET      GPIO_SetBits(GPIOA, GPIO_Pin_15) 
#define XPT_CS_RESET    GPIO_ResetBits(GPIOA, GPIO_Pin_15)

void xpt2046_init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOB时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//复用功能输出   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
    
    XPT_CS_RESET;
    SPI3_ReadWriteByte(0x80);
    SPI3_ReadWriteByte(0x00);
    SPI3_ReadWriteByte(0x00);
    XPT_CS_SET;
}

uint16_t xpt2046_get_x(void) {
    uint16_t LSB, MSB;
    uint8_t adress;
    uint8_t dataX;
    uint16_t touch_x = 0;
    XPT_CS_RESET;
    adress = 0xD0;
    SPI3_ReadWriteByte(adress);
    adress = 0x00;
    dataX = SPI3_ReadWriteByte(adress);
    MSB = dataX;
    adress = 0x00;
    dataX = SPI3_ReadWriteByte(adress);
    LSB = dataX;
    XPT_CS_SET;
    touch_x = ((MSB<<8)|(LSB))>>3;
    touch_x -= 300;
    touch_x =  touch_x/15;
    return touch_x;
}

uint16_t xpt2046_get_y(void) {
    uint16_t touch_y;
    uint16_t LSB, MSB;
    uint8_t adress;
    uint8_t dataY;
    XPT_CS_RESET;
    adress = 0x90;
    SPI3_ReadWriteByte(adress);
    adress = 0x00;
    dataY = SPI3_ReadWriteByte(adress);
    MSB = dataY;
    adress = 0x00;
    dataY = SPI3_ReadWriteByte(adress);
    LSB = dataY;
    XPT_CS_SET;
    touch_y = ((MSB<<8)|(LSB))>>3;
    touch_y -= 350;
    touch_y =  touch_y/11;
    return touch_y;
}

