/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _TOUCH_H_
#define _TOUCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"

#define USB_IIC 1

typedef enum {
    KEY_NULL = 0,
    KEY1,
    KEY2,
    KEY3,
    KEY4,
    KEY5,
    KEY6,
    KEY7,
    KEY8,
    KEY9,
} touch_key_e;

void touch_init(void);
touch_key_e touch_read(void);
uint16_t touch_get_val(void);
void touch_get_val_set(uint16_t val);

#ifdef __cplusplus
}
#endif

#endif
