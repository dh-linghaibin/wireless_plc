/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#include "can.h"
#include <cstring>
#include "task_can.h"

#define HAL_REMAP_PA11_PA12     (SYSCFG_CFGR1_PA11_PA12_RMP) 
#define __HAL_REMAP_PIN_ENABLE(__PIN_REMAP__) \
    do {assert_param(IS_HAL_REMAP_PIN((__PIN_REMAP__))); \
        SYSCFG->CFGR1 |= (__PIN_REMAP__);       \
    }while(0)

static uint8_t can_id = 0x06;

void bxcan_init(void) {
    GPIO_InitTypeDef                    GPIO_InitStructure;
    CAN_InitTypeDef                     CAN_InitStructure;
    CAN_FilterInitTypeDef               CAN_FilterInitStructure;
    NVIC_InitTypeDef                    NVIC_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_15);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    __HAL_REMAP_PIN_ENABLE(HAL_REMAP_PA11_PA12);
    
    /* Enable GPIO clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    /* Enable CAN clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);
    
    /* Connect the involved CAN pins to AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_4);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_4);
    
    /* Configure CAN RX and TX pins */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
     //CAN中断使能和优先级设置
    NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;         
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
        
    //CAN单元设置
    CAN_InitStructure.CAN_TTCM = DISABLE; 
    CAN_InitStructure.CAN_ABOM = DISABLE;  
    CAN_InitStructure.CAN_AWUM = DISABLE; 
    CAN_InitStructure.CAN_NART = ENABLE; 
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//CAN_Mode_Normal; 
    //CAN波特率设置,500KHz@48MHz
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
    CAN_InitStructure.CAN_Prescaler = 160;
    CAN_Init(CAN, &CAN_InitStructure);
    
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    //使能CAN中断
    CAN_ClearITPendingBit(CAN, CAN_IT_FF0);
    CAN_ITConfig(CAN, CAN_IT_TME, DISABLE);
    CAN_ITConfig(CAN, CAN_IT_FMP0, ENABLE);
    CAN_ITConfig(CAN, CAN_IT_FF0, ENABLE);
}

void bxcan_send(CanTxMsg send_msg) {
    send_msg.RTR=CAN_RTR_DATA;
    send_msg.IDE=CAN_ID_STD;
    uint8_t TransmitMailbox = CAN_Transmit(CAN, &send_msg);
    uint32_t timeout = 0xFFFF;
    while((CAN_TransmitStatus(CAN, TransmitMailbox) != CANTXOK) && (timeout != 0xFFFFFF)) {
        timeout++;
    }
}

void bxcan_set_id(uint8_t id) {
    can_id = id;
}

void bxcan_set_br(uint8_t br) {
    CAN_InitTypeDef CAN_InitStructure;
    //CAN单元设置
    CAN_InitStructure.CAN_TTCM = DISABLE; 
    CAN_InitStructure.CAN_ABOM = DISABLE;  
    CAN_InitStructure.CAN_AWUM = DISABLE; 
    CAN_InitStructure.CAN_NART = ENABLE; 
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//CAN_Mode_Normal; 
    //CAN波特率设置,500KHz@48MHz
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
     switch(br) {
        case 4:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_7tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
            CAN_InitStructure.CAN_Prescaler = 12;
        break;
        case 3:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq;
            CAN_InitStructure.CAN_Prescaler = 96;
        break;
        case 2:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
            CAN_InitStructure.CAN_Prescaler = 160;
        break;
        case 1:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
            CAN_InitStructure.CAN_Prescaler = 240;
        break;
        case 0:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
            CAN_InitStructure.CAN_Prescaler = 600;
        break;
    }
    CAN_Init(CAN, &CAN_InitStructure);
}

uint8_t bxcan_get_id(void) {
    return can_id;
}

void CEC_CAN_IRQHandler (void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;//中断中唤醒新任务
    
    if (CAN_GetITStatus(CAN,CAN_IT_FMP0)) {
        CAN_ClearITPendingBit(CAN, CAN_IT_FMP0);
        CanRxMsg msg_buf;
        CAN_Receive(CAN, CAN_FIFO0, &msg_buf);
        if(task_can_get_queue() != NULL) {
            xQueueSendFromISR( task_can_get_queue(), &msg_buf, 0 );
        }
    }
    
    if( xHigherPriorityTaskWoken != pdFALSE ) {
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );//强制上下文切换
    }
}



