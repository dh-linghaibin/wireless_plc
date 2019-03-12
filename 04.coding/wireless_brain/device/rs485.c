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
      //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); //ʹ��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART1ʱ��

    //����1��Ӧ���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOA9����ΪUSART1
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOA10����ΪUSART1

    //USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOA9��GPIOA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //�ٶ�50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
    GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA9��PA10
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ù���
    GPIO_Init(GPIOF,&GPIO_InitStructure); //��ʼ��PA9��PA10
    
    GPIO_ResetBits(GPIOF,GPIO_Pin_11);
    
    //USART1 ��ʼ������
    USART_InitStructure.USART_BaudRate = 9600;//����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //�շ�ģʽ
    USART_Init(USART3, &USART_InitStructure); //��ʼ������1

    USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);    //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

void rs485_baud_set(uint16_t baud) {
    USART_InitTypeDef USART_InitStructure;
    
    USART_InitStructure.USART_BaudRate = baud;//����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //�շ�ģʽ
    USART_Init(USART3, &USART_InitStructure); //��ʼ������1
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
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) { //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
        uint8_t dat = USART_ReceiveData(USART3);//��ȡ���յ�������
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
                    rec_ok = 1; /* ������� */
                }
            }
        }
    }
}
