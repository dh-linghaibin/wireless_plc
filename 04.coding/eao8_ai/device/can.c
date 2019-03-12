/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#include "can.h"
#include "analog_quantity.h"
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
    nvic_irq_enable(CAN0_EWMC_IRQn,0,2);
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
}

static uint8_t can_id = 0x06;

void bxcan_init(uint16_t br) {
    /* configure GPIO */
    can_gpio_config();
    /* configure NVIC */
    nvic_config();
    
    bxcan_set_br(br);
    
    /* enable can receive FIFO0 not empty interrupt */
    can_interrupt_enable(CAN0, CAN_INT_RFNE0);
    can_interrupt_enable(CAN0, CAN_INT_RFO0);
    
    can_interrupt_enable(CAN0, CAN_INT_ERR | CAN_INT_WERR);// | CAN_INT_PERR
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
    can_id = id;
}

void bxcan_set_br(uint16_t br) {
    can_parameter_struct can_init_parameter;
    can_filter_parameter_struct can_filter_parameter;
    can_init_parameter.time_triggered = DISABLE;
    can_init_parameter.auto_bus_off_recovery = DISABLE;
    can_init_parameter.auto_wake_up = DISABLE;
    can_init_parameter.auto_retrans = DISABLE;
    can_init_parameter.rec_fifo_overwrite = DISABLE;
    can_init_parameter.trans_fifo_order = DISABLE;
    can_init_parameter.working_mode = CAN_NORMAL_MODE;//CAN_SILENT_LOOPBACK_MODE CAN_NORMAL_MODE
    can_init_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
    
     /* initialize filter */ 
    can_filter_parameter.filter_number=0;
    can_filter_parameter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter_parameter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter_parameter.filter_list_high = 0;
    can_filter_parameter.filter_list_low = 0;
    can_filter_parameter.filter_mask_high = 0;
    can_filter_parameter.filter_mask_low = 0;
    can_filter_parameter.filter_fifo_number = CAN_FIFO0;
    can_filter_parameter.filter_enable = ENABLE;
    switch(br) {
        case 0:{//10k
            can_init_parameter.time_segment_1 = CAN_BT_BS1_8TQ;
            can_init_parameter.time_segment_2 = CAN_BT_BS2_1TQ;
            can_init_parameter.prescaler = 540;
        } break;
        case 1:{//20k
            can_init_parameter.time_segment_1 = CAN_BT_BS1_8TQ;
            can_init_parameter.time_segment_2 = CAN_BT_BS2_1TQ;
            can_init_parameter.prescaler = 270;
        } break;
        case 2:{//50k
            can_init_parameter.time_segment_1 = CAN_BT_BS1_8TQ;
            can_init_parameter.time_segment_2 = CAN_BT_BS2_1TQ;
            can_init_parameter.prescaler = 108;
        } break;
        case 3:{//125k
            can_init_parameter.time_segment_1 = CAN_BT_BS1_10TQ;
            can_init_parameter.time_segment_2 = CAN_BT_BS2_1TQ;
            can_init_parameter.prescaler = 36;
        } break;
        case 4:{//250k
            can_init_parameter.time_segment_1 = CAN_BT_BS1_16TQ;
            can_init_parameter.time_segment_2 = CAN_BT_BS2_1TQ;
            can_init_parameter.prescaler = 12;
        } break;
        case 5:{//500k
            can_init_parameter.time_segment_1 = CAN_BT_BS1_16TQ;
            can_init_parameter.time_segment_2 = CAN_BT_BS2_1TQ;
            can_init_parameter.prescaler = 6;
        } break;
        case 6:{//800k
            can_init_parameter.time_segment_1 = CAN_BT_BS1_15TQ;
            can_init_parameter.time_segment_2 = CAN_BT_BS2_1TQ;
            can_init_parameter.prescaler = 4;
        } break;
        case 7:{//1000k
            can_init_parameter.time_segment_1 = CAN_BT_BS1_16TQ;
            can_init_parameter.time_segment_2 = CAN_BT_BS2_1TQ;
            can_init_parameter.prescaler = 3;
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
    can_receive_message_struct receive_message;
    can_message_receive(CAN0, CAN_FIFO0, &receive_message);
    if( receive_message.rx_sfid == 0x0155 ) { /* ±ê¶¨ÆµµÀ */
        analog_quantity_calibration(receive_message.rx_data);
    } else {
        /* do your something */
    }
}

void USBD_HP_CAN0_TX_IRQHandler(void) {
    
}

void CAN0_EWMC_IRQHandler( void ) {
    
}

