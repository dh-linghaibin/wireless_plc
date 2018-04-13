/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "task_can.h"
#include "can.h"
#include "task_set.h"
#include "digital_tube.h"
#include "insignal.h"

static void task_can(void *pvParameters);
static xQueueHandle can_msg_queue = NULL; /*队列句柄*/
static TimerHandle_t xtime_can; /* 定义句柄 */
static uint8_t online = 25; /* 设备状态标志 */

void task_can_init(void) {
    
}

void task_can_create(void) {
    xTaskCreate( task_can,"task_can", 100, NULL, tskIDLE_PRIORITY+3, NULL );
}

xQueueHandle task_can_get_queue(void) {
    return can_msg_queue;
}

// 定时器回调函数格式
static void vtimer_callback( TimerHandle_t xTimer ) {
    configASSERT(xTimer);
    static uint8_t dr = 1;
    if(online < 25) {
        online++;
        dr = 0;
    } else {
        dr = dr==0?1:0;
    }
    igital_tube_toggle_point(1,dr);
}

static CanRxMsg rx_message;
static void task_can(void *pvParameters) {
    /* 建立队列 */  
    can_msg_queue = xQueueCreate( 5 , sizeof( CanRxMsg ) );
    xtime_can = xTimerCreate("can_tic",         /* 定时器名字 */
                              200,             /* 定时器周期,单位时钟节拍 */
                              pdTRUE,           /* 周期性 */
                              (void *) 0,       /* 定时器ID */
                              vtimer_callback); /* 定时器回调函数 */

    if(xtime_can == NULL) {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    } else {
        /* 启动定时器，系统启动后才开始工作  0 表示不阻塞*/
        if(xTimerStart(xtime_can, 0) != pdPASS) {
            /* 定时器还没有进入激活状态 */
        }
    }
    for( ;; ) {
        //vTaskDelay(20 / portTICK_RATE_MS);
        if( xQueueReceive( can_msg_queue, &rx_message, 100/portTICK_RATE_MS ) == pdPASS) {
            switch(rx_message.StdId) {
                case RADIO: { /* 广播 */
                    switch(rx_message.Data[0]) {
                        case RADIO_ASK:{
                            CanTxMsg send_msg;
                            send_msg.StdId = bxcan_get_id();
                            send_msg.DLC = 3;
                            send_msg.Data[0] = DI_4; /* 设备类型 */
                            send_msg.Data[1] = 0xf0; /* 命令 */
                            send_msg.Data[2] = insignal_read(8); /* 继电器状态 */
                            bxcan_send(send_msg);
                            online = 0;/* 设置设备在线 */
                        } break;
                        case RADIO_SET_BR: { /* 我要修改波特率了 */
                            task_set_br(rx_message.Data[1]);
                        } break;
                    }
                } break;
                default: {
                    if(rx_message.StdId == bxcan_get_id()) { /* 判断地址 */
                        switch(rx_message.Data[0]) {
                            case 0:{ /* 设置设备 */
                                CanTxMsg send_msg;
                                send_msg.StdId = bxcan_get_id();
                                send_msg.DLC = 3;
                                send_msg.Data[0] = DI_4; /* 设备类型 */
                                send_msg.Data[1] = 0xf0; /* 命令 */
                                send_msg.Data[2] = insignal_read(rx_message.Data[1]); /* 继电器状态 */
                                bxcan_send(send_msg);
                            } break;
                            case 1: { /* 修改设备地址 */
                                task_set_address(rx_message.Data[1]);
                            } break;
                        }
                        online = 0;/* 设置设备在线 */
                    }
                }
            }
        }
    }
}

