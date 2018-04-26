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

static void task_can(void *pvParameters);
static xQueueHandle can_msg_queue = NULL; /*���о��*/
static TimerHandle_t xtime_can; /* ������ */
static uint8_t online = 25; /* �豸״̬��־ */

void task_can_init(void) {
    
}

void task_can_create(void) {
    xTaskCreate( task_can,"task_can", 512, NULL, tskIDLE_PRIORITY+2, NULL );
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
    for( ;; ) {
        if( xQueueReceive( can_msg_queue, &rx_message, 100/portTICK_RATE_MS ) == pdPASS) {
            switch(rx_message.rx_sfid) {
                case RADIO: { /* �㲥 */
                    switch(rx_message.rx_data[0]) {
                        case RADIO_ASK:{
                            can_trasnmit_message_struct send_msg;
                            send_msg.tx_sfid = bxcan_get_id();
                            send_msg.tx_dlen = 3;
                            send_msg.tx_data[0] = DI_4; /* �豸���� */
                            send_msg.tx_data[1] = 0xf0; /* ���� */
                            send_msg.tx_data[2] = insignal_read(8); /* �̵���״̬ */
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
                            case 0:{ /* �����豸 */
                                can_trasnmit_message_struct send_msg;
                                send_msg.tx_sfid = bxcan_get_id();
                                send_msg.tx_dlen = 3;
                                send_msg.tx_data[0] = DI_4; /* �豸���� */
                                send_msg.tx_data[1] = 0xf0; /* ���� */
                                send_msg.tx_data[2] = insignal_read(8); /* �̵���״̬ */
                                bxcan_send(send_msg);
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
