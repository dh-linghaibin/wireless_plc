/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _Persistence_H_
#define _Persistence_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

#define BASED_ADDRESS 26100

typedef enum {
    PA_RES = BASED_ADDRESS+1, //1
    PA_PRO_FLAG = BASED_ADDRESS+19, //1
    PA_IP = BASED_ADDRESS+20,  //4
    PA_MASK = BASED_ADDRESS+24, //4
    PA_LOCK = BASED_ADDRESS+28, //1
    PA_PAS = BASED_ADDRESS+29, //6
} per_adr;

void persistence_init(void);
void persistence_set_res(uint8_t * flag);
void persistence_get_res(uint8_t * flag);
void persistence_set_pro_flag(uint8_t * flag);
void persistence_get_pro_flag(uint8_t * flag);
void persistence_set_ip(uint8_t * ip);
void persistence_get_ip(uint8_t * ip);
void persistence_get_mask(uint8_t * mask);
void persistence_set_mask(uint8_t * mask);
void persistence_get_password(uint8_t * pas);
void persistence_set_password(uint8_t * pas);
void persistence_get_lock(uint8_t * pas);
void persistence_set_lock(uint8_t * pas);

#ifdef __cplusplus
}
#endif

#endif
