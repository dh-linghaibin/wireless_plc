/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#include "can.h"
#include "flash.h"
#include "wdog.h"
#include "delay.h"
#include "task_can.h"
#include "task_set.h"
#include "wdog.h"
#include "digital_tube.h"
#include "button.h"
#include "task_modbus.h"
#include "rs485.h"
#include "modbus_master.h"

//uint16_t  Input_Resul2[2];


//static void nvic_configuration(void)
//{
//    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
//    nvic_irq_enable(TIMER2_IRQn, 1, 2);
//}

//void timer_config(void) {
//    /* -----------------------------------------------------------------------
//    TIMER0 configuration:
//    generate 3 complementary PWM signal.
//    TIMER0CLK is fixed to systemcoreclock, the TIMER0 prescaler is equal to 108 
//    so the TIMER0 counter clock used is 1MHz.
//    insert a dead time equal to (64 + 36) * 2 / systemcoreclock =1.85us 
//    configure the break feature, active at low level, and using the automatic
//    output enable feature.
//    use the locking parameters level 0.
//    ----------------------------------------------------------------------- */
//    //timer_oc_parameter_struct timer_ocintpara;
//    timer_parameter_struct timer_initpara;
//    //timer_break_parameter_struct timer_breakpara;

//    rcu_periph_clock_enable(RCU_TIMER2);

//    timer_deinit(TIMER2);

//    /* TIMER0 configuration */
//    timer_initpara.prescaler         = 375 - 1;
//    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
//    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
//    timer_initpara.period            = 1000;
//    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
//    timer_initpara.repetitioncounter = 0;
//    timer_init(TIMER2,&timer_initpara);

//    /* TIMER0 channel control update interrupt enable */
//    timer_interrupt_enable(TIMER2,TIMER_INT_UP);

//    /* TIMER0 counter enable */
//    timer_enable(TIMER2);
//}

int main(void) {
    delay_init();
    delay_ms(100);
    digital_tube_init();
    button_init();
    wdog_init();/* 看门狗初始化 */
    task_set_init();
    task_can_init();
    task_modbus_init();
    task_can_create();
    task_set_create();
    task_modbus_create();
    /* Start the scheduler. */
    vTaskStartScheduler();
    while(1);
}

//void TIMER2_IRQHandler(void) {
//    /* clear TIMER interrupt flag */
//    timer_interrupt_flag_clear(TIMER2,TIMER_INT_UP);
//}


