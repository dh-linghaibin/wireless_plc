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

void vTaskLED(void *pvParameters);
void vTaskLEDFlash(void *pvParameters);


int main(void) {
	tm1650_init();		/* 显示初始化 */
	tm1650_set_nex(0,1);/* 显示设备地址 */
	tm1650_set_nex(1,2);/* 显示设备地址 */
	tm1650_set_nex(2,3);

	xTaskCreate(vTaskLED, "Task LED", 10, NULL, 1, NULL);    
	xTaskCreate(vTaskLEDFlash, "Task LED Flash", 10, NULL, 2, NULL);
    vTaskStartScheduler();
    while (1) {       
    
	}
}

/**
  * @brief  LED task, turn on all the LED one by one and then turn off them.
  * @param  pvParameters: passing parameter when the task is created
  * @retval None
  */
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

uint8_t add = 0;

void vTaskLEDFlash(void *pvParameters) {
    while (1) {
        /* Turn on the LED */
        vTaskDelay(500 / portTICK_RATE_MS);
		tm1650_set_led(L_RUN,LV_OPEN);
		tm1650_set_led(L_ERROR,LV_OPEN);
        vTaskDelay(500 / portTICK_RATE_MS);
		tm1650_set_led(L_RUN,LV_CLOSE);
		tm1650_set_led(L_ERROR,LV_CLOSE);
		add++;
		tm1650_set_nex(0,add/100);/* 显示设备地址 */
		tm1650_set_nex(1,add%100/10);/* 显示设备地址 */
		tm1650_set_nex(2,add%10);
    }
}

