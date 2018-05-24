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
#include "si446x.h"
#include "rs485.h"
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

#include "persistence.h"

#include "iwdg.h"

const char logo[] = "\
/*\n\
 * This file is part of the \n\
 *\n\
 * Copyright (c) 2017-2018 linghaibin\n\
 *\n\
 */\n\
 Welcome to use device!\n";

USB_OTG_CORE_HANDLE USB_OTG_dev;

static lv_res_t btn_click_action(lv_obj_t * btn) {
    __set_FAULTMASK(1);//关闭总中断
    NVIC_SystemReset();//请求单片机重启
    return LV_RES_OK; /*Return OK if the button is not deleted*/
}

int main(void) {
    delay_init(168); /* 延时初始化 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); /*  官方推荐 */
    l_malloc_init();    /*内存初始化*/
    buzzer_init(); /* 蜂鸣器初始化 */
    delay_ms(100);
    uart_init(115200); /* 串口初始化 */
    printf("%s",logo); 
    rtc_init();         /* 时钟初始化 */
    task_file_init();   /* 文件系统初始化 */
    persistence_init(); /* 数据初始化 */
    task_can_init();    /* can初始化 */
    task_gui_init();    /* gui初始化 */
    
    iwdg_init(4,2000);
    {
        uint8_t buf2[1];
        persistence_get_pro_flag(buf2);
        if(buf2[0] == 0x55) {
            uint8_t buf[1];
            buf[0] = 0x00;
            persistence_set_pro_flag(buf);
            {
                lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);
                lv_btn_set_action(btn1, LV_BTN_ACTION_CLICK, btn_click_action);
                lv_obj_t * but_name = lv_label_create(btn1, NULL);
                lv_label_set_text(but_name, "更新完成");
                lv_obj_set_size(btn1, LV_HOR_RES/2, LV_VER_RES/2);
                lv_obj_align(btn1,NULL ,LV_ALIGN_CENTER,0,0);
            }
            
            USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
            delay_ms(1800);
            while(1) {
                iwdg_feed();
                delay_ms(5);
                lv_tick_inc(5);
                xpt2046_loop();
                lv_task_handler();
            }
        }
    }

    rs485_init();
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
