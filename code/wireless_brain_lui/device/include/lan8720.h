/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _LAN8720_H_
#define _LAN8720_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"
#include "stm32f4x7_eth.h"

//lwip控制结构体
typedef struct  
{
	u8 mac[6];      //MAC地址
	u8 remoteip[4];	//远端主机IP地址 
	u8 ip[4];       //本机IP地址
	u8 netmask[4]; 	//子网掩码
	u8 gateway[4]; 	//默认网关的IP地址
	
	vu8 dhcpstatus;	//dhcp状态 
					//0,未获取DHCP地址;
					//1,进入DHCP获取状态
					//2,成功获取DHCP地址
					//0XFF,获取失败.
}__lwip_dev;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define LAN8720_PHY_ADDRESS 0x01 /* Relative to STM324xG-EVAL Board */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void  eth_init(void);
    
#ifdef __cplusplus
}
#endif

#endif


