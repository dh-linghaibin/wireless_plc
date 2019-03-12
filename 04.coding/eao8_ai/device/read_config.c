/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */
 
#include "read_config.h"
#include "delay.h"

#define PAR_PRE_1_PORT GPIOC
#define PAR_PRE_2_PORT GPIOA
#define PAR_PRE_3_PORT GPIOB
#define PAR_PRE_4_PORT GPIOA
#define PAR_PRE_5_PORT GPIOE
#define PAR_PRE_6_PORT GPIOE
#define PAR_PRE_7_PORT GPIOE
#define PAR_PRE_8_PORT GPIOE

#define PAR_PRE_1_PIN GPIO_PIN_2
#define PAR_PRE_2_PIN GPIO_PIN_1
#define PAR_PRE_3_PIN GPIO_PIN_14
#define PAR_PRE_4_PIN GPIO_PIN_6
#define PAR_PRE_5_PIN GPIO_PIN_7
#define PAR_PRE_6_PIN GPIO_PIN_10
#define PAR_PRE_7_PIN GPIO_PIN_12
#define PAR_PRE_8_PIN GPIO_PIN_14

#define SMA_RES_1_PORT GPIOC
#define SMA_RES_2_PORT GPIOA
#define SMA_RES_3_PORT GPIOB
#define SMA_RES_4_PORT GPIOA
#define SMA_RES_5_PORT GPIOE
#define SMA_RES_6_PORT GPIOE
#define SMA_RES_7_PORT GPIOE
#define SMA_RES_8_PORT GPIOE

#define SMA_RES_1_PIN GPIO_PIN_3
#define SMA_RES_2_PIN GPIO_PIN_2
#define SMA_RES_3_PIN GPIO_PIN_13
#define SMA_RES_4_PIN GPIO_PIN_7
#define SMA_RES_5_PIN GPIO_PIN_8
#define SMA_RES_6_PIN GPIO_PIN_9
#define SMA_RES_7_PIN GPIO_PIN_11
#define SMA_RES_8_PIN GPIO_PIN_13

#define PASS_ENABLE_PORT GPIOB
#define PASS_A_PORT GPIOB
#define PASS_B_PORT GPIOB
#define PASS_C_PORT GPIOE

#define PASS_ENABLE_PIN GPIO_PIN_11
#define PASS_A_PIN GPIO_PIN_12
#define PASS_B_PIN GPIO_PIN_10
#define PASS_C_PIN GPIO_PIN_15

