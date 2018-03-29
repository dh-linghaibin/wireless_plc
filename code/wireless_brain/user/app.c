/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "app_configure.h"
#include "usart.h"
#include "delay.h"
#include "sram.h"
#include "spi.h"
#include "tft.h"
#include "can.h"
#include "xpt2046.h"
#include "w25qxx.h"
#include "ff.h"
#include "malloc.h"
#include "timer.h"
#include "stdio.h"
//lvgl
#include "../lv_hal/lv_hal_disp.h"
#include "../lv_misc/lv_mem.h"
#include "../lv_core/lv_obj.h"
#include "../lvgl.h"
//LWIP
#include "lan8720.h"

//lua
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "string.h"
//freertos
#include "FreeRTOS.h"

#include "gui_demo.h"

const char logo[] = "\
/*\n\
 * This file is part of the \n\
 *\n\
 * Copyright (c) 2017-2018 linghaibin\n\
 *\n\
 */\n\
 Welcome to use device!\n";

int main(void) {
    delay_init(168);
    uart_init(115200);
    printf("%s",logo);
    
    fsmc_sram_init();//��ʼ���ⲿSRAM
    my_mem_init(SRAMIN);//��ʼ���ڲ��ڴ��
    my_mem_init(SRAMEX);//��ʼ���ⲿ�ڴ��
    my_mem_init(SRAMCCM);//��ʼ��CCM�ڴ��
   
    eth_init();
    tcp_server_init();
    vTaskStartScheduler();
    while(1) {
    
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
