/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 */
 
#ifndef _MULTI_TIMER_H_
#define _MULTI_TIMER_H_

#include "stdint.h"

typedef struct Timer {
    uint32_t timeout;
    uint32_t repeat;
    void (*timeout_cb)(void);
    struct Timer* next;
}Timer;

#undef NULL
    #define NULL ((void *)0)

#ifdef __cplusplus  
extern "C" {  
#endif  

void mtimer_init(struct Timer* handle, void(*timeout_cb)(), uint32_t timeout, uint32_t repeat);
int  mtimer_start(struct Timer* handle);
void mtimer_stop(struct Timer* handle);
void mtimer_ticks(void);
void mtimer_loop(void);

// void timer_again(struct Timer* handle);
// void timer_set_repeat(struct Timer* handle, uint32_t repeat);

#ifdef __cplusplus
} 
#endif

#endif
