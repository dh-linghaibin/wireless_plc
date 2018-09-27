/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "digital_tube.h"

#define PORT_A GPIOB
#define PORT_B GPIOB
#define PORT_C GPIOB
#define PORT_D GPIOA
#define PORT_E GPIOA
#define PORT_F GPIOC
#define PORT_G GPIOB
#define PORT_DP GPIOB
#define PORT_DG1 GPIOC
#define PORT_DG2 GPIOA

#define PIN_A GPIO_PIN_5
#define PIN_B GPIO_PIN_8
#define PIN_C GPIO_PIN_6
#define PIN_D GPIO_PIN_9
#define PIN_E GPIO_PIN_8
#define PIN_F GPIO_PIN_13
#define PIN_G GPIO_PIN_4
#define PIN_DP GPIO_PIN_3
#define PIN_DG1 GPIO_PIN_14
#define PIN_DG2 GPIO_PIN_15

static volatile const uint8_t show_code[13] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x7c,0x00,0xff};//数码管编码
static volatile uint8_t show[2];//显示数字
static volatile uint8_t choice_num;//轮询标志
static volatile uint8_t point[2];//小数点
static void set_bit_num(uint8_t val);

void digital_tube_init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);
    gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);
    gpio_pin_remap_config(GPIO_TIMER2_PARTIAL_REMAP,ENABLE);
    
    gpio_init(PORT_A, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_A);
    gpio_init(PORT_B, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_B);
    gpio_init(PORT_C, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_C);
    gpio_init(PORT_D, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_D);
    gpio_init(PORT_E, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_E);
    gpio_init(PORT_F, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_F);
    gpio_init(PORT_G, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_G);
    gpio_init(PORT_DP, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_DP);
    gpio_init(PORT_DG1, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_DG1);
    gpio_init(PORT_DG2, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_DG2);
    
    set_bit_num(0x00);
    gpio_bit_reset(PORT_DG1,PIN_DG1);
    gpio_bit_reset(PORT_DG2,PIN_DG2);
}

static void set_bit_num(uint8_t val) {
    gpio_bit_write(PORT_A,PIN_A,       \
                  (bit_status)( ( (val) >> 0)&0x01 ));
    gpio_bit_write(PORT_B,PIN_B,       \
                  (bit_status)( ( (val) >> 1)&0x01 ));
    gpio_bit_write(PORT_C,PIN_C,       \
                  (bit_status)( ( (val) >> 2)&0x01 ));
    gpio_bit_write(PORT_D,PIN_D,       \
                  (bit_status)( ( (val) >> 3)&0x01 ));
    gpio_bit_write(PORT_E,PIN_E,       \
                  (bit_status)( ( (val) >> 4)&0x01 ));
    gpio_bit_write(PORT_F,PIN_F,       \
                  (bit_status)( ( (val) >> 5)&0x01 ));
    gpio_bit_write(PORT_G,PIN_G,       \
                  (bit_status)( ( (val) >> 6)&0x01 ));
    gpio_bit_write(PORT_DP,PIN_DP,       \
                  (bit_status)( ( (val) >> 7)&0x01 ));
}

static void motion_dig(uint8_t choice_num) {
    switch(choice_num) {
        case 0:
        gpio_bit_write(PORT_DG2,PIN_DG2,RESET);
        gpio_bit_write(PORT_DG1,PIN_DG1,SET);
        break;
        case 1:
        gpio_bit_write(PORT_DG1,PIN_DG1,RESET);
        gpio_bit_write(PORT_DG2,PIN_DG2,SET);
        break;
    }
}

void digital_tube_refresh(void) {
    if(point[choice_num] == 0) {
        set_bit_num(show_code[show[choice_num]] & 0x7f);
    } else {
        set_bit_num(show_code[show[choice_num]] | 0x80);
    }
    
    motion_dig(choice_num);
    if(choice_num < 1) {
        choice_num++;
    } else {
        choice_num = 0;
    }
}

void digital_tube_set_num(uint8_t bit,uint8_t num) {
    if(bit > 1) {
        bit  = 1;
    }
    show[bit] = num;
}
void igital_tube_toggle_point(uint8_t point_num,uint8_t val) {
    if(point_num > 1) {
        point_num  = 1;
    }
    if(val == 0) {
        point[point_num] = 0;
    } else {
        point[point_num] = 1;
    }
}
