/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#include "can.h"
#include <cstring>
#include "task_can.h"
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
/*!
    \brief      initialize CAN and filter
    \param[in]  can_parameter
      \arg        can_parameter_struct
    \param[in]  can_filter
      \arg        can_filter_parameter_struct
    \param[out] none
    \retval     none
*/
static void can_config(can_parameter_struct can_parameter, can_filter_parameter_struct can_filter) {
    /* initialize CAN register */
    can_deinit(CAN0);
    
    /* initialize CAN */
    can_init(CAN0, &can_parameter);
    
    can_filter_init(&can_filter);
}

/*!
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void nvic_config(void) {
    /* configure CAN0 NVIC */
    nvic_irq_enable(USBD_LP_CAN0_RX0_IRQn,0,1);
    //nvic_irq_enable(USBD_HP_CAN0_TX_IRQn,0,2);
}

/*!
    \brief      configure GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void can_gpio_config(void) {
    /* enable CAN clock */
    rcu_periph_clock_enable(RCU_CAN0);
    rcu_periph_clock_enable(RCU_GPIOA);
    
    /* configure CAN0 GPIO */
    gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_11);
    gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_12);

    gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);

    gpio_bit_write(GPIOA,GPIO_PIN_10,RESET);
}

static uint8_t can_id = 0x06;

void bxcan_init(uint8_t br) {
    /* configure GPIO */
    can_gpio_config();
    /* configure NVIC */
    nvic_config();
    
    bxcan_set_br(br);
    
    /* enable can receive FIFO0 not empty interrupt */
    can_interrupt_enable(CAN0, CAN_INT_RFNE0);
    can_interrupt_enable(CAN0, CAN_INT_RFO0);
}

void bxcan_send(can_trasnmit_message_struct send_msg) {
    send_msg.tx_ft = CAN_FT_DATA;
    send_msg.tx_ff = CAN_FF_STANDARD;
    can_message_transmit(CAN0, &send_msg);
    uint32_t timeout = 0xFFFF;
    while((can_transmit_states(CAN0, CAN_MAILBOX0) != CAN_TRANSMIT_OK) && (timeout != 0)){
        timeout--;
    }
    timeout = 0xFFFF;
    while((can_transmit_states(CAN0, CAN_MAILBOX1) != CAN_TRANSMIT_OK) && (timeout != 0)){
        timeout--;
    }
    timeout = 0xFFFF;
    while((can_transmit_states(CAN0, CAN_MAILBOX2) != CAN_TRANSMIT_OK) && (timeout != 0)){
        timeout--;
    }
}

void bxcan_set_id(uint8_t id) {
    can_filter_parameter_struct can_filter_parameter;
    can_id = id;
    /* initialize filter */ 
    can_filter_parameter.filter_number=0;
    can_filter_parameter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter_parameter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter_parameter.filter_list_high = (can_id<<5);
    can_filter_parameter.filter_list_low = 0|0x0000;
    can_filter_parameter.filter_mask_high = ((0xf001<<3)>>16) & 0xffff;
    can_filter_parameter.filter_mask_low = ((0xf001<<3)& 0xffff) | 0x0004;
    can_filter_parameter.filter_fifo_number = CAN_FIFO0;
    can_filter_parameter.filter_enable = ENABLE;
    can_filter_init(&can_filter_parameter);
}

void bxcan_set_br(uint8_t br) {
    can_parameter_struct can_init_parameter;
    can_filter_parameter_struct can_filter_parameter;
    can_init_parameter.time_triggered = DISABLE;
    can_init_parameter.auto_bus_off_recovery = DISABLE;
    can_init_parameter.auto_wake_up = DISABLE;
    can_init_parameter.auto_retrans = DISABLE;
    can_init_parameter.rec_fifo_overwrite = DISABLE;
    can_init_parameter.trans_fifo_order = DISABLE;
    can_init_parameter.working_mode = CAN_NORMAL_MODE;
    can_init_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
    can_init_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
    can_init_parameter.time_segment_1 = CAN_BT_BS1_13TQ;
    can_init_parameter.time_segment_2 = CAN_BT_BS2_4TQ;
    can_init_parameter.prescaler = BR_50K;
    
     /* initialize filter */ 
    can_filter_parameter.filter_number=0;
    can_filter_parameter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter_parameter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter_parameter.filter_list_high = (can_id<<5);
    can_filter_parameter.filter_list_low = 0|0x0000;
    can_filter_parameter.filter_mask_high = ((0xf001<<3)>>16) & 0xffff;
    can_filter_parameter.filter_mask_low = ((0xf001<<3)& 0xffff) | 0x0004;
    can_filter_parameter.filter_fifo_number = CAN_FIFO0;
    can_filter_parameter.filter_enable = ENABLE;
    switch(br) {
        case 0:{
            can_init_parameter.prescaler = BR_10K;
        } break;
        case 1:{
            can_init_parameter.prescaler = BR_20K;
        } break;
        case 2:{
            can_init_parameter.prescaler = BR_50K;
        } break;
        case 3:{
            can_init_parameter.prescaler = BR_125K;
        } break;
        case 4:{
            can_init_parameter.prescaler = BR_250K;
        } break;
        case 5:{
            can_init_parameter.prescaler = BR_500K;
        } break;
        case 6:{
            can_init_parameter.time_segment_2 = CAN_BT_BS2_3TQ;
            can_init_parameter.prescaler = BR_800K;
        } break;
        case 7:{
            can_init_parameter.prescaler = BR_1000K;
        } break;
    }
    can_config(can_init_parameter, can_filter_parameter);
}

uint8_t bxcan_get_id(void) {
    return can_id;
}

/*!
    \brief      this function handles CAN0 RX0 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USBD_LP_CAN0_RX0_IRQHandler(void) {
    /* check the receive message */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;//中断中唤醒新任务
    //taskENTER_CRITICAL();//进入中断
    can_receive_message_struct receive_message;
    can_message_receive(CAN0, CAN_FIFO0, &receive_message);
    if(task_can_get_queue() != NULL) {
        xQueueSendFromISR( task_can_get_queue(), &receive_message, 0 );
    }
   // taskEXIT_CRITICAL();
    if( xHigherPriorityTaskWoken != pdFALSE ) {
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );//强制上下文切换
    }
}

void USBD_HP_CAN0_TX_IRQHandler(void) {
    
}



