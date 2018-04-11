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
#include "buzzer.h"
#include "xpt2046.h"
#include "task_file.h"
#include "ff.h"
#include "rtc.h"
#include "malloc.h"
#include "timer.h"
#include "stdio.h"
//lvgl
#include "../lv_hal/lv_hal_disp.h"
#include "../lv_misc/lv_mem.h"
#include "../lv_core/lv_obj.h"
#include "../lvgl.h"
#include "gui_demo.h"
//LWIP
#include "lan8720.h"
//lua
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "string.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//freemodbus
#include "mb.h"
//usb
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h" 

#include "task_can.h"
#include "task_modbus.h"
#include "task_gui.h"
#include "task_lua.h"

const char logo[] = "\
/*\n\
 * This file is part of the \n\
 *\n\
 * Copyright (c) 2017-2018 linghaibin\n\
 *\n\
 */\n\
 Welcome to use device!\n";


int main(void) {
    delay_init(168); /* 延时初始化 */
    l_malloc_init();    /*内存初始化*/
    buzzer_init(); /* 蜂鸣器初始化 */
    delay_ms(500);
    uart_init(115200); /* 串口初始化 */
    printf("%s",logo); 
    rtc_init();         /* 时钟初始化 */
    task_can_init();    /* can初始化 */
    task_gui_init();    /* gui初始化 */
    task_file_init();   /* 文件系统初始化 */
    task_lua_init();    /* lua环境初始化 */
    eth_init();         /* 网络初始化 */
    task_modbus_init(); /* modbus 初始化 */
    task_can_create();
    task_gui_create();/* gui任务 */
    task_lua_create();/* lua任务 */
    vTaskStartScheduler();
    while(1);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
