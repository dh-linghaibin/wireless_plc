/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "task_can.h"
#include "can.h"
#include "stdio.h"

static void task_can_send(void *pvParameters);
static xQueueHandle can_msg_queue = NULL;

void task_can_init(void) {
    can1_init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal);
}

void task_can_create(void) {
    xTaskCreate( task_can_send, ( signed portCHAR * ) "task_can_send", 2048, NULL, tskIDLE_PRIORITY+3, NULL );
}

xQueueHandle task_can_get_queue(void) {
    return can_msg_queue;
}

static void task_can_send(void *pvParameters) {
    u8* canbuf;
    CanRxMsg rx_message;
    /* 建立队列 */  
    can_msg_queue = xQueueCreate( 20 , sizeof( CanRxMsg ) );
    canbuf = l_malloc(sizeof(u8)*8);
    for( ;; ) {
        if( xQueueReceive( can_msg_queue, &rx_message, 100/portTICK_RATE_MS ) == pdPASS) {
            switch(rx_message.Data[0]) {
                case DO_8: {
                    switch(rx_message.Data[1]) {
                        case 0: { /* 读取 */
                            printf("DO-8 地址 %d 变量 %d\n",rx_message.StdId,rx_message.Data[2]);
                        } break;
                    }
                } break;
                case DO_4: {
                    switch(rx_message.Data[1]) {
                        case 0: { /* 读取 */
                            printf("DO-4 地址 %d 变量 %d\n",rx_message.StdId,rx_message.Data[2]);
                        } break;
                    }
                } break;
                case DI_4: {
                
                } break;
                default:{
                
                } break;
            }
            //can1_send_msg(canbuf,8);
        }
    }
}
