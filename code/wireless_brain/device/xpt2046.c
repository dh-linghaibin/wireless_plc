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

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOBʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//���ù������   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//���ù������   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
    
    
    NVIC_InitTypeDef   NVIC_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource10);
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;//LINE0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
    EXTI_Init(&EXTI_InitStructure);//����
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);//����
    
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//���ù������   
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���ù���
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
//    GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
//    
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource7);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line7;//LINE0
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
//    EXTI_Init(&EXTI_InitStructure);//����
//    
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�0
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
//    NVIC_Init(&NVIC_InitStructure);//����
    SPI3_SetSpeed(SPI_BaudRatePrescaler_128);
    XPT_CS_RESET;
    SPI3_ReadWriteByte(0x80);
    SPI3_ReadWriteByte(0x00);
    SPI3_ReadWriteByte(0x00);
    XPT_CS_SET;
}

void EXTI15_10_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line10)!=RESET) {
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10) == 0) {
            EXTI_ClearITPendingBit(EXTI_Line10);
        } else {
            EXTI_ClearITPendingBit(EXTI_Line10);
        }
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
}

void EXTI9_5_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line7)!=RESET) {
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
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

lv_indev_data_t poin;

void xpt2046_loop(void) {
    static uint16_t test = 0;
    if(test < 10000) {
        test++;
    } else {
        test = 0;
         SPI3_SetSpeed(SPI_BaudRatePrescaler_128);
    poin.point.x = xpt2046_get_x();
    poin.point.y = xpt2046_get_y();
    if( poin.point.x < 4000) {
        poin.state = LV_INDEV_STATE_PR;
    } else {
        poin.state = LV_INDEV_STATE_REL;
    }
    //printf("%d %d \n", poin.point.x, poin.point.y);
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