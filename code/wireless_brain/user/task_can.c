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
static xQueueHandle can_msg_queue = NULL; /*���о��*/
static xQueueHandle can_send_queue = NULL; /*���о��*/
static TimerHandle_t xtime_can; /* ������ */
static l_list_t * online_head; /* ����ͷ */

static void up_device(device_online online) {
    device_online *temp;
    for (int i = 0; i < list_len(online_head); i++){ //list_len��ȡ����ĳ���
        list_get(online_head, i, (void **)&temp); //ȡ��λ��Ϊi�Ľ�������
        //printf("%d\n", temp->type);
        if( (temp->type == online.type) && (temp->address == online.address) ) { /* �ж��豸�Ƿ���뵱ǰ���� */
            temp->online = 0;/* ˵���豸���� */
            return;
        }
    }
    /* �����豸 */
    device_online * device;
    device = l_malloc(sizeof(device_online));
    device->type    = online.type;
    device->address = online.address;
    device->online  = 0;/* ˵���豸���� */
    list_append(online_head, device); //׷�ӽ��
}

void task_can_init(void) {
    online_head = (l_list_t *)l_malloc(sizeof(l_list_t)); /* ����ͷ�ڵ� */
    static device_online * online;
    list_init(online_head); //��ʼ������
    //can1_init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_Normal); /* ��ʼ��can 500k */
    uint8_t but = 0;
    persistence_get_can_adr(&but);
    can1_init(but,CAN_Mode_Normal); /* ��ʼ��can 50k */
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

// ��ʱ���ص�������ʽ
static void vtimer_callback( TimerHandle_t xTimer ) {
    uint32_t ulTimerID;
    configASSERT(xTimer);
    //printf("time \n");
    device_online *temp;
    for (int i = 0; i < list_len(online_head); i++){ //list_len��ȡ����ĳ���
        list_get(online_head, i, (void **)&temp); //ȡ��λ��Ϊi�Ľ�������
        if(temp->online > 4) { /* �豸��ʱδ��Ӧ ɾ���豸 */
            list_pop(online_head,i);
            l_free(temp);
        } else {
            //printf("�豸����:%d  �豸��ַ:%d �豸����:%d\n", temp->type,temp->address,temp->online);
            temp->online ++;
        }
    }
    /* �㲥 */
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
    /* �������� */  
    can_msg_queue = xQueueCreate( 20 , sizeof( CanRxMsg ) );
    xtime_can = xTimerCreate("can_tic",         /* ��ʱ������ */
                              2000,             /* ��ʱ������,��λʱ�ӽ��� */
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
    for( ;; ) {
        if( xQueueReceive( can_msg_queue, &rx_message, 100/portTICK_RATE_MS ) == pdPASS) {
            switch(rx_message.Data[0]) {
                case DO_8: {
                    switch(rx_message.Data[1]) {
                        case 0xf0: { /* ��ȡ */
                            //printf("DO-8 ��ַ %d ���� %d\n",rx_message.StdId,rx_message.Data[2]);
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
                        case 0xf0: { /* ��ȡ */
                            //printf("DO-4 ��ַ %d ���� %d\n",rx_message.StdId,rx_message.Data[2]);
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
                         case 0xf0: { /* ��ȡ */
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
        xQueueSendFromISR( can_send_queue, &send_msg, 0 ); /* 0����˼������������� */
    }
}

static void task_can_send(void *pvParameters) {
    device_send send_msg;
    /* �������� */  
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

