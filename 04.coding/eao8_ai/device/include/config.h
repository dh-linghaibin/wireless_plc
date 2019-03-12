/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"

void config_init(void);
uint16_t config_read_address(void);
uint8_t config_read_baudrate(void);

#ifdef __cplusplus
}
#endif

#endif
