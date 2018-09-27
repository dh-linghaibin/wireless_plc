/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _BUZZER_H_
#define _BUZZER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"

void buzzer_init(void);
void buzzer_set(uint16_t time,uint8_t fre);

#ifdef __cplusplus
}
#endif

#endif


