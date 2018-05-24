/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "task_can.h"
#include "task_modbus.h"
#include "can.h"
#include "stdio.h"
#include "iwdg.h"
#include "persistence.h"

static void task_can(void *pvParameters);
static void task_can_send(void *pvParameters);
static xQueueHandle can_msg_queue = NULL; /*队列句柄*/
static xQueueHandle can_send_queue = NULL; /*队列句柄*/
static TimerHandle_t xtime_can; /* 定义句柄 */
static l_list_t * online_head; /* 链表头 */

static void up_device(device_online online) {
    device_online *temp;
    for (int i = 0; i < list_len(online_head); i++){ //list_len获取链表的长度
        list_get(online_head, i, (void **)&temp); //取得位置为i的结点的数据
        //printf("%d\n", temp->type);
        if( (temp->type == online.type) && (temp->address == online.address) ) { /* 判断设备是否加入当前链表 */
            temp->online = 0;/* 说明设备在线 */
            return;
        }
    }
    /* 创建设备 */
    device_online * device;
    device = l_malloc(sizeof(device_online));
    device->type    = online.type;
    device->address = online.address;
    device->online  = 0;/* 说明设备在线 */
    list_append(online_head, device); //追加结点
}

void task_can_init(void) {
    online_head = (l_list_t *)l_malloc(sizeof(l_list_t)); /* 创建头节点 */
    static device_online * online;
    list_init(online_head); //初始化链表
    //can1_init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal); /* 初始化can 500k */
    uint8_t but = 0;
    persistence_get_can_adr(&but);
    can1_init(but,CAN_Mode_Normal); /* 初始化can 50k */
}

void task_can_create(void) {
    xTaskCreate( task_can,"task_can", 1024, NULL, tskIDLE_PRIORITY+3, NULL );
    xTaskCreate( task_can_send,"task_send", 512, NULL, tskIDLE_PRIORITY+3, NULL );
}

xQueueHandle task_can_get_queue(void) {
    return can_msg_queue;
}

l_list_t * task_can_get_device(void) {
    return online_head;;
}

// 定时器回调函数格式
static void vtimer_callback( TimerHandle_t xTimer ) {
    uint32_t ulTimerID;
    configASSERT(xTimer);
    //printf("time \n");
    device_online *temp;
    for (int i = 0; i < list_len(online_head); i++){ //list_len获取链表的长度
        list_get(online_head, i, (void **)&temp); //取得位置为i的结点的数据
        if(temp->online > 4) { /* 设备超时未响应 删除设备 */
            list_pop(online_head,i);
            l_free(temp);
        } else {
            //printf("设备类型:%d  设备地址:%d 设备在线:%d\n", temp->type,temp->address,temp->online);
            temp->online ++;
        }
    }
    /* 广播 */
    CanTxMsg tx_msg;
    tx_msg.StdId = RADIO;
    tx_msg.ExtId = RADIO;
    tx_msg.RTR=CAN_RTR_DATA;
    tx_msg.IDE=CAN_ID_STD;
    tx_msg.Data[0] = RADIO_ASK;
    tx_msg.DLC = 1;
    can1_send_msg(tx_msg);
    
    iwdg_feed();
}

static void task_can(void *pvParameters) {
    CanRxMsg rx_message;
    /* 建立队列 */  
    can_msg_queue = xQueueCreate( 20 , sizeof( CanRxMsg ) );
    xtime_can = xTimerCreate("can_tic",         /* 定时器名字 */
                              2000,             /* 定时器周期,单位时钟节拍 */
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
            switch(rx_message.Data[0]) {
                case DO_8: {
                    switch(rx_message.Data[1]) {
                        case 0xf0: { /* 读取 */
                            //printf("DO-8 地址 %d 变量 %d\n",rx_message.StdId,rx_message.Data[2]);
                            task_modbus_set_coil(rx_message.StdId,rx_message.Data[2]);
                        } break;
                        case 0x0f1: {
                        
                        } break;
                    }
                    device_online online;
                    online.type = DO_8;
                    online.address = rx_message.StdId;
                    up_device(online);
                } break;
                case DO_4: {
                    switch(rx_message.Data[1]) {
                        case 0xf0: { /* 读取 */
                            //printf("DO-4 地址 %d 变量 %d\n",rx_message.StdId,rx_message.Data[2]);
                            task_modbus_set_coil(rx_message.StdId,rx_message.Data[2]);
                        } break;
                        case 0x0f1: {
                        
                        } break;
                    }
                    device_online online;
                    online.type = DO_4;
                    online.address = rx_message.StdId;
                    up_device(online);
                } break;
                case DI_4: {
                    switch(rx_message.Data[1]) {
                         case 0xf0: { /* 读取 */
                            task_modbus_set_input(rx_message.StdId,rx_message.Data[2]);
                         } break;
                    }
                    device_online online;
                    online.type = DI_4;
                    online.address = rx_message.StdId;
                    up_device(online);
                } break;
                default:{
                
                } break;
            }
        }
    }
}

void task_can_set(device_send send_msg) {
    if(can_send_queue != NULL) {
        xQueueSendFromISR( can_send_queue, &send_msg, 0 ); /* 0的意思是立即进入队列 */
    }
}

static void task_can_send(void *pvParameters) {
    device_send send_msg;
    /* 建立队列 */  
    can_send_queue = xQueueCreate( 20 , sizeof( device_send ) );
    for( ;; ) {
        if( xQueueReceive( can_send_queue, &send_msg, 100/portTICK_RATE_MS ) == pdPASS) {
            switch(send_msg.type) {
                case DO_8: {
                    CanTxMsg tx_msg;
                    tx_msg.StdId = send_msg.address;
                    tx_msg.ExtId = send_msg.address;
                    tx_msg.RTR=CAN_RTR_DATA;
                    tx_msg.IDE=CAN_ID_STD;
                    tx_msg.Data[0] = DO_8;
                    tx_msg.Data[1] = send_msg.data[0];
                    tx_msg.Data[2] = send_msg.data[1];
                    tx_msg.DLC = 3;
                    can1_send_msg(tx_msg);
                } break;
                case DO_4: {
                
                } break;
                case DI_4: {
                
                } break;
            }
        }
    }
}

