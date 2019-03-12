/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef __SRAM_H
#define __SRAM_H

#include "stm32f4xx.h" 

#ifndef NULL
#define NULL 0
#endif

void * malloc(u32 size);
void free(void * ptr);
void * realloc(void *ptr,u32 size);

#endif
