/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
 
#include "sign_led.h"


void sign_led_init(void) {
    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_init(GPIOD, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_5);
    gpio_init(GPIOD, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_4);
    sign_led_set(SN_RUN,SV_OFF);
    sign_led_set(SN_ERROR,SV_OFF);
}

void sign_led_toggle(sign_led_name name) {
    switch(name) {
        case SN_RUN: {
            gpio_bit_write(GPIOD,GPIO_PIN_4,(bit_status)( 1 - gpio_output_bit_get(GPIOD,GPIO_PIN_4) ));
        } break;
        case SN_ERROR: {
            gpio_bit_write(GPIOD,GPIO_PIN_5,(bit_status)( 1 - gpio_output_bit_get(GPIOD,GPIO_PIN_5) ) );
        } break;
    }
}

void sign_led_set(sign_led_name name,sign_led_avl val) {
    switch(name) {
        case SN_RUN: {
            gpio_bit_write(GPIOD,GPIO_PIN_4,(bit_status)(val));
        } break;
        case SN_ERROR: {
            gpio_bit_write(GPIOD,GPIO_PIN_5,(bit_status)( val ) );
        } break;
    }
}