void read_config_init(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC); /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOE);
    gpio_init(PAR_PRE_1_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, PAR_PRE_1_PIN);
    gpio_init(PAR_PRE_2_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, PAR_PRE_2_PIN);
    gpio_init(PAR_PRE_3_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, PAR_PRE_3_PIN);
    gpio_init(PAR_PRE_4_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, PAR_PRE_4_PIN);
    gpio_init(PAR_PRE_5_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, PAR_PRE_5_PIN);
    gpio_init(PAR_PRE_6_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, PAR_PRE_6_PIN);
    gpio_init(PAR_PRE_7_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, PAR_PRE_7_PIN);
    gpio_init(PAR_PRE_8_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, PAR_PRE_8_PIN);
    
    gpio_init(SMA_RES_1_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, SMA_RES_1_PIN);
    gpio_init(SMA_RES_2_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, SMA_RES_2_PIN);
    gpio_init(SMA_RES_3_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, SMA_RES_3_PIN);
    gpio_init(SMA_RES_4_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, SMA_RES_4_PIN);
    gpio_init(SMA_RES_5_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, SMA_RES_5_PIN);
    gpio_init(SMA_RES_6_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, SMA_RES_6_PIN);
    gpio_init(SMA_RES_7_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, SMA_RES_7_PIN);
    gpio_init(SMA_RES_8_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, SMA_RES_8_PIN);
    
    gpio_init(PASS_ENABLE_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, PASS_ENABLE_PIN);
    gpio_init(PASS_A_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, PASS_A_PIN);
    gpio_init(PASS_B_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, PASS_B_PIN);
    gpio_init(PASS_C_PORT, GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ, PASS_C_PIN);
    

    gpio_bit_write(PAR_PRE_1_PORT,PAR_PRE_1_PIN,(bit_status)PR_RESET);
    gpio_bit_write(PAR_PRE_2_PORT,PAR_PRE_2_PIN,(bit_status)PR_RESET);
    gpio_bit_write(PAR_PRE_3_PORT,PAR_PRE_3_PIN,(bit_status)PR_RESET);
    gpio_bit_write(PAR_PRE_4_PORT,PAR_PRE_4_PIN,(bit_status)PR_RESET);
    gpio_bit_write(PAR_PRE_5_PORT,PAR_PRE_5_PIN,(bit_status)PR_RESET);
    gpio_bit_write(PAR_PRE_6_PORT,PAR_PRE_6_PIN,(bit_status)PR_RESET);
    gpio_bit_write(PAR_PRE_7_PORT,PAR_PRE_7_PIN,(bit_status)PR_RESET);
    gpio_bit_write(PAR_PRE_8_PORT,PAR_PRE_8_PIN,(bit_status)PR_RESET);

    gpio_bit_write(SMA_RES_1_PORT,SMA_RES_1_PIN,(bit_status)SR_RESET);
    gpio_bit_write(SMA_RES_2_PORT,SMA_RES_2_PIN,(bit_status)SR_RESET);
    gpio_bit_write(SMA_RES_3_PORT,SMA_RES_3_PIN,(bit_status)SR_RESET);
    gpio_bit_write(SMA_RES_4_PORT,SMA_RES_4_PIN,(bit_status)SR_RESET);
    gpio_bit_write(SMA_RES_5_PORT,SMA_RES_5_PIN,(bit_status)SR_RESET);
    gpio_bit_write(SMA_RES_6_PORT,SMA_RES_6_PIN,(bit_status)SR_RESET);
    gpio_bit_write(SMA_RES_7_PORT,SMA_RES_7_PIN,(bit_status)SR_RESET);
    gpio_bit_write(SMA_RES_8_PORT,SMA_RES_8_PIN,(bit_status)SR_RESET);

    gpio_bit_write(PASS_ENABLE_PORT,PASS_ENABLE_PIN,(bit_status)PA_RESET);
}

/*

*/
static void par_pre_set(check_channel passageway,par_reg_val val) {
//    gpio_bit_write(PAR_PRE_1_PORT,PAR_PRE_1_PIN,(bit_status)~val);
//    gpio_bit_write(PAR_PRE_2_PORT,PAR_PRE_2_PIN,(bit_status)~val);
//    gpio_bit_write(PAR_PRE_3_PORT,PAR_PRE_3_PIN,(bit_status)~val);
//    gpio_bit_write(PAR_PRE_4_PORT,PAR_PRE_4_PIN,(bit_status)~val);
//    gpio_bit_write(PAR_PRE_5_PORT,PAR_PRE_5_PIN,(bit_status)~val);
//    gpio_bit_write(PAR_PRE_6_PORT,PAR_PRE_6_PIN,(bit_status)~val);
//    gpio_bit_write(PAR_PRE_7_PORT,PAR_PRE_7_PIN,(bit_status)~val);
//    gpio_bit_write(PAR_PRE_8_PORT,PAR_PRE_8_PIN,(bit_status)~val);
    switch(passageway) {
        case CC_1:{
            gpio_bit_write(PAR_PRE_1_PORT,PAR_PRE_1_PIN,(bit_status)val);
        } break;
        case CC_2:{
            gpio_bit_write(PAR_PRE_2_PORT,PAR_PRE_2_PIN,(bit_status)val);
        } break;
        case CC_3:{
            gpio_bit_write(PAR_PRE_3_PORT,PAR_PRE_3_PIN,(bit_status)val);
        } break;
        case CC_4:{
            gpio_bit_write(PAR_PRE_4_PORT,PAR_PRE_4_PIN,(bit_status)val);
        } break;
        case CC_5:{
            gpio_bit_write(PAR_PRE_5_PORT,PAR_PRE_5_PIN,(bit_status)val);
        } break;
        case CC_6:{
            gpio_bit_write(PAR_PRE_6_PORT,PAR_PRE_6_PIN,(bit_status)val);
        } break;
        case CC_7:{
            gpio_bit_write(PAR_PRE_7_PORT,PAR_PRE_7_PIN,(bit_status)val);
        } break;
        case CC_8:{
            gpio_bit_write(PAR_PRE_8_PORT,PAR_PRE_8_PIN,(bit_status)val);
        } break;
        default:{
        
        } break;
    }
}

