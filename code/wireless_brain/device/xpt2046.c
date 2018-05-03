/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "xpt2046.h"
#include "spi.h"
#include "stdio.h"

#define XPT_CS_SET      GPIO_SetBits(GPIOD, GPIO_Pin_2) 
#define XPT_CS_RESET    GPIO_ResetBits(GPIOD, GPIO_Pin_2)

void xpt2046_init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//复用功能输出   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//复用功能输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化

    NVIC_InitTypeDef   NVIC_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;//LINE0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
    EXTI_Init(&EXTI_InitStructure);//配置
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置

    SPI3_SetSpeed(SPI_BaudRatePrescaler_64);
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
    adress = 0x90;//0x90
    SPI3_ReadWriteByte(adress);
    adress = 0x00;
    dataX = SPI3_ReadWriteByte(adress);
    MSB = dataX;
    adress = 0x00;
    dataX = SPI3_ReadWriteByte(adress);
    LSB = dataX;
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
    adress = 0xD0;//0xD0
    SPI3_ReadWriteByte(adress);
    adress = 0x00;
    dataY = SPI3_ReadWriteByte(adress);
    MSB = dataY;
    adress = 0x00;
    dataY = SPI3_ReadWriteByte(adress);
    LSB = dataY;
    touch_y = ((MSB<<8)|(LSB))>>3;
    touch_y -= 350;
    touch_y =  touch_y/11;
    return touch_y;
}

static lv_indev_data_t poin;


void EXTI3_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line3)!=RESET) {
        printf("up\n");
        XPT_CS_RESET;
        poin.point.x = xpt2046_get_y();
        poin.point.y = 240-xpt2046_get_x();
        XPT_CS_SET;
        if( poin.point.x < 4000) {
            poin.state = LV_INDEV_STATE_PR;
        } else {
            poin.state = LV_INDEV_STATE_REL;
        }
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}


void xpt2046_loop(void) {
    static uint16_t test = 0;
    if(test < 20) {
        test++;
    } else {
        test = 0;
        XPT_CS_RESET;
        poin.point.x = xpt2046_get_y();
        poin.point.y = 240-xpt2046_get_x();
        XPT_CS_SET;
        if( poin.point.x < 4000) {
            poin.state = LV_INDEV_STATE_PR;
        } else {
            poin.state = LV_INDEV_STATE_REL;
        }
    }
}

bool mouse_read(lv_indev_data_t * data)
{
    /*Store the collected data*/
    data->point.x = poin.point.x;
    data->point.y = poin.point.y;
    data->state = poin.state;
    return false;
}