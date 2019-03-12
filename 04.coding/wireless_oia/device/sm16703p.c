/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "sm16703p.h"
#include "delay.h"

#define DATA_H gpio_bit_reset(GPIOA,GPIO_PIN_2)
#define DATA_L gpio_bit_set(GPIOA,GPIO_PIN_2)

#define TIME_0_H delay_us
#define TIME_0_L

#define TIME_1_H
#define TIME_1_L

void lsdelay_us(uint32_t dly)
{
	uint8_t i;
	while(dly--) {
		for(i=0;i<5;i++);
	}
}

void sm16703p_init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
//    while(1) {
//        DATA_H;
//        //lsdelay_us(10000);
//        DATA_L;
//        //lsdelay_us(10000);
//    }
}
