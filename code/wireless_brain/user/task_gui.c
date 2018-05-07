/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "task_gui.h"
#include "../lv_hal/lv_hal_disp.h"
#include "../lv_misc/lv_mem.h"
#include "../lv_core/lv_obj.h"
#include "../lvgl.h"
#include "spi.h"
#include "tft.h"
#include "xpt2046.h"
#include "buzzer.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdio.h"
#include "delay.h"
//usb
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h" 

#include "task_lua.h"
#include "task_can.h"

#include "ui_menu.h"

static xTaskHandle xhandle_gui; /* lua句柄 */

static void vtask_gui_tic(void *pvParameters);
static void demo_init(void);
static void vtask_show_device(void *pvParameters);
extern USB_OTG_CORE_HANDLE USB_OTG_dev;

void task_gui_init(void) {
    SPI3_Init();
    xpt2046_init();
    tft_init();
    lv_vdb_init();
    lv_init();
    lv_disp_drv_t disp_drv;  
    lv_disp_drv_init(&disp_drv);  
    disp_drv.disp_flush = monitor_flush; /* 显示屏接口 */
    lv_disp_drv_register(&disp_drv);
    
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv); 
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = mouse_read;     /* 触摸接口 */
    lv_indev_drv_register(&indev_drv);

    lv_theme_t *th = lv_theme_material_init(200, NULL);
    lv_theme_set_current(th);
}

void task_gui_create(void) {
    ui_menu_create();
    xTaskCreate(vtask_gui_tic, "ui", 512, NULL, 3, &xhandle_gui);
    //task_gui_set(0);
   // xTaskCreate(vtask_show_device, "device", 512, NULL, 3, NULL);
}


void task_gui_set(uint8_t cmd) {
    if(cmd) {
        tft_set(1);
        tft_set_backlight(0);
        vTaskResume( xhandle_gui ); /* 恢复lua任务 */
    } else {
        tft_set(0);
        tft_set_backlight(1);
        vTaskSuspend( xhandle_gui ); /* 挂起lua任务 */
    }
}

static void vtask_gui_tic(void *pvParameters) {
    for( ;; ) {
        vTaskDelay(5 / portTICK_RATE_MS);
        lv_tick_inc(5);
        xpt2046_loop();
        lv_task_handler();
    }
}

//USB_OTG_CORE_HANDLE USB_OTG_dev;

static lv_res_t change_action(lv_obj_t * sw) {
    if(lv_sw_get_state(sw) == true) {
        task_lua_set(false);
        //taskENTER_CRITICAL();   /* 进入临界区 */
        //USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
        //delay_ms(2800);
        //taskEXIT_CRITICAL();    /* 退出临界区 */
    } else {
        task_lua_set(true);
        //DCD_DevDisconnect(&USB_OTG_dev);
    }
    buzzer_set(50,1);
    return LV_RES_OK;
}

lv_obj_t * list1;
lv_obj_t* label3;

static void vtask_show_device(void *pvParameters) {
    for( ;; ) {
//        vTaskDelay(1000 / portTICK_RATE_MS);
//        lv_obj_del(list1);
//        list1 = lv_list_create(lv_scr_act(), NULL);
//        lv_obj_set_size(list1, 130, 170);
//        device_online *temp;
//        for (int i = 0; i < list_len(task_can_get_device()); i++){ //list_len获取链表的长度
//            list_get(task_can_get_device(), i, (void **)&temp); //取得位置为i的结点的数据
//            char *buf = l_malloc(sizeof(char)*10);
//            switch(temp->type) {
//                case DO_8: {
//                    sprintf(buf, "DI_8  %d", temp->address);
//                } break;
//                case DO_4: {
//                    sprintf(buf, "DO_4  %d", temp->address);
//                } break;
//                case DI_4: {
//                    sprintf(buf, "DI_4  %d", temp->address);
//                } break;
//            }
//            lv_list_add(list1, SYMBOL_FILE, buf, list_release_action);
//            l_free(buf);
//        }
    }
}
