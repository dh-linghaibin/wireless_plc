/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _SIGN_CONFIGURE_H_
#define _SIGN_CONFIGURE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"

typedef enum _sign_name {
    S_DI1,
    S_DI2,
    S_DI3,
    S_DI4,
    S_DO1,
    S_DO2,
    S_AI1,
    S_AI2,
    S_AI3,
    S_AI4,
    S_AO1,
    S_AO2,
} sign_name;

typedef enum _ai_mode {
    AM_VOLTAGE,
    AM_CURRENT,
} ai_mode;

void
sign_configure(void);
void
sign_update(void);
uint16_t
sign_read(sign_name name);

#ifdef __cplusplus
}
#endif

#endif


