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

static xTaskHandle xhandle_gui; /* lua句柄 */

static void vtask_gui_tic(void *pvParameters);
static void demo_init(void);
static void vtask_show_device(void *pvParameters);

/*Create a button descriptor string array*/
static const char * btnm_map[] = {"1", "2", "3", "4", "5", "\n",
                           "6", "7", "8", "9", "0", "\n",
                           "\202del", "enter", ""};

void task_gui_init(void) {
    SPI3_Init();
    xpt2046_init();
    tft_init();
    lv_vdb_init();
    lv_init();
    lv_disp_drv_t disp_drv;  
    lv_disp_drv_init(&disp_drv);  
    disp_drv.disp_flush = monitor_flush; /* 显示屏接口 */

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv); 
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = mouse_read;     /* 触摸接口 */
    lv_indev_drv_register(&indev_drv);

    lv_disp_drv_register(&disp_drv);
    /*Create a simple base object*/
    
    static lv_style_t style_bg;
    lv_style_copy(&style_bg, &lv_style_plain);
    style_bg.body.main_color = LV_COLOR_SILVER;
    style_bg.body.grad_color = LV_COLOR_SILVER;
    style_bg.body.padding.hor = 0;
    style_bg.body.padding.ver = 0;
    style_bg.body.padding.inner = 0;

    /*Create 2 button styles*/
    static lv_style_t style_btn_rel;
    static lv_style_t style_btn_pr;
    lv_style_copy(&style_btn_rel, &lv_style_btn_rel);
    style_btn_rel.body.main_color = LV_COLOR_MAKE(0x30, 0x30, 0x30);
    style_btn_rel.body.grad_color = LV_COLOR_BLACK;
    style_btn_rel.body.border.color = LV_COLOR_SILVER;
    style_btn_rel.body.border.width = 1;
    style_btn_rel.body.border.opa = LV_OPA_50;
    style_btn_rel.body.radius = 0;

    lv_style_copy(&style_btn_pr, &style_btn_rel);
    style_btn_pr.body.main_color = LV_COLOR_MAKE(0x55, 0x96, 0xd8);
    style_btn_pr.body.grad_color = LV_COLOR_MAKE(0x37, 0x62, 0x90);
    style_btn_pr.text.color = LV_COLOR_MAKE(0xbb, 0xd5, 0xf1);

    /*Create a second button matrix with the new styles*/
    lv_obj_t * btnm2 = lv_btnm_create(lv_scr_act(), NULL);
    lv_btnm_set_map(btnm2, btnm_map);
    lv_btnm_set_style(btnm2, LV_BTNM_STYLE_BG, &style_bg);
    lv_btnm_set_style(btnm2, LV_BTNM_STYLE_BTN_REL, &style_btn_rel);
    lv_btnm_set_style(btnm2, LV_BTNM_STYLE_BTN_PR, &style_btn_pr);
    //lv_obj_align(btnm2, btnm1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}

void task_gui_create(void) {
    xTaskCreate(vtask_gui_tic, "ui", 512, NULL, 3, &xhandle_gui);
    task_gui_set(0);
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
