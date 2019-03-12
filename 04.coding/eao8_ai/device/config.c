/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#include "config.h"

void config_init(void) {
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOD, GPIO_MODE_IPU,GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    gpio_init(GPIOD, GPIO_MODE_IPU,GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    gpio_init(GPIOD, GPIO_MODE_IPU,GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_init(GPIOD, GPIO_MODE_IPU,GPIO_OSPEED_50MHZ, GPIO_PIN_0);
    gpio_init(GPIOC, GPIO_MODE_IPU,GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    gpio_init(GPIOC, GPIO_MODE_IPU,GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_init(GPIOC, GPIO_MODE_IPU,GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    gpio_init(GPIOA, GPIO_MODE_IPU,GPIO_OSPEED_50MHZ, GPIO_PIN_15);
}

uint16_t config_read_address(void) {
    uint16_t adr = 0x00;
    if(gpio_input_bit_get(GPIOD,GPIO_PIN_3)) {
        adr |= 0x0001;
    }
    if(gpio_input_bit_get(GPIOD,GPIO_PIN_2)) {
        adr |= 0x0002;
    }
    if(gpio_input_bit_get(GPIOD,GPIO_PIN_1)) {
        adr |= 0x0004;
    }
    if(gpio_input_bit_get(GPIOD,GPIO_PIN_0)) {
        adr |= 0x0008;
    }
    if(gpio_input_bit_get(GPIOC,GPIO_PIN_12)) {
        adr |= 0x0010;
    }
    return adr;
}

uint8_t config_read_baudrate(void) {
    uint8_t baudrate = 0x00;
    if(gpio_input_bit_get(GPIOC,GPIO_PIN_11)) {
        baudrate |= 0x0001;
    }
    if(gpio_input_bit_get(GPIOC,GPIO_PIN_10)) {
        baudrate |= 0x0002;
    }
    if(gpio_input_bit_get(GPIOA,GPIO_PIN_15)) {
        baudrate |= 0x0004;
    }
    return baudrate;
}
