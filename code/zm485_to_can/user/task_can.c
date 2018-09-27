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
static xQueueHandle can_msg_queue = NULL; /*���о��*/
static TimerHandle_t xtime_can; /* ������ */
static uint8_t online = 25; /* �豸״̬��־ */

void task_can_init(void) {

}

void task_can_create(void) {
    xTaskCreate( task_can,"task_can", 256, NULL, tskIDLE_PRIORITY+2, NULL );
    xTaskCreate( task_can_val,"task_can_val", 256, NULL, tskIDLE_PRIORITY+2, NULL );
    
    //xTaskCreate( task_modbus,"task_modbus", 256, NULL, tskIDLE_PRIORITY+2, NULL );
    xTaskCreate( task_modbus_tic,"task_modbus_tic", 256, NULL, tskIDLE_PRIORITY+2, NULL );
}

xQueueHandle task_can_get_queue(void) {
    return can_msg_queue;
}

// ��ʱ���ص�������ʽ
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

static void task_can(void *pvParameters) {
    can_receive_message_struct rx_message;
    /* �������� */  
    can_msg_queue = xQueueCreate( 20 , sizeof( can_receive_message_struct ) );
    xtime_can = xTimerCreate("can_tic",         /* ��ʱ������ */
                              200,             /* ��ʱ������,��λʱ�ӽ��� */
                              pdTRUE,           /* ������ */
                              (void *) 0,       /* ��ʱ��ID */
                              vtimer_callback); /* ��ʱ���ص����� */

    if(xtime_can == NULL) {
        /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
    } else {
        /* ������ʱ����ϵͳ������ſ�ʼ����  0 ��ʾ������*/
        if(xTimerStart(xtime_can, 0) != pdPASS) {
            /* ��ʱ����û�н��뼤��״̬ */
        }
    }
    ModbusMaster_begin();
    for( ;; ) {
        if( xQueueReceive( can_msg_queue, &rx_message, 100/portTICK_RATE_MS ) == pdPASS) {
            switch(rx_message.rx_sfid) {
                case RADIO: { /* �㲥 */
                    switch(rx_message.rx_data[0]) {
                        case RADIO_ASK:{
                            can_trasnmit_message_struct send_msg;
                            send_msg.tx_sfid = bxcan_get_id();
                            send_msg.tx_dlen = 3;
                            send_msg.tx_data[0] = CAN_485; /* �豸���� */
                            send_msg.tx_data[1] = 0xf0; /* ���� */
                            send_msg.tx_data[2] = 0x00; /* �̵���״̬ */
                            bxcan_send(send_msg);
                            online = 0;/* �����豸���� */
                        } break;
                        case RADIO_SET_BR: { /* ��Ҫ�޸Ĳ������� */
                            task_set_br(rx_message.rx_data[1]);
                        } break;
                    }
                } break;
                default: {
                    if(rx_message.rx_sfid == bxcan_get_id()) { /* �жϵ�ַ */
                        switch(rx_message.rx_data[0]) {
                            case CAN_485 :{ /* �����豸 */
                                uint8_t result;
                                uint16_t  Input_Result;
                                if(rx_message.rx_data[1] == 0x10) {
                                    switch(rx_message.rx_data[4]) {
                                        case 1 : {
                                            result = ModbusMaster_readCoils(rx_message.rx_data[2],rx_message.rx_data[3], 1);
                                        } break;
                                        case 2 : {
                                            result = ModbusMaster_readDiscreteInputs(rx_message.rx_data[2],rx_message.rx_data[3], 1);
                                        } break;
                                        case 3 : {
                                            result = ModbusMaster_readHoldingRegisters(rx_message.rx_data[2],rx_message.rx_data[3], 1);
                                        } break;
                                        case 4 : {
                                            result = ModbusMaster_readInputRegisters(rx_message.rx_data[2],rx_message.rx_data[3], 1);
                                        } break;
                                    }
                                    if (result == 0x00) {
                                        Input_Result = ModbusMaster_getResponseBuffer(0x00);
                                    }
                                    can_trasnmit_message_struct send_msg;
                                    send_msg.tx_sfid = bxcan_get_id();
                                    send_msg.tx_dlen = 4;
                                    send_msg.tx_data[0] = CAN_485; /* �豸���� */
                                    send_msg.tx_data[1] = 0xf1; /* ���� */
                                    send_msg.tx_data[2] = Input_Result; /* �̵���״̬ */
                                    send_msg.tx_data[3] = Input_Result >> 8; /* �̵���״̬ */
                                    bxcan_send(send_msg);
                                } else {
                                    switch(rx_message.rx_data[4]) {
                                        case 1 : {
                                            result = ModbusMaster_writeSingleCoil(rx_message.rx_data[2],rx_message.rx_data[3], rx_message.rx_data[5]);
                                        } break;
                                        case 2 : {
                                            result = ModbusMaster_writeSingleRegister(rx_message.rx_data[2],rx_message.rx_data[3], rx_message.rx_data[5] | rx_message.rx_data[6] << 8);
                                        } break;
                                        case 3 : {
                                            result = ModbusMaster_writeSingleRegister(rx_message.rx_data[2],rx_message.rx_data[3], rx_message.rx_data[5] | rx_message.rx_data[6] << 8 );
                                        } break;
                                        case 4 : {
                                            result = ModbusMaster_writeMultipleCoils(rx_message.rx_data[2],rx_message.rx_data[3], rx_message.rx_data[5] | rx_message.rx_data[6] << 8 );
                                        } break;
                                    }
                                }
                            } break;
                            case 1: { /* �޸��豸��ַ */
                                task_set_address(rx_message.rx_data[1]);
                            } break;
                        }
                        online = 0;/* �����豸���� */
                    }
                }
            }
        }
    }
}

static void task_can_val(void *pvParameters) {
    for( ;; ) {
        vTaskDelay(20 / portTICK_RATE_MS);
        uint8_t val = 0x00;
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
