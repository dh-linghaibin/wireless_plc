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

    lv_theme_t *th = lv_theme_material_init(200, NULL);
    lv_theme_set_current(th);
}

void task_gui_create(void) {
    ui_menu_create();
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


static TimerHandle_t xtime_buzz;
static uint8_t sleep_flag = 0;

uint8_t task_gui_get_sleep_flag(void) {
    return sleep_flag;
}

static void buzzer_callback( TimerHandle_t xTimer ) {
    configASSERT(xTimer);
    xTimerDelete( xTimer ,0 );
    task_gui_set(0);
    sleep_flag = 1;
}

static void vtask_show_device(void *pvParameters) {
    if( xTimerIsTimerActive( xtime_buzz ) != pdFALSE ) {
        xtime_buzz = xTimerCreate("can_tic",      
                                  20000,        
                                  pdTRUE,     
                                  (void *) 0,    
                                  buzzer_callback); 

        if(xtime_buzz == NULL) {
         
        } else {
            if(xTimerStart(xtime_buzz, 0) == pdPASS) {
                
            }
        }
    }
    for( ;; ) {
        vTaskDelay(5 / portTICK_RATE_MS);
        switch(button_read()) {
            case 0x00: {
            
            } break;
            case 0x01: {
                if(sleep_flag == 1) {
                    sleep_flag = 0;
                    task_gui_set(1);
                    if(xTimerStart(xtime_buzz, 0) == pdPASS) {
                
                    }
                }
            } break;
        }
    }
}