/*

*/
static void sma_res_set(check_channel passageway,SMA_RES_val val) {
//    gpio_bit_write(SMA_RES_1_PORT,SMA_RES_1_PIN,(bit_status)~val);
//    gpio_bit_write(SMA_RES_2_PORT,SMA_RES_2_PIN,(bit_status)~val);
//    gpio_bit_write(SMA_RES_3_PORT,SMA_RES_3_PIN,(bit_status)~val);
//    gpio_bit_write(SMA_RES_4_PORT,SMA_RES_4_PIN,(bit_status)~val);
//    gpio_bit_write(SMA_RES_5_PORT,SMA_RES_5_PIN,(bit_status)~val);
//    gpio_bit_write(SMA_RES_6_PORT,SMA_RES_6_PIN,(bit_status)~val);
//    gpio_bit_write(SMA_RES_7_PORT,SMA_RES_7_PIN,(bit_status)~val);
//    gpio_bit_write(SMA_RES_8_PORT,SMA_RES_8_PIN,(bit_status)~val);
    switch(passageway) {
        case CC_1:{
            gpio_bit_write(SMA_RES_1_PORT,SMA_RES_1_PIN,(bit_status)val);
        } break;
        case CC_2:{
            gpio_bit_write(SMA_RES_2_PORT,SMA_RES_2_PIN,(bit_status)val);
        } break;
        case CC_3:{
            gpio_bit_write(SMA_RES_3_PORT,SMA_RES_3_PIN,(bit_status)val);
        } break;
        case CC_4:{
            gpio_bit_write(SMA_RES_4_PORT,SMA_RES_4_PIN,(bit_status)val);
        } break;
        case CC_5:{
            gpio_bit_write(SMA_RES_5_PORT,SMA_RES_5_PIN,(bit_status)val);
        } break;
        case CC_6:{
            gpio_bit_write(SMA_RES_6_PORT,SMA_RES_6_PIN,(bit_status)val);
        } break;
        case CC_7:{
            gpio_bit_write(SMA_RES_7_PORT,SMA_RES_7_PIN,(bit_status)val);
        } break;
        case CC_8:{
            gpio_bit_write(SMA_RES_8_PORT,SMA_RES_8_PIN,(bit_status)val);
        } break;
        default:{
        
        } break;
    }
}

