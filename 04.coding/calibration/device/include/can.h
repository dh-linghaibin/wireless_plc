/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _CAN_H_
#define _CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h" 

#define BAUD_RATE_1000K    ((uint16_t)2)
#define BAUD_RATE_500K     ((uint16_t)4)
#define BAUD_RATE_250K     ((uint16_t)8)
#define BAUD_RATE_125K     ((uint16_t)16)
#define BAUD_RATE_100K     ((uint16_t)20)
#define BAUD_RATE_50K      ((uint16_t)40)
#define BAUD_RATE_20K      ((uint16_t)100)
#define BAUD_RATE_10K      ((uint16_t)200)
    
//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE    1//0,不使能;1,使能.

u8 can1_init(uint16_t baud_rate,u8 mode);//CAN初始化
 
u8 can1_send_msg(CanTxMsg TxMessage);//发送数据

u8 can1_receive_msg(u8 *buf);//接收数据

#ifdef __cplusplus
}
#endif

#endif


