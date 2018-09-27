/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#include "time.h"

static uint8_t flag_1ms = 0;

void time_init(struct _time_obj* time) {
	/* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 5000U)){
        /* capture error */
        while (1){
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

uint8_t time_get_1ms(struct _time_obj* time) {
	return flag_1ms;
}

void time_set_1ms(struct _time_obj* time,uint8_t var) {
	flag_1ms = var;
}

void SysTick_Handler2(void) {
	flag_1ms++;
}
