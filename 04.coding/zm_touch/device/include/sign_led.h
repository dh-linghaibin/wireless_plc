/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _SIGN_LED_H_
#define _SIGN_LED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"

void sign_led_init(void);
void sign_led_set(uint16_t num);

#ifdef __cplusplus
}
#endif

#endif
