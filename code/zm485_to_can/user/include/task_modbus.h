/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _TASK_MODBUS_H_
#define _TASK_MODBUS_H_

#ifdef __cplusplus
extern "C" {
#endif

//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

void task_modbus_init(void);
void task_modbus_create(void);
uint32_t HAL_GetTick(void);

#ifdef __cplusplus
}
#endif

#endif
