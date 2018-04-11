/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#include "outsignal.h"

static uint8_t coil_val = 0;

void outsigned_init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_1);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_3);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_4);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_5);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);

    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,GPIO_PIN_0);
    
    outsignal_set_out(8,0); /* 释放所有继电器 */
}

void outsignal_set_out(uint8_t num,uint8_t var) {
    uint16_t pin;
    switch(num) {
        case 0:
            pin = GPIO_PIN_0;
        break;
        case 1:
            pin = GPIO_PIN_1;
        break;
        case 2:
            pin = GPIO_PIN_2;
        break;
        case 3:
            pin = GPIO_PIN_3;
        break;
        case 4:
            pin = GPIO_PIN_4;
        break;
        case 5:
            pin = GPIO_PIN_5;
        break;
        case 6:
            pin = GPIO_PIN_6;
        break;
        case 7:
            pin = GPIO_PIN_7;
        break;
        case 8: {
            gpio_bit_write(GPIOA,GPIO_PIN_0,(FlagStatus)(var));
            gpio_bit_write(GPIOA,GPIO_PIN_1,(FlagStatus)(var));
            gpio_bit_write(GPIOA,GPIO_PIN_2,(FlagStatus)(var));
            gpio_bit_write(GPIOA,GPIO_PIN_3,(FlagStatus)(var));
            gpio_bit_write(GPIOA,GPIO_PIN_4,(FlagStatus)(var));
            gpio_bit_write(GPIOA,GPIO_PIN_5,(FlagStatus)(var));
            gpio_bit_write(GPIOA,GPIO_PIN_6,(FlagStatus)(var));
            gpio_bit_write(GPIOA,GPIO_PIN_7,(FlagStatus)(var));
            if(var == 0) {
                coil_val = 0x00;
            } else {
                coil_val = 0xff;
            }
            return;
        } break;
        default:{
            return;
        }break;
    }
    if(var == 0) {
        coil_val &= ~(1 << num);
    } else {
        coil_val |= (1 << num);
    }
    gpio_bit_write(GPIOA,pin,(FlagStatus)(var));
}

uint8_t outsignal_read_out(uint8_t num) {
    uint16_t pin;
    switch(num) {
        case 0:
            pin = GPIO_PIN_0;
        break;
        case 1:
            pin = GPIO_PIN_1;
        break;
        case 2:
            pin = GPIO_PIN_2;
        break;
        case 3:
            pin = GPIO_PIN_3;
        break;
        case 4:
            pin = GPIO_PIN_4;
        break;
        case 5:
            pin = GPIO_PIN_5;
        break;
        case 6:
            pin = GPIO_PIN_6;
        break;
        case 7:
            pin = GPIO_PIN_7;
        break;
        case 8: {
            return coil_val;
        } break;
    }
    return gpio_output_bit_get(GPIOA,pin);
}

uint8_t outsignal_emergency_stop(void) {
    return gpio_input_bit_get(GPIOB,GPIO_PIN_0);
}

/***************************************************************END OF FILE****/
