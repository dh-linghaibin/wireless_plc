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

#define BASED_ADDRESS 25100

typedef enum {
    PA_IP = BASED_ADDRESS+20,
    PA_MASK = BASED_ADDRESS+24,
} per_adr;

void persistence_init(void);
void persistence_set_ip(uint8_t * ip);
void persistence_get_ip(uint8_t * ip);
void persistence_get_mask(uint8_t * mask);
void persistence_set_mask(uint8_t * mask);
void persistence_get_password(uint8_t * pas);
void persistence_set_password(uint8_t * pas);

#ifdef __cplusplus
}
#endif

#endif
