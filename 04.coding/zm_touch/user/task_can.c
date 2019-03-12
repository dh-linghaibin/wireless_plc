/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "task_can.h"
#include "can.h"
#include "digital_tube.h"
#include "insignal.h"
#include "task_set.h"
#include "touch.h"
#include "sign_led.h"

static void task_can_val(void *pvParameters);
static void task_can(void *pvParameters);
static xQueueHandle can_msg_queue = NULL; /*队列句柄*/
static TimerHandle_t xtime_can; /* 定义句柄 */
static uint8_t online = 25; /* 设备状态标志 */

void task_can_init(void) {
    
}

void task_can_create(void) {
    xTaskCreate( task_can,"task_can", 512, NULL, tskIDLE_PRIORITY+2, NULL );
    xTaskCreate( task_can_val,"task_can_val", 512, NULL, tskIDLE_PRIORITY+2, NULL );
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

uint16_t val_bit(uint16_t val) {
    switch(val) {
        case 0 : {
            return 0x01;
        }
        case 1 : {
            return 0x02;
        }
        case 2 : {
            return 0x04;
        }
        case 3 : {
            return 0x08;
        }
        case 4 : {
            return 0x10;
        }
        case 5 : {
            return 0x20;
        }
        case 6 : {
            return 0x40;
        }
        case 7 : {
            return 0x80;
        }
        case 8 : {
            return 0x80;
        }
    }
    return 0x00;
}

static void task_can(void *pvParameters) {
    can_receive_message_struct rx_message;
    /* 建立队列 */  
    can_msg_queue = xQueueCreate( 20 , sizeof( can_receive_message_struct ) );
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
        if( xQueueReceive( can_msg_queue, &rx_message, 100/portTICK_RATE_MS ) == pdPASS) {
            switch(rx_message.rx_sfid) {
                case RADIO: { /* 广播 */
                    switch(rx_message.rx_data[0]) {
                        case RADIO_ASK:{
                            can_trasnmit_message_struct send_msg;
                            send_msg.tx_sfid = bxcan_get_id();
                            send_msg.tx_dlen = 4;
                            send_msg.tx_data[0] = DO_8; /* 设备类型 */
                            send_msg.tx_data[1] = 0xf0; /* 命令 */
                            send_msg.tx_data[2] = touch_get_val(); /* 继电器状态 */
                            send_msg.tx_data[3] = (touch_get_val() >> 8); /* 继电器状态 */
                            bxcan_send(send_msg);
                            online = 0;/* 设置设备在线 */
                        } break;
                        case RADIO_SET_BR: { /* 我要修改波特率了 */
                            task_set_br(rx_message.rx_data[1]);
                        } break;
                    }
                } break;
                default: {
                    if( ( rx_message.rx_sfid == bxcan_get_id() ) || ( rx_message.rx_sfid == bxcan_get_id() + 1 ) ) { /* 判断地址 */
                        switch(rx_message.rx_data[0]) {
                            case 0:{ /* 设置设备 */
                                  if(rx_message.rx_sfid == bxcan_get_id()) {
                                      if(rx_message.rx_data[2] > 0) {
                                        sign_led_set(val_bit(rx_message.rx_data[1]));
                                        touch_get_val_set(val_bit(rx_message.rx_data[1]));
                                      }
                                  } else {
                                    if(rx_message.rx_data[2] > 0) {
                                        sign_led_set(0x100);
                                        touch_get_val_set(0x100);
                                    }
                                  }
                                  audio_set(0);
                            } break;
                            case 1: { /* 修改设备地址 */
                                task_set_address(rx_message.rx_data[1]);
                            } break;
                        }
                        online = 0;/* 设置设备在线 */
                    }
                }
            }
        }
    }
}

static void task_can_val(void *pvParameters) {
    uint16_t l_val = 0x0f;
    uint8_t change_count = 0;
    for( ;; ) {
        vTaskDelay(20 / portTICK_RATE_MS);
        touch_key_e t_k = touch_read();
        uint16_t val = touch_get_val();
        if(t_k != KEY_NULL) {
            sign_led_set(val);
            audio_set(0);
        }
        if(val != l_val) {
            if(change_count < 5) {
                change_count++;
            } else if(change_count == 5) {
                change_count = 0;
                l_val = val;
                can_trasnmit_message_struct send_msg;
                send_msg.tx_sfid = bxcan_get_id();
                send_msg.tx_dlen = 4;
                send_msg.tx_data[0] = DO_8; /* 设备类型 */
                send_msg.tx_data[1] = 0xf0; /* 命令 */
                send_msg.tx_data[2] = touch_get_val(); /* 继电器状态 */
                send_msg.tx_data[3] = (touch_get_val() >> 8);
                bxcan_send(send_msg);
            }
        } else {
            change_count = 0;
        }
    }
}
