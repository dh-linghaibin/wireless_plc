/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _WIRE_LESS_H_
#define _WIRE_LESS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"

void wireless_init(void);
uint8_t spi_send(uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif


