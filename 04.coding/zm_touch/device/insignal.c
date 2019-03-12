/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "insignal.h"


void insignal_init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
}

uint8_t insignal_read(uint8_t ch) {
    static uint8_t val = 0;
    uint32_t port;
    uint32_t pin;
}

