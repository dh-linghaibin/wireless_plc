/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "rs485.h"
#include "delay.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "modbus_master.h"

#define RS485_SEND gpio_bit_set(GPIOC, GPIO_PIN_13)
#define RS485_RECE gpio_bit_reset(GPIOC, GPIO_PIN_13)

void rs485_init(uint8_t br) {
    rcu_periph_clock_enable( RCU_GPIOA );
    rcu_periph_clock_enable( RCU_GPIOC );
    rcu_periph_clock_enable( RCU_USART0 );
    
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_13);
    
    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* USART configure */
    usart_deinit(USART0);
    if(br == 0) {
        usart_baudrate_set(USART0, 9600u);
    } else {
        usart_baudrate_set(USART0, 115200u);
    }
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    /* USART interrupt configuration */
    nvic_irq_enable(USART0_IRQn, 0, 0);
    /* enable EVAL_COM1 receive interrupt */
    usart_interrupt_enable(USART0, USART_INT_RBNE );  //开启接收中断
    usart_enable(USART0);
    
    RS485_RECE;
}

uint8_t rs485_send(uint8_t * buf, uint8_t len) {
    taskENTER_CRITICAL();//进入〇接
    usart_receive_config(USART0, USART_RECEIVE_DISABLE);
    RS485_SEND;
    for(int i = 0;i < len;i++) {
        usart_data_transmit(USART0, buf[i]);
        while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    }
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    taskEXIT_CRITICAL();
    vTaskDelay(1 / portTICK_RATE_MS);
    //delay_us(550);
    RS485_RECE;
    return HAL_OK;
}

#include "trans_recieve_buff_control.h"

uint8_t rs485_get(uint8_t * buf, uint8_t len) {
    buf[0] = usart_data_receive(USART0);
    return HAL_OK;
}

void USART0_IRQHandler() {
    if(RESET != usart_flag_get(USART0, USART_FLAG_RBNE)) {
        /* receive data */
        Modbus_Master_Rece_Handler();
        usart_flag_clear(USART0 , USART_FLAG_RBNE);
    }
}

