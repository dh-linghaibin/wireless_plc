/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _RS485_H_
#define _RS485_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"

typedef enum 
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

void rs485_init(uint8_t br);
uint8_t rs485_send(uint8_t * buf, uint8_t len);
uint8_t rs485_get(uint8_t * buf, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif
