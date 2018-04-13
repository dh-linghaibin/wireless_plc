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
static xQueueHandle can_msg_queue = NULL; /*���о��*/
static TimerHandle_t xtime_can; /* ������ */
static uint8_t online = 25; /* �豸״̬��־ */

void task_can_init(void) {
    
}

void task_can_create(void) {
    xTaskCreate( task_can,"task_can", 100, NULL, tskIDLE_PRIORITY+3, NULL );
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

static CanRxMsg rx_message;
static void task_can(void *pvParameters) {
    /* �������� */  
    can_msg_queue = xQueueCreate( 5 , sizeof( CanRxMsg ) );
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
    for( ;; ) {
        //vTaskDelay(20 / portTICK_RATE_MS);
        if( xQueueReceive( can_msg_queue, &rx_message, 100/portTICK_RATE_MS ) == pdPASS) {
            switch(rx_message.StdId) {
                case RADIO: { /* �㲥 */
                    switch(rx_message.Data[0]) {
                        case RADIO_ASK:{
                            CanTxMsg send_msg;
                            send_msg.StdId = bxcan_get_id();
                            send_msg.DLC = 3;
                            send_msg.Data[0] = DI_4; /* �豸���� */
                            send_msg.Data[1] = 0xf0; /* ���� */
                            send_msg.Data[2] = insignal_read(8); /* �̵���״̬ */
                            bxcan_send(send_msg);
                            online = 0;/* �����豸���� */
                        } break;
                        case RADIO_SET_BR: { /* ��Ҫ�޸Ĳ������� */
                            task_set_br(rx_message.Data[1]);
                        } break;
                    }
                } break;
                default: {
                    if(rx_message.StdId == bxcan_get_id()) { /* �жϵ�ַ */
                        switch(rx_message.Data[0]) {
                            case 0:{ /* �����豸 */
                                CanTxMsg send_msg;
                                send_msg.StdId = bxcan_get_id();
                                send_msg.DLC = 3;
                                send_msg.Data[0] = DI_4; /* �豸���� */
                                send_msg.Data[1] = 0xf0; /* ���� */
                                send_msg.Data[2] = insignal_read(rx_message.Data[1]); /* �̵���״̬ */
                                bxcan_send(send_msg);
                            } break;
                            case 1: { /* �޸��豸��ַ */
                                task_set_address(rx_message.Data[1]);
                            } break;
                        }
                        online = 0;/* �����豸���� */
                    }
                }
            }
        }
    }
}

