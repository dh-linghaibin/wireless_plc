/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "insignal.h"

#define INSI_PORT_1 GPIOA
#define INSI_PORT_2 GPIOA
#define INSI_PORT_3 GPIOA
#define INSI_PORT_4 GPIOA

#define INSI_PIN_1 GPIO_PIN_6
#define INSI_PIN_2 GPIO_PIN_5
#define INSI_PIN_3 GPIO_PIN_4
#define INSI_PIN_4 GPIO_PIN_3

void insignal_init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    
    gpio_init(INSI_PORT_1, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,INSI_PIN_1);
    gpio_init(INSI_PORT_2, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,INSI_PIN_2);
    gpio_init(INSI_PORT_3, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,INSI_PIN_3);
    gpio_init(INSI_PORT_4, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,INSI_PIN_4);
}

uint8_t insignal_read(uint8_t ch) {
    static uint8_t val = 0;
    uint32_t port;
    uint32_t pin;
    switch(ch) {
        case 0: {
            port = INSI_PORT_1;
            pin = INSI_PIN_1;
        } break;
        case 1: {
            port = INSI_PORT_2;
            pin = INSI_PIN_2;
        } break;
        case 2: {
            port = INSI_PORT_3;
            pin = INSI_PIN_3;
        } break;
        case 3: {
            port = INSI_PORT_4;
            pin = INSI_PIN_4;
        } break;
        case 8: {
            val = 0x00;
            port = INSI_PORT_1;
            pin = INSI_PIN_1;
            if( gpio_input_bit_get(port,pin) ) {
                val |= (1 << 0);
            } else {
                val &= ~(1 << 0);
            }
            port = INSI_PORT_2;
            pin = INSI_PIN_2;
            if( gpio_input_bit_get(port,pin) ) {
                val |= (1 << 1);
            } else {
                val &= ~(1 << 1);
            }
            port = INSI_PORT_3;
            pin = INSI_PIN_3;
            if( gpio_input_bit_get(port,pin) ) {
                val |= (1 << 2);
            } else {
                val &= ~(1 << 2);
            }
            port = INSI_PORT_4;
            pin = INSI_PIN_4;
            if( gpio_input_bit_get(port,pin) ) {
                val |= (1 << 3);
            } else {
                val &= ~(1 << 3);
            }
            return val;
        } break;
        default: {
            return I_UP;
        } break;
    }
    if( gpio_input_bit_get(port,pin) ) {
        val |= (1 << ch);
        return I_DOWN;
    } else {
        val &= ~(1 << ch);
        return I_UP;
    }
}

