/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _SPI_H_
#define _SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"

void SPI1_Init(void);			 //初始化SPI1口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI1速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节

void SPI3_Init(void);			 //初始化SPI1口
void SPI3_SetSpeed(u8 SpeedSet); //设置SPI1速度   
u8 SPI3_ReadWriteByte(u8 TxData);//SPI1总线读写一个字节
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte);
    
#ifdef __cplusplus
}
#endif

#endif


