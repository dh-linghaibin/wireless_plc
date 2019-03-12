/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _TM1650_H_
#define _TM1650_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"

typedef enum _key_code {
	K_RESET = 0x4F,
	K_SET = 0X47,
	K_ADD = 0X57,
	K_SUB = 0X5F,
} key_code;

typedef enum _led_code {
	L_DI1 = 1,
	L_DI2 = 6,
	L_DI3 = 5,
	L_DI4 = 4,
	L_DO1 = 3,
	L_DO2 = 2,
	L_RUN = 0,
	L_ERROR = 7,
} led_code;

typedef enum _led_val {
	LV_OPEN = 1,
	LV_CLOSE = 0,
} led_val;

void tm1650_init(void);
void tm1650_set_nex(uint8_t num,uint8_t cmd);
uint8_t tm1650_read_key(void);
void tm1650_set_led(led_code code, led_val val);

#ifdef __cplusplus
}
#endif

#endif