/*

*/
void read_config_pass_set(check_channel passageway) {
    gpio_bit_write(PASS_ENABLE_PORT,PASS_ENABLE_PIN,(bit_status)PA_RESET);
    delay_ms(5);
    switch(passageway) {
        case CC_8:{
            gpio_bit_write(PASS_A_PORT,PASS_A_PIN,(bit_status)PA_RESET);
            gpio_bit_write(PASS_B_PORT,PASS_B_PIN,(bit_status)PA_RESET);
            gpio_bit_write(PASS_C_PORT,PASS_C_PIN,(bit_status)PA_RESET);
        } break;
        case CC_7:{
            gpio_bit_write(PASS_A_PORT,PASS_A_PIN,(bit_status)PA_SET);
            gpio_bit_write(PASS_B_PORT,PASS_B_PIN,(bit_status)PA_RESET);
            gpio_bit_write(PASS_C_PORT,PASS_C_PIN,(bit_status)PA_RESET);
        } break;
        case CC_6:{
            gpio_bit_write(PASS_A_PORT,PASS_A_PIN,(bit_status)PA_RESET);
            gpio_bit_write(PASS_B_PORT,PASS_B_PIN,(bit_status)PA_SET);
            gpio_bit_write(PASS_C_PORT,PASS_C_PIN,(bit_status)PA_RESET);
        } break;
        case CC_5:{
            gpio_bit_write(PASS_A_PORT,PASS_A_PIN,(bit_status)PA_SET);
            gpio_bit_write(PASS_B_PORT,PASS_B_PIN,(bit_status)PA_SET);
            gpio_bit_write(PASS_C_PORT,PASS_C_PIN,(bit_status)PA_RESET);
        } break;
        case CC_4:{
            gpio_bit_write(PASS_A_PORT,PASS_A_PIN,(bit_status)PA_RESET);
            gpio_bit_write(PASS_B_PORT,PASS_B_PIN,(bit_status)PA_RESET);
            gpio_bit_write(PASS_C_PORT,PASS_C_PIN,(bit_status)PA_SET);
        } break;
        case CC_3:{
            gpio_bit_write(PASS_A_PORT,PASS_A_PIN,(bit_status)PA_SET);
            gpio_bit_write(PASS_B_PORT,PASS_B_PIN,(bit_status)PA_RESET);
            gpio_bit_write(PASS_C_PORT,PASS_C_PIN,(bit_status)PA_SET);
        } break;
        case CC_2:{
            gpio_bit_write(PASS_A_PORT,PASS_A_PIN,(bit_status)PA_RESET);
            gpio_bit_write(PASS_B_PORT,PASS_B_PIN,(bit_status)PA_SET);
            gpio_bit_write(PASS_C_PORT,PASS_C_PIN,(bit_status)PA_SET);
        } break;
        case CC_1:{
            gpio_bit_write(PASS_A_PORT,PASS_A_PIN,(bit_status)PA_SET);
            gpio_bit_write(PASS_B_PORT,PASS_B_PIN,(bit_status)PA_SET);
            gpio_bit_write(PASS_C_PORT,PASS_C_PIN,(bit_status)PA_SET);
        } break;
        default:{
        
        } break;
    }
    gpio_bit_write(PASS_ENABLE_PORT,PASS_ENABLE_PIN,(bit_status)PA_SET);
}

/*

*/
void read_config_mode(ads_read_mode mode,check_channel passageway) {
    gpio_bit_write(PASS_ENABLE_PORT,PASS_ENABLE_PIN,(bit_status)PA_RESET);
    switch(mode) {
        case PT100: {
            /*  分压 用哪个 哪个置零 */
            par_pre_set(passageway,PR_RESET);
            /* 取样 同上 */
            sma_res_set(passageway,SR_RESET);
            /* 用那个选那个 */
            //pass_set(passageway);
        } break;
        case PT1000: {
            /*  分压 用哪个 哪个置零 */
            par_pre_set(passageway,PR_RESET);
            /* 取样 同上 */
            sma_res_set(passageway,SR_RESET);
            /* 用那个选那个 */
            //pass_set(passageway);
        } break;
//        case NTC10K: {
//            /*  分压 用哪个 哪个置零 */
//            par_pre_set(passageway,PR_RESET);
//            /* 取样 同上 */
//            sma_res_set(passageway,SR_RESET);
//            /* 用那个选那个 */
//            pass_set(passageway);
//        } break;
        case VOLTAGE:{
             /*  分压 用哪个 哪个置1 */
            par_pre_set(passageway,PR_SET);
            /*  取样 用哪个 哪个置零 */
            sma_res_set(passageway,SR_RESET);
            /* 无所谓 */
            /* 模拟开关 0 */
            //pass_set(CC_NULL);
        } break;
        case ELE_CURRENT: {
            /*  分压 用哪个 哪个置1 */
            par_pre_set(passageway,PR_SET);
            /*  取样 用哪个 哪个置1 */
            sma_res_set(passageway,SR_SET);
            /* 无所谓 */
            /* 模拟开关 0 */
            //pass_set(CC_NULL);
        } break;
        default:{
            par_pre_set(CC_NULL,PR_SET);
            sma_res_set(CC_NULL,SR_SET);
        } break;
    }
}

