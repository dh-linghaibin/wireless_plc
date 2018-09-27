/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _OUTSIGNAL_H
#define _OUTSIGNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"

void outsigned_init(void);
void outsignal_set_out(uint8_t num,uint8_t var);
uint8_t outsignal_read_out(uint8_t num);
uint8_t outsignal_emergency_stop(void);

#ifdef __cplusplus
}
#endif

#endif

