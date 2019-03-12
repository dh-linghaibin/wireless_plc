/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "can.h"
#include <stdio.h>

u8 can_init(uint16_t baud_rate,u8 mode) {
    GPIO_InitTypeDef GPIO_InitStructure; 
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    //ʹ�����ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PORTAʱ��
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//ʹ��CAN1ʱ��    

    //��ʼ��GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PA11,PA12

    //���Ÿ���ӳ������
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_CAN2); //GPIOA11����ΪCAN1
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_CAN2); //GPIOA12����ΪCAN1

    //CAN��Ԫ����
    CAN_InitStructure.CAN_TTCM=DISABLE;    //��ʱ�䴥��ͨ��ģʽ   
    CAN_InitStructure.CAN_ABOM=DISABLE;    //����Զ����߹���      
    CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN_InitStructure.CAN_NART=ENABLE;    //��ֹ�����Զ����� 
    CAN_InitStructure.CAN_RFLM=DISABLE;    //���Ĳ�����,�µĸ��Ǿɵ�  
    CAN_InitStructure.CAN_TXFP=DISABLE;    //���ȼ��ɱ��ı�ʶ������ 
    CAN_InitStructure.CAN_Mode= mode;     //ģʽ���� 
    CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;    //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
    CAN_InitStructure.CAN_BS1=CAN_BS1_16tq; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
    CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;  //Tbs2��ΧCAN_BS2_1tq ~    CAN_BS2_8tq
    CAN_InitStructure.CAN_Prescaler=baud_rate;  //��Ƶϵ��(Fdiv)Ϊbrp+1    
    CAN_Init(CAN2, &CAN_InitStructure);   // ��ʼ��CAN1 

    //���ù�����
    CAN_FilterInitStructure.CAN_FilterNumber=14;      //������0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
    CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��

    CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.            

    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;     // �����ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    return 0;
}   
 
void CAN2_RX0_IRQHandler(void) {
    CanRxMsg RxMessage;
    CAN_Receive(CAN2, 0, &RxMessage);
    if(RxMessage.StdId == 0x0154) {
        get_data(RxMessage.Data);
    }
}

u8 can_send_msg( CanTxMsg TxMessage) {
    TxMessage.RTR=CAN_RTR_DATA;
    TxMessage.IDE=CAN_ID_STD;
    u8 mbox;
    int i = 0;
    mbox= CAN_Transmit(CAN2, &TxMessage);   
    while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;    //�ȴ����ͽ���
    if(i>=0XFFF)return 1;
    return 0;

}
