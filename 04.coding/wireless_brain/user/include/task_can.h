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
#include "l_list.h"
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
    HOLD = 3,
    CAN_485 = 4,
} device_type;

/* 消息频道 */
typedef enum {
    RADIO = 0x0155,
    RADIO_ASK = 0,
    RADIO_SET_BR = 1,
} device_channel;

/* 设备在线记录 */
typedef struct {
    device_type type; /* 设备类型 */
    uint16_t address; /* 设备地址 */
    uint8_t online; /* 设备是否在线 */
} device_online;

typedef struct {
    device_type type; /* 设备类型 */
    uint16_t address; /* 设备地址 */
    uint8_t cmd;      /* 命令 */
    uint8_t data[7];  /* 设备命令 */
} device_send;

void task_can_init(void);
void task_can_create(void);
xQueueHandle task_can_get_queue(void);
void task_can_set(device_send send_msg);
l_list_t * task_can_get_device(void);
void tack_can_modbus_read_to_hold_set(uint8_t num);

#ifdef __cplusplus
}
#endif

#endif


