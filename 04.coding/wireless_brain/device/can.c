/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "can.h"
#include <stdio.h>
#include "task_can.h"

u8 can1_init(uint16_t baud_rate,u8 mode) {
    GPIO_InitTypeDef GPIO_InitStructure; 
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    #if CAN1_RX0_INT_ENABLE 
    NVIC_InitTypeDef  NVIC_InitStructure;
    #endif
    //ʹ�����ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PORTAʱ��                                                                    

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��    

    //��ʼ��GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PA11,PA12

    //���Ÿ���ӳ������
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1); //GPIOA12����ΪCAN1

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
    CAN_Init(CAN1, &CAN_InitStructure);   // ��ʼ��CAN1 

    //���ù�����
    CAN_FilterInitStructure.CAN_FilterNumber=0;      //������0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
    CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��

    #if CAN1_RX0_INT_ENABLE

    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.            

    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;     // �����ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    #endif
    return 0;
}   
 
#if CAN1_RX0_INT_ENABLE    //ʹ��RX0�ж�
//�жϷ�����                
void CAN1_RX0_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;//�ж��л���������
    //taskENTER_CRITICAL();//�����ж�
    CanRxMsg RxMessage;
    CAN_Receive(CAN1, 0, &RxMessage);
    if(task_can_get_queue() != NULL) {
        xQueueSendFromISR( task_can_get_queue(), &RxMessage, 0 );
    }
   // taskEXIT_CRITICAL();
    if( xHigherPriorityTaskWoken != pdFALSE ) {
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );//ǿ���������л�
    }
}
#endif

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)    
//len:���ݳ���(���Ϊ8)                     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//         ����,ʧ��;
u8 can1_send_msg( CanTxMsg TxMessage) {
    u8 mbox;
    int i = 0;
    mbox= CAN_Transmit(CAN1, &TxMessage);   
    while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;    //�ȴ����ͽ���
    if(i>=0XFFF)return 1;
    return 0;

}
//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;     
//����ֵ:0,�����ݱ��յ�;
//         ����,���յ����ݳ���;
u8 can1_receive_msg(u8 *buf) {
    u32 i;
    CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;        //û�н��յ�����,ֱ���˳� 
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����    
    for(i=0;i<RxMessage.DLC;i++)
        buf[i]=RxMessage.Data[i];
    return RxMessage.DLC;
}
