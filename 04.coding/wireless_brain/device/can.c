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
    //使能相关时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PORTA时钟                                                                    

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟    

    //初始化GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化PA11,PA12

    //引脚复用映射配置
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1); //GPIOA11复用为CAN1
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1); //GPIOA12复用为CAN1

    //CAN单元设置
    CAN_InitStructure.CAN_TTCM=DISABLE;    //非时间触发通信模式   
    CAN_InitStructure.CAN_ABOM=DISABLE;    //软件自动离线管理      
    CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN_InitStructure.CAN_NART=ENABLE;    //禁止报文自动传送 
    CAN_InitStructure.CAN_RFLM=DISABLE;    //报文不锁定,新的覆盖旧的  
    CAN_InitStructure.CAN_TXFP=DISABLE;    //优先级由报文标识符决定 
    CAN_InitStructure.CAN_Mode= mode;     //模式设置 
    CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;    //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
    CAN_InitStructure.CAN_BS1=CAN_BS1_16tq; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
    CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;  //Tbs2范围CAN_BS2_1tq ~    CAN_BS2_8tq
    CAN_InitStructure.CAN_Prescaler=baud_rate;  //分频系数(Fdiv)为brp+1    
    CAN_Init(CAN1, &CAN_InitStructure);   // 初始化CAN1 

    //配置过滤器
    CAN_FilterInitStructure.CAN_FilterNumber=0;      //过滤器0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
    CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化

    #if CAN1_RX0_INT_ENABLE

    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.            

    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;     // 主优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    #endif
    return 0;
}   
 
#if CAN1_RX0_INT_ENABLE    //使能RX0中断
//中断服务函数                
void CAN1_RX0_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;//中断中唤醒新任务
    //taskENTER_CRITICAL();//进入中断
    CanRxMsg RxMessage;
    CAN_Receive(CAN1, 0, &RxMessage);
    if(task_can_get_queue() != NULL) {
        xQueueSendFromISR( task_can_get_queue(), &RxMessage, 0 );
    }
   // taskEXIT_CRITICAL();
    if( xHigherPriorityTaskWoken != pdFALSE ) {
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );//强制上下文切换
    }
}
#endif

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)    
//len:数据长度(最大为8)                     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//         其他,失败;
u8 can1_send_msg( CanTxMsg TxMessage) {
    u8 mbox;
    int i = 0;
    mbox= CAN_Transmit(CAN1, &TxMessage);   
    while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;    //等待发送结束
    if(i>=0XFFF)return 1;
    return 0;

}
//can口接收数据查询
//buf:数据缓存区;     
//返回值:0,无数据被收到;
//         其他,接收的数据长度;
u8 can1_receive_msg(u8 *buf) {
    u32 i;
    CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;        //没有接收到数据,直接退出 
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据    
    for(i=0;i<RxMessage.DLC;i++)
        buf[i]=RxMessage.Data[i];
    return RxMessage.DLC;
}
