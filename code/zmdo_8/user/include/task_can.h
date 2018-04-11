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

//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/* �豸���� */
typedef enum {
    DO_8 = 0,
    DO_4 = 1,
    DI_4 = 2,
} device_type;

/* ��ϢƵ�� */
typedef enum {
    RADIO = 0x0155,
} device_channel;

void task_can_init(void);
void task_can_create(void);
xQueueHandle task_can_get_queue(void);

#ifdef __cplusplus
}
#endif

#endif


