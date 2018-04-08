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

#include "app_configure.h"

//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE    1//0,不使能;1,使能.

u8 can1_init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
 
u8 can1_send_msg(u8* msg,u8 len);//发送数据

u8 can1_receive_msg(u8 *buf);//接收数据

#ifdef __cplusplus
}
#endif

#endif


