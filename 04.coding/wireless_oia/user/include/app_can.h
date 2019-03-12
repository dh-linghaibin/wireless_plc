/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _APP_CAN_H_
#define _APP_CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 设备类型 */
typedef enum {
    DO_8 = 0,
    DO_4 = 1,
    DI_4 = 2,
    IIO442 = 3, //IIO442
} device_type;

/* 消息频道 */
typedef enum {
    RADIO = 0x0155,
    RADIO_ASK = 0,
    RADIO_SET_BR = 1,
    RADIO_SET_ADDRESS = 2,
} device_channel;

void app_can_init(void);

#ifdef __cplusplus
}
#endif

#endif
