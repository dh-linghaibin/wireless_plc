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
#include "gui_demo.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

static void vtask_gui_tic(void *pvParameters);

void task_gui_init(void) {
    SPI3_Init();
    xpt2046_init();
    lv_vdb_init();
    lv_init();
    lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/
    /*Set up the functions to access to your display*/
    disp_drv.disp_flush = monitor_flush;            /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_drv_register(&indev_drv);

    lv_disp_drv_register(&disp_drv);
    /*Create a simple base object*/
    demo_create();
}

void task_gui_create(void) {
    xTaskCreate(vtask_gui_tic, "ui", 512, NULL, 3, NULL);
}

static void vtask_gui_tic(void *pvParameters) {
    for( ;; ) {
        vTaskDelay(5 / portTICK_RATE_MS);
        lv_tick_inc(5);
        lv_task_handler();
        xpt2046_loop();
    }
}

