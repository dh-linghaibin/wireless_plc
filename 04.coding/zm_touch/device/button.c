/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "button.h"

#define BUT_ADD_PORT GPIOC
#define BUT_SUB_PORT GPIOC

#define BUT_ADD_PIN GPIO_PIN_15
#define BUT_SUB_PIN GPIO_PIN_14

void button_init(void) {
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOC);
    
    gpio_init(BUT_ADD_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,BUT_ADD_PIN);
    gpio_init(BUT_SUB_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ,BUT_SUB_PIN);
//    while(1) {
//        button_read();
//    }
}

button_code button_read(void) {
    uint8_t add = gpio_input_bit_get(BUT_ADD_PORT,BUT_ADD_PIN);
    uint8_t sub = gpio_input_bit_get(BUT_SUB_PORT,BUT_SUB_PIN);
    
    if( (add) && (sub) ) {
        return B_ALL;
    } else if (add == 0) {
        return B_ADD;
    } else if (sub == 0) {
        return B_SUB;
    } else {
        return B_NULL;
    }
}
