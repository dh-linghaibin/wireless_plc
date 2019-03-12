/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "rs485.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdio.h"
#include "delay.h"

void rs485_init(void) {
      //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); //使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART1时钟

    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA9复用为USART1
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA10复用为USART1

    //USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOA9与GPIOA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA9，PA10
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
    GPIO_Init(GPIOF,&GPIO_InitStructure); //初始化PA9，PA10
    
    GPIO_ResetBits(GPIOF,GPIO_Pin_11);
    
    //USART1 初始化设置
    USART_InitStructure.USART_BaudRate = 9600;//波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式
    USART_Init(USART3, &USART_InitStructure); //初始化串口1

    USART_Cmd(USART3, ENABLE);  //使能串口1 

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化VIC寄存器、
}

void rs485_baud_set(uint16_t baud) {
    USART_InitTypeDef USART_InitStructure;
    
    USART_InitStructure.USART_BaudRate = baud;//波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式
    USART_Init(USART3, &USART_InitStructure); //初始化串口1
}

void rs485_send(uint8_t * buf, uint8_t len) {
    GPIO_SetBits(GPIOF,GPIO_Pin_11);
    for(int i = 0;i < len;i++) {
        while((USART3->SR&0X40)==0) {}
            USART3->DR = (uint8_t) buf[i]; 
    }
    vTaskDelay( 5/portTICK_RATE_MS );  
    GPIO_ResetBits(GPIOF,GPIO_Pin_11);
}

static uint8_t head = 0x87;
static uint8_t len = 0;
static uint8_t rec_ok = 0;
static uint8_t y_len = 0;
static uint8_t rec_buf[30];

void rs485_rec_set(uint8_t rshead, uint8_t rslen) {
    head = rshead;
    len  = rslen;
}

uint8_t rs485_rec_flag(void) {
    return rec_ok;
}

uint8_t rs485_rec_len(void) {
    return len;
}

void rs485_rec_flag_set(uint8_t cmd) {
    y_len = 0;
    rec_ok = cmd;
}

uint8_t * rs485_rec_get(void) {
    return rec_buf;
}

void USART3_IRQHandler(void) {
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) { //接收中断(接收到的数据必须是0x0d 0x0a结尾)
        uint8_t dat = USART_ReceiveData(USART3);//读取接收到的数据
        if(rec_ok == 0) {
            if(y_len == 0) {
                if(dat == head) {
                    rec_buf[y_len] = dat;
                    y_len++;
                }
            } else {
                rec_buf[y_len] = dat;
                y_len++;
                if(y_len == len) {
                    y_len = 0;
                    rec_ok = 1; /* 传输完成 */
                }
            }
        }
    }
}
