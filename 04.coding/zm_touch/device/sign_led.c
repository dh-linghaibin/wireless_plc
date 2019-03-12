/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
#include "sign_led.h"

void sign_led_init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);
    gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);
    
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_1);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);
    
    gpio_bit_write(GPIOA,GPIO_PIN_0,SET);
    gpio_bit_write(GPIOA,GPIO_PIN_1,SET);
    gpio_bit_write(GPIOA,GPIO_PIN_2,SET);
    
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_12);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_4);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_13);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_13);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_15);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_14);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_15);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_8);
    
    gpio_bit_write(GPIOB,GPIO_PIN_12,SET);
    gpio_bit_write(GPIOB,GPIO_PIN_4,SET);
    gpio_bit_write(GPIOB,GPIO_PIN_3,SET);
    gpio_bit_write(GPIOB,GPIO_PIN_13,SET);
    gpio_bit_write(GPIOC,GPIO_PIN_13,SET);
    gpio_bit_write(GPIOA,GPIO_PIN_15,SET);
    gpio_bit_write(GPIOB,GPIO_PIN_14,SET);
    gpio_bit_write(GPIOB,GPIO_PIN_15,SET);
    gpio_bit_write(GPIOA,GPIO_PIN_8,SET);
}

void sign_led_set(uint16_t num) {
    switch(num) {
        case 0x01: {
            gpio_bit_write(GPIOB,GPIO_PIN_12,RESET);
            gpio_bit_write(GPIOB,GPIO_PIN_4,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_3,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOC,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_14,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_8,SET);
        } break;
        case 0x02: {
            gpio_bit_write(GPIOB,GPIO_PIN_12,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_4,RESET);
            gpio_bit_write(GPIOB,GPIO_PIN_3,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOC,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_14,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_8,SET);
        } break;
        case 0x04: {
            gpio_bit_write(GPIOB,GPIO_PIN_12,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_4,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_3,RESET);
            gpio_bit_write(GPIOB,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOC,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_14,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_8,SET);
        } break;
        case 0x08: {
            gpio_bit_write(GPIOB,GPIO_PIN_12,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_4,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_3,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_13,RESET);
            gpio_bit_write(GPIOC,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_14,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_8,SET);
        } break;
        case 0x10: {
            gpio_bit_write(GPIOB,GPIO_PIN_12,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_4,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_3,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOC,GPIO_PIN_13,RESET);
            gpio_bit_write(GPIOA,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_14,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_8,SET);
        } break;
        case 0x20: {
            gpio_bit_write(GPIOB,GPIO_PIN_12,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_4,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_3,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOC,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_15,RESET);
            gpio_bit_write(GPIOB,GPIO_PIN_14,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_8,SET);
        } break;
        case 0x40: {
            gpio_bit_write(GPIOB,GPIO_PIN_12,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_4,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_3,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOC,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_14,RESET);
            gpio_bit_write(GPIOB,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_8,SET);
        } break;
        case 0x80: {
            gpio_bit_write(GPIOB,GPIO_PIN_12,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_4,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_3,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOC,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_14,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_15,RESET);
            gpio_bit_write(GPIOA,GPIO_PIN_8,SET);
        } break;
        case 0x100: {
            gpio_bit_write(GPIOB,GPIO_PIN_12,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_4,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_3,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOC,GPIO_PIN_13,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_14,SET);
            gpio_bit_write(GPIOB,GPIO_PIN_15,SET);
            gpio_bit_write(GPIOA,GPIO_PIN_8,RESET);
        } break;
    }
}

