/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#include "lcd.h"
#include "delay.h"
#include "tlc7226.h"
#include "ec11.h"
#include "stdio.h"
#include "simulation.h"
#include "button.h"
#include "sram.h"
#include "can.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
    int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
    x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 
    return ch;
}
#endif

static uint8_t bd_state = 0; 

int main(void) {
    delay_init(168);
    lcd_init();

    simulation_init();
    ec11_init();
    tlc7226_init();
    tlc7226_set(1,0,0);//250
    tlc7226_set(1,1,10);
    tlc7226_set(1,2,5);
    tlc7226_set(1,3,30);
    
    can1_init(BAUD_RATE_250K,CAN_Mode_Normal); 
    while(1) {
//        delay_ms(10);
//        static uint8_t flag = 0;
//        if(ec11_get() == E_BUTTON) {
//            if(flag == 0) {
//                flag = 1;
//                static uint8_t dr = 0;
//                dr = (0 == dr) ? 1 : 0;
//                GPIO_WriteBit(GPIOB,GPIO_Pin_4,dr);
//                CanTxMsg tx_msg;
//                tx_msg.StdId = 0x0155;
//                tx_msg.DLC = 8;
//                tx_msg.Data[0] = 0xF0;
//                tx_msg.Data[1] = 8;
//                tx_msg.Data[2] = 1;
//                can1_send_msg(tx_msg);
//                delay_ms(100);
//            }
//            delay_ms(100);
//        } else {
//            flag = 0;
//            //GPIO_WriteBit(GPIOB,GPIO_Pin_4,0);
//        }
    }
}

