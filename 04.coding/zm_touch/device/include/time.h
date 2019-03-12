/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _TIME_H_
#define _TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"

typedef struct _time_obj{   
    void  	(*init)(struct _time_obj* time);
	uint8_t (*get_1ms)(struct _time_obj* time);
	void  	(*set_1ms)(struct _time_obj* time,uint8_t var);
} time_obj; 

void time_init(struct _time_obj* time);
uint8_t time_get_1ms(struct _time_obj* time);
void time_set_1ms(struct _time_obj* time,uint8_t var);

#ifdef __cplusplus
}
#endif

#endif


