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

//lwip���ƽṹ��
typedef struct  
{
	u8 mac[6];      //MAC��ַ
	u8 remoteip[4];	//Զ������IP��ַ 
	u8 ip[4];       //����IP��ַ
	u8 netmask[4]; 	//��������
	u8 gateway[4]; 	//Ĭ�����ص�IP��ַ
	
	vu8 dhcpstatus;	//dhcp״̬ 
					//0,δ��ȡDHCP��ַ;
					//1,����DHCP��ȡ״̬
					//2,�ɹ���ȡDHCP��ַ
					//0XFF,��ȡʧ��.
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


