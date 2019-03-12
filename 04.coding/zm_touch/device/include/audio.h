/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _AUDIO_H_
#define _AUDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"

void audio_init(void);
void audio_set(uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif
