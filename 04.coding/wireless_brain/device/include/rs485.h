/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _RS485_H_
#define _RS485_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"

void rs485_init(void);
void rs485_baud_set(uint16_t baud);
void rs485_send(uint8_t * buf, uint8_t len);

void rs485_rec_set(uint8_t rshead, uint8_t rslen);
uint8_t rs485_rec_flag(void);
void rs485_rec_flag_set(uint8_t cmd);
uint8_t * rs485_rec_get(void);
uint8_t rs485_rec_len(void);

#ifdef __cplusplus
}
#endif

#endif


