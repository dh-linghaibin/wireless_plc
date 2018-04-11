/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _TM1650_H
#define _TM1650_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"

#define USB_IIC 1

typedef enum {
    L_COIL1 = 0,
    L_COIL2 = 1,
    L_COIL3 = 2,
    L_COIL4 = 3,
    L_COIL5 = 4,
    L_COIL6 = 5,
    L_COIL7 = 6,
    L_COIL8 = 7,
    L_COIL_ALL = 8,
    L_RUN = 9,
    L_RADIO = 10,
} tm1650_led;

typedef enum {
    K_COIL1 = 0x67,
    K_COIL2 = 0x5f,
    K_COIL3 = 0x57,
    K_COIL4 = 0x6f,
    K_COIL5 = 0x47,
    K_COIL6 = 0x4f,
    K_COIL7 = 0x77,
    K_COIL8 = 0x76,
    K_ADD = 0x66,
    K_SUB = 0x5e,
    K_ALL = 0x56,
} tm1650_key;

void tm1650_init(void);
uint8_t tm1650_read_key(void);
void tm1650_set_nex(uint8_t num,uint8_t cmd);
void tm1650_set_led(tm1650_led num,uint8_t var);
    
#ifdef __cplusplus
}
#endif

#endif
