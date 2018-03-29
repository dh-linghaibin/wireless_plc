/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "gd32f10x.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"
#include "croutine.h"

#include "tm1650.h"
#include "sign_led.h"
#include "sign_configure.h"
#include "delay.h"
#include "sm16703p.h"
#include "wireless.h"

void vTaskLED(void *pvParameters);
void vTaskLEDFlash(void *pvParameters);


int main(void) {
    delay_init();
    tm1650_init();		/* 显示初始化 */
    sign_led();
    sign_configure();
    //sm16703p_init();
    
   // xTaskCreate(vTaskLED, "Task LED", 10, NULL, 1, NULL);    
    xTaskCreate(vTaskLEDFlash, "Task LED Flash", 20, NULL, 2, NULL);
    vTaskStartScheduler();
    while (1) {       
        
    }
}

void vTaskLED(void *pvParameters) {
    while (1) {
        vTaskDelay(100 / portTICK_RATE_MS);
        uint8_t but_key = tm1650_read_key();
        switch(but_key) {
            case K_RESET: {
                
            } break;
        }
    }
}


void vTaskLEDFlash(void *pvParameters) {
    uint8_t add = 0;
    sign_write(L_AI1,3500);
    sign_write(L_AI2,4000);
    sign_write(L_AI3,4000);
    sign_write(L_AI4,4000);
    sign_write(L_AO1,4000);
    sign_write(L_AO2,4000);
    
    tm1650_set_nex(0,add/100);/* 显示设备地址 */
    tm1650_set_nex(1,add%100/10);/* 显示设备地址 */
    tm1650_set_nex(2,add%10);
    
    while (1) {
        /* Turn on the LED */
        vTaskDelay(500 / portTICK_RATE_MS);
        tm1650_set_led(L_RUN,LV_OPEN);
        tm1650_set_led(L_ERROR,LV_OPEN);
        vTaskDelay(500 / portTICK_RATE_MS);
        tm1650_set_led(L_RUN,LV_CLOSE);
        tm1650_set_led(L_ERROR,LV_CLOSE);
    //		add = sign_read(S_DI4);
        add++;
        tm1650_set_nex(0,add/100);/* 显示设备地址 */
        tm1650_set_nex(1,add%100/10);/* 显示设备地址 */
        tm1650_set_nex(2,add%10);
        sign_update();
    }
}

