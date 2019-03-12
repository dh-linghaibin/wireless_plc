/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "digital_tube.h"

#define PORT_A GPIOA
#define PORT_B GPIOA
#define PORT_C GPIOB
#define PORT_D GPIOA

#define PORT_DG1 GPIOA
#define PORT_DG2 GPIOA
#define PORT_DG3 GPIOB
#define PORT_DG4 GPIOB

#define PIN_A GPIO_PIN_6
#define PIN_B GPIO_PIN_7
#define PIN_C GPIO_PIN_0
#define PIN_D GPIO_PIN_5

#define PIN_DG1 GPIO_PIN_4
#define PIN_DG2 GPIO_PIN_3
#define PIN_DG3 GPIO_PIN_1
#define PIN_DG4 GPIO_PIN_10

static volatile const uint8_t show_code[13] = {0xbf,0x06,0x6d,0x4f,0xc6,0xcb,0xeb,0x0e,0xef,0xcf,0xe3,0x00,0xff};//数码管编码
static volatile uint8_t show[2];//显示数字
static volatile uint8_t choice_num;//轮询标志
static volatile uint8_t point[2];//小数点
static void set_bit_num(uint8_t val);

void digital_tube_init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

    rcu_periph_clock_enable(RCU_AF);
    gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);
    gpio_pin_remap_config(GPIO_TIMER2_PARTIAL_REMAP,ENABLE);
    
    gpio_init(PORT_A, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_A);
    gpio_init(PORT_B, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_B);
    gpio_init(PORT_C, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_C);
    gpio_init(PORT_D, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_D);

    gpio_init(PORT_DG1, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_DG1);
    gpio_init(PORT_DG2, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_DG2);
    gpio_init(PORT_DG3, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_DG3);
    gpio_init(PORT_DG4, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,PIN_DG4);
    
    set_bit_num(0x00);
    gpio_bit_reset(PORT_DG1,PIN_DG1);
    gpio_bit_reset(PORT_DG2,PIN_DG2);
}

static void set_bit_num(uint8_t val) {
    gpio_bit_write(PORT_A,PIN_A,       \
                  (bit_status)( ( (val) >> 3)&0x01 ));
    gpio_bit_write(PORT_B,PIN_B,       \
                  (bit_status)( ( (val) >> 2)&0x01 ));
    gpio_bit_write(PORT_C,PIN_C,       \
                  (bit_status)( ( (val) >> 1)&0x01 ));
    gpio_bit_write(PORT_D,PIN_D,       \
                  (bit_status)( ( (val) >> 0)&0x01 ));
}

static void motion_dig(uint8_t choice_num) {
    switch(choice_num) {
        case 0:
        gpio_bit_write(PORT_DG1,PIN_DG1,SET);
        gpio_bit_write(PORT_DG2,PIN_DG2,RESET);
        gpio_bit_write(PORT_DG3,PIN_DG3,RESET);
        gpio_bit_write(PORT_DG4,PIN_DG4,RESET);
        break;
        case 1:
        gpio_bit_write(PORT_DG1,PIN_DG1,RESET);
        gpio_bit_write(PORT_DG2,PIN_DG2,SET);
        gpio_bit_write(PORT_DG3,PIN_DG3,RESET);
        gpio_bit_write(PORT_DG4,PIN_DG4,RESET);
        break;
        case 2:
        gpio_bit_write(PORT_DG1,PIN_DG1,RESET);
        gpio_bit_write(PORT_DG2,PIN_DG2,RESET);
        gpio_bit_write(PORT_DG3,PIN_DG3,SET);
        gpio_bit_write(PORT_DG4,PIN_DG4,RESET);
        break;
        case 3:
        gpio_bit_write(PORT_DG1,PIN_DG1,RESET);
        gpio_bit_write(PORT_DG2,PIN_DG2,RESET);
        gpio_bit_write(PORT_DG3,PIN_DG3,RESET);
        gpio_bit_write(PORT_DG4,PIN_DG4,SET);
        break;
    }
}

#define POINT_NO 0xef
#define POINT_OK 0x10

void digital_tube_refresh(void) {
    switch(choice_num) {
        case 0: {
            if(point[0] == 0) {
                set_bit_num( (show_code[show[0]] & POINT_NO) );
            } else {
                set_bit_num( (show_code[show[0]] | POINT_OK) );
            }
        } break;
        case 1: {
            if(point[0] == 0) {
                set_bit_num( (show_code[show[0]] & POINT_NO) >> 4);
            } else {
                set_bit_num( (show_code[show[0]] | POINT_OK) >> 4);
            }
        } break;
        case 2: {
            if(point[1] == 0) {
                set_bit_num( (show_code[show[1]] & POINT_NO) );
            } else {
                set_bit_num( (show_code[show[1]] | POINT_OK) );
            }
        } break;
        case 3: {
            if(point[1] == 0) {
                set_bit_num( (show_code[show[1]] & POINT_NO) >> 4);
            } else {
                set_bit_num( (show_code[show[1]] | POINT_OK) >> 4);
            }
        } break;
    }
    motion_dig(choice_num);
    if(choice_num < 3) {
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
