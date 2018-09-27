/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "task_gui.h"
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

#include "button.h"

static xTaskHandle xhandle_gui; /* lua¾ä±ú */

static void vtask_show_device(void *pvParameters);
static void vtask_gui_tic(void *pvParameters) ;
extern USB_OTG_CORE_HANDLE USB_OTG_dev;

void task_gui_init(void) {
    button_init();
    SPI3_Init();
    xpt2046_init();
    tft_init();
}


void task_gui_create(void) {
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
        xpt2046_loop();
    }
}


//static TimerHandle_t xtime_buzz;
static uint8_t sleep_flag = 0;
uint32_t sleep_flag_count = 0;

uint8_t task_gui_get_sleep_flag(void) {
    return sleep_flag;
}

static void vtask_show_device(void *pvParameters) {
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
