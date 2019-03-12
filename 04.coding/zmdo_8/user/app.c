/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#include "can.h"
#include "tm1650.h"
#include "outsignal.h"
#include "flash.h"
#include "wdog.h"
#include "delay.h"
#include "task_can.h"
#include "task_set.h"
#include "wdog.h"

int main(void) {
    delay_init();
    delay_ms(100);
    tm1650_init();
    delay_ms(100);
    wdog_init();/* 看门狗初始化 */
    outsigned_init();
    task_set_init();
    task_can_init();
    task_can_create();
    task_set_create();
    /* Start the scheduler. */
    vTaskStartScheduler();
    while(1);
}




