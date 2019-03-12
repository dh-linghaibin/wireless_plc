/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "timer.h"
#include "multi_timer.h"

static void timer_config(void);


void timer_init_l(void) {
    nvic_irq_enable(TIMER0_UP_IRQn, 1, 1);
    timer_config();
}

/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
void timer_config(void)
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
    timer_break_parameter_struct timer_breakpara;

    rcu_periph_clock_enable(RCU_TIMER0);

    timer_deinit(TIMER0);

    /* TIMER0 configuration */
    timer_initpara.prescaler         = 1079;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 9;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV4;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0,&timer_initpara);
    
    timer_update_event_enable(TIMER0);
    timer_interrupt_enable(TIMER0,TIMER_INT_UP);
    timer_flag_clear(TIMER0, TIMER_FLAG_UP);
    timer_update_source_config(TIMER0, TIMER_UPDATE_SRC_GLOBAL);
    
    /* TIMER0 counter enable */
    timer_enable(TIMER0);
}

/*!
    \brief      this function handles Timer0 updata interrupt request.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER0_UP_IRQHandler(void) {
    if (RESET != timer_flag_get(TIMER0, TIMER_FLAG_UP))    {
        timer_flag_clear(TIMER0, TIMER_FLAG_UP);
        mtimer_ticks();
        //timer_disable(TIMER0);
    }
}
