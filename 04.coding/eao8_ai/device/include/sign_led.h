/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _SIGN_LED_H_
#define _SIGN_LED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"

typedef enum {
    SN_RUN = 0,
    SN_ERROR = 1,
} sign_led_name;

typedef enum {
    SV_ON = 0,
    SV_OFF = 1,
} sign_led_avl;

void sign_led_init(void);
void sign_led_toggle(sign_led_name name);
void sign_led_set(sign_led_name name,sign_led_avl val);
    
#ifdef __cplusplus
}
#endif

#endif


