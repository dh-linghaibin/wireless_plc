/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _USART_H_
#define _USART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"
#include "stdio.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART6_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

void uart_init(u32 bound);
    
#ifdef __cplusplus
}
#endif

#endif


