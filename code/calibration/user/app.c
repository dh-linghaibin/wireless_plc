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
//标准库需要的支持函数                 
struct __FILE 
{ 
    int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
    x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 
    return ch;
}
#endif


int main(void) {
    delay_init(168);
    lcd_init();
    //lcd_clolor_box(0,0,320,240,White);
    lcd_clolor_box(0,0,110,46,alpha_blend(Green,Black,40));
    lcd_string(F_7X8B,5,5,"Val 24.000V",Black,alpha_blend(Green,Black,40));
    lcd_string(F_7X8B,5,23,"Cul 0.0000A",Black,alpha_blend(Green,Black,40));
    lcd_string(F_7X8B,200,20,"ing...  5%",White,Black);
    
    simulation_init();
    ec11_init();
    tlc7226_init();
    tlc7226_set(1,0,250);
    tlc7226_set(1,1,250);
    tlc7226_set(1,2,250);
    tlc7226_set(1,3,250);
    
    can1_init(BAUD_RATE_250K,CAN_Mode_Normal); 
    while(1) {
        delay_ms(10);
        static uint8_t flag = 0;
        if(ec11_get() == E_BUTTON) {
            if(flag == 0) {
                flag = 1;
                static uint8_t dr = 0;
                dr = (0 == dr) ? 1 : 0;
                GPIO_WriteBit(GPIOB,GPIO_Pin_4,dr);
                CanTxMsg tx_msg;
                tx_msg.StdId = 0x0155;
                tx_msg.DLC = 8;
                tx_msg.Data[0] = 0xF0;
                tx_msg.Data[1] = 8;
                tx_msg.Data[2] = 1;
                can1_send_msg(tx_msg);
                delay_ms(100);
            }
            delay_ms(100);
        } else {
            flag = 0;
            //GPIO_WriteBit(GPIOB,GPIO_Pin_4,0);
        }
    }
}

