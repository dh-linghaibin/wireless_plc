/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _TSAK_CAN_H_
#define _TSAK_CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/* 设备类型 */
typedef enum {
    DO_8 = 0,
    DO_4 = 1,
    DI_4 = 2,
} device_type;

/* 设备在线记录 */
typedef struct {
    device_type type; /* 设备类型 */
    uint16_t address; /* 设备地址 */
    uint8_t online; /* 设备是否在线 */
} device_online;

void task_can_init(void);
void task_can_create(void);
xQueueHandle task_can_get_queue(void);

#ifdef __cplusplus
}
#endif

#endif


