/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "task_can.h"
#include "can.h"
#include "digital_tube.h"
#include "task_set.h"
#include "rs485.h"
#include "modbus_master.h"

static void task_modbus_tic(void *pvParameters);
static uint32_t uwTick = 0;

static void task_can_val(void *pvParameters);
static void task_can(void *pvParameters);
static xQueueHandle can_msg_queue = NULL; /*队列句柄*/
static TimerHandle_t xtime_can; /* 定义句柄 */
static uint8_t online = 25; /* 设备状态标志 */

void task_can_init(void) {

}

void task_can_create(void) {
    xTaskCreate( task_can,"task_can", 256, NULL, tskIDLE_PRIORITY+2, NULL );
    //xTaskCreate( task_can_val,"task_can_val", 256, NULL, tskIDLE_PRIORITY+2, NULL );
    
    //xTaskCreate( task_modbus,"task_modbus", 256, NULL, tskIDLE_PRIORITY+2, NULL );
    xTaskCreate( task_modbus_tic,"task_modbus_tic", 256, NULL, tskIDLE_PRIORITY+2, NULL );
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

static uint16_t get_bit(uint16_t val,uint8_t num) {
    return (val >> num) & 0x0001;
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
    ModbusMaster_begin();
    for( ;; ) {
        if( xQueueReceive( can_msg_queue, &rx_message, 100/portTICK_RATE_MS ) == pdPASS) {
            switch(rx_message.rx_sfid) {
                case RADIO: { /* 广播 */
                    switch(rx_message.rx_data[0]) {
                        case RADIO_ASK:{
                            can_trasnmit_message_struct send_msg;
                            send_msg.tx_sfid = bxcan_get_id();
                            send_msg.tx_dlen = 3;
                            send_msg.tx_data[0] = CAN_485; /* 设备类型 */
                            send_msg.tx_data[1] = 0xf0; /* 命令 */
                            send_msg.tx_data[2] = 0x00; /* 继电器状态 */
                            bxcan_send(send_msg);
                            online = 0;/* 设置设备在线 */
                        } break;
                        case RADIO_SET_BR: { /* 我要修改波特率了 */
                            task_set_br(rx_message.rx_data[1]);
                        } break;
                    }
                } break;
                default: {
                    if(rx_message.rx_sfid == bxcan_get_id()) { /* 判断地址 */
                        switch(rx_message.rx_data[0]) {
                            case CAN_485 :{ /* 设置设备 */
                                uint8_t result;
                                uint16_t  Input_Result;
                                if(rx_message.rx_data[1] == 0x10) {
                                    uint16_t num_adr;
                                    uint8_t type = rx_message.rx_data[4];
                                    uint8_t read_num = rx_message.rx_data[5];
                                    num_adr = rx_message.rx_data[6];
                                    num_adr |= (rx_message.rx_data[7] << 8);
                                    switch(type) {
                                        case 1 : {
                                            result = ModbusMaster_readCoils(rx_message.rx_data[2],rx_message.rx_data[3], read_num);
                                        } break;
                                        case 2 : {
                                            result = ModbusMaster_readDiscreteInputs(rx_message.rx_data[2],rx_message.rx_data[3], read_num);
                                        } break;
                                        case 3 : {
                                            result = ModbusMaster_readHoldingRegisters(rx_message.rx_data[2],rx_message.rx_data[3], read_num);
                                        } break;
                                        case 4 : {
                                            result = ModbusMaster_readInputRegisters(rx_message.rx_data[2],rx_message.rx_data[3], read_num);
                                        } break;
                                    }
                                    if (result == 0x00) {
                                        for(int r_i = 0; r_i < read_num; r_i++) {
                                            switch(type) {
                                                case 1 : {
                                                    Input_Result = get_bit(ModbusMaster_getResponseBuffer(r_i/16),r_i);
                                                } break;
                                                case 2 : {
                                                    Input_Result = get_bit(ModbusMaster_getResponseBuffer(r_i/16),r_i);
                                                } break;
                                                case 3 : {
                                                    Input_Result = ModbusMaster_getResponseBuffer(r_i);
                                                } break;
                                                case 4 : {
                                                    Input_Result = ModbusMaster_getResponseBuffer(r_i);
                                                } break;
                                            }
                                            
                                            can_trasnmit_message_struct send_msg;
                                            send_msg.tx_sfid = bxcan_get_id();
                                            send_msg.tx_dlen = 6;
                                            send_msg.tx_data[0] = CAN_485; /* 设备类型 */
                                            send_msg.tx_data[1] = 0xf1; /* 命令 */
                                            send_msg.tx_data[2] = Input_Result; /* 继电器状态 */
                                            send_msg.tx_data[3] = Input_Result >> 8; /* 继电器状态 */
                                            send_msg.tx_data[4] = num_adr;
                                            send_msg.tx_data[5] = (num_adr >> 8);
                                            bxcan_send(send_msg);
                                            num_adr++;
                                            vTaskDelay(1 / portTICK_RATE_MS);
                                        }
                                    } else {
                                        can_trasnmit_message_struct send_msg;
                                        send_msg.tx_sfid = bxcan_get_id();
                                        send_msg.tx_dlen = 3;
                                        send_msg.tx_data[0] = CAN_485; /* 设备类型 */
                                        send_msg.tx_data[1] = 0xf2; /* 命令 */
                                        send_msg.tx_data[2] = result; /* 错误码 */
                                        bxcan_send(send_msg);
                                    }
                                } else {
                                    switch(rx_message.rx_data[4]) {
                                        case 1 : {
                                            result = ModbusMaster_writeSingleCoil(rx_message.rx_data[2],rx_message.rx_data[3], rx_message.rx_data[5]);
                                        } break;
                                        case 2 : {
                                            result = ModbusMaster_writeSingleRegister(rx_message.rx_data[2],rx_message.rx_data[3], rx_message.rx_data[5] | rx_message.rx_data[6] << 8);
                                        } break;
                                        case 3 : {
                                            result = ModbusMaster_writeMultipleCoils(rx_message.rx_data[2],rx_message.rx_data[3], rx_message.rx_data[5] | rx_message.rx_data[6] << 8 );
                                        } break;
                                        case 4 : {
                                            result = ModbusMaster_writeMultipleRegisters(rx_message.rx_data[2],rx_message.rx_data[3], rx_message.rx_data[5] | rx_message.rx_data[6] << 8 );
                                        } break;
                                    }
                                }
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
    uint8_t result;
    
    for( ;; ) {
        vTaskDelay(2000 / portTICK_RATE_MS);
        uint8_t val = 0x00;
        result = ModbusMaster_readDiscreteInputs(2,0, 1);
        if (result == 0x00) {
            vTaskDelay(20 / portTICK_RATE_MS);
        } else {
            vTaskDelay(1000 / portTICK_RATE_MS);
        }
    }
}



static void task_modbus_tic(void *pvParameters) {
    for( ;; ) {
        uwTick++;
        vTaskDelay(1 / portTICK_RATE_MS);
    }
}

uint32_t HAL_GetTick(void) {
  return uwTick;
}
