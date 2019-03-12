/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "app_configure.h"
#include <stdbool.h>
#include "analog_quantity.h"
#include "read_config.h"
#include "delay.h"
#include "sign_led.h"
#include "config.h"
#include "can.h"

/* 测试使用 */
float res[8];
uint16_t adr = 0;
uint8_t bu = 0;

int main(void) {
    delay_init();
    delay_ms(200);
    sign_led_init();
    config_init();

    read_config_init();
    analog_quantity_init();
    bxcan_init(4);
    
    
    //配置八个通道模式
    analog_quantity_set_ch_mode(PT1000,CC_1);
    analog_quantity_set_ch_mode(PT1000,CC_2);
    analog_quantity_set_ch_mode(ELE_CURRENT,CC_3);
    analog_quantity_set_ch_mode(ELE_CURRENT,CC_4);
    analog_quantity_set_ch_mode(ELE_CURRENT,CC_5);
    analog_quantity_set_ch_mode(ELE_CURRENT,CC_6);
    analog_quantity_set_ch_mode(ELE_CURRENT,CC_7);
    analog_quantity_set_ch_mode(ELE_CURRENT,CC_8);

    while(1) {
        /* 读取八个通道值 配置什么模式 读出来的就是什么值 */
        
        if(analog_quantity_calibration_flag_get() == 1) {
            //正在标定 不允许读值 can发送
        } else {
            //普通状态
                    static uint8_t count = 0;
            static uint8_t count2 = 0;
            if(count < 20) {
                count++;
            } else {
                count = 0;
                analog_quantity_tic_100ms();
            }

            if(count2 < 10) {
                count2++;
            } else {
                count2 = 0;
                for(int i = 0;i < 8;i++) { 
                    res[i] = analog_quantity_get(i);
                }
            }

            //        adr = config_read_address();
            //        bu = config_read_baudrate();
            sign_led_toggle(SN_RUN);
            sign_led_toggle(SN_ERROR);
            
            can_trasnmit_message_struct send_msg;
            send_msg.tx_sfid = 0x0154;
            send_msg.tx_dlen = 4;
            send_msg.tx_data[0] = 2; /* 设备类型 */
            send_msg.tx_data[1] = 2; /* 命令 */
            bxcan_send(send_msg);
        }
        

        delay_ms(3);
    }
}
