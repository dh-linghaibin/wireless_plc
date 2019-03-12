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

#include "button.h"

static xTaskHandle xhandle_gui; /* lua句柄 */

static void vtask_gui_tic(void *pvParameters);
static void demo_init(void);
static void vtask_show_device(void *pvParameters);
extern USB_OTG_CORE_HANDLE USB_OTG_dev;

void task_gui_init(void) {
    button_init();
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

    lv_theme_t *th = lv_theme_material_init(16, NULL);
    lv_theme_set_current(th);
}

static void demo_init(void) {
    lv_obj_t *tab;
    lv_obj_t *tab1;
    lv_obj_t *tab2;
    lv_obj_t *tab3;

    // lv_theme_t* theme = lv_theme_zen_init(22, NULL);
    lv_theme_t* theme = lv_theme_material_init(16, NULL);
    lv_theme_set_current(theme);

    lv_obj_t* win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, "wireless_b");

    //	lv_win_add_btn(win, SYMBOL_CLOSE, NULL);
    lv_win_add_btn(win, SYMBOL_UPLOAD, NULL);
    lv_win_add_btn(win, SYMBOL_BLUETOOTH, NULL);
    lv_win_set_btn_size(win,LV_VER_RES/8);
    lv_win_set_sb_mode(win,LV_SB_MODE_DRAG);

    tab = lv_tabview_create(win, NULL);
    tab1 = lv_tabview_add_tab(tab, "tabv1");
    tab2 = lv_tabview_add_tab(tab, "tabv2");
    tab3 = lv_tabview_add_tab(tab, "tabv3");
    lv_tabview_set_anim_time(tab, 200);


    lv_obj_t* btn1 = lv_btn_create(tab1, NULL);
    lv_obj_set_size(btn1, 160, 60);
    lv_obj_align(btn1, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

    lv_obj_t* label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Hello!\n#ffff00 LittlevGl2RTT#");
    lv_label_set_anim_speed(label, 30);
    lv_label_set_long_mode(label, LV_LABEL_LONG_DOT);
    lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_recolor(label, true);

    lv_obj_t* label2 = lv_label_create(tab1, NULL);
    lv_obj_set_pos(label2, 20, 100);
    lv_label_set_text(label2, "bluetooth "SYMBOL_BLUETOOTH);
    
    /*Create a switch and apply the styles*/
    lv_obj_t *sw1 = lv_sw_create(tab2, NULL);
    lv_obj_align(sw1, NULL, LV_ALIGN_CENTER, 0, -50);
    //lv_sw_set_action(sw1, change_action);

    lv_obj_t * label3 = lv_label_create(tab2, NULL);
    lv_obj_set_pos(label3, 20, 100);
    lv_label_set_text(label3, "pro_lua "SYMBOL_OK);
}

void task_gui_create(void) {
    ui_menu_create();
    //demo_init();
    xTaskCreate(vtask_gui_tic, "ui", 512, NULL, 3, &xhandle_gui);
    xTaskCreate(vtask_show_device, "device", 512, NULL, 3, NULL);
}


void task_gui_set(uint8_t cmd) {
    if(cmd) {
        tft_set(1);
        tft_set_backlight(0);
        vTaskResume( xhandle_gui );
    } else {
        tft_set(0);
        tft_set_backlight(1);
        vTaskSuspend( xhandle_gui );
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


//static TimerHandle_t xtime_buzz;
static uint8_t sleep_flag = 0;
uint32_t sleep_flag_count = 0;

uint8_t task_gui_get_sleep_flag(void) {
    return sleep_flag;
}

//static void buzzer_callback( TimerHandle_t xTimer ) {
//    configASSERT(xTimer);
//    xTimerDelete( xTimer ,0 );
//    task_gui_set(0);
//    sleep_flag = 1;
//}

static void vtask_show_device(void *pvParameters) {
//    if( xTimerIsTimerActive( xtime_buzz ) != pdFALSE ) {
//        xtime_buzz = xTimerCreate("can_tic",      
//                                  20000,        
//                                  pdTRUE,     
//                                  (void *) 0,    
//                                  buzzer_callback); 

//        if(xtime_buzz == NULL) {
//         
//        } else {
//            if(xTimerStart(xtime_buzz, 0) == pdPASS) {
//                
//            }
//        }
//    }
    for( ;; ) {
        vTaskDelay(5 / portTICK_RATE_MS);
        if(sleep_flag_count < 8000) {
            sleep_flag_count++;
        } else {
            if(sleep_flag_count == 8000) {
                sleep_flag_count++;
                task_gui_set(0);
                sleep_flag = 1;
            }
        }
        switch(button_read()) {
            case 0x00: {
            
            } break;
            case 0x01: {
                if(sleep_flag == 1) {
                    sleep_flag_count = 0;
                    sleep_flag = 0;
                    task_gui_set(1);
                }
            } break;
        }
    }
}
