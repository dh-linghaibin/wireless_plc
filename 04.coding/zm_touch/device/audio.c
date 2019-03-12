/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "audio.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/**
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
  */
static void nvic_configuration(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(TIMER2_IRQn, 1, 2);
}

//timer_oc_parameter_struct timer_ocintpara;
timer_parameter_struct timer_initpara;
/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
void timer_config(void) {
    /* -----------------------------------------------------------------------
    TIMER0 configuration:
    generate 3 complementary PWM signal.
    TIMER0CLK is fixed to systemcoreclock, the TIMER0 prescaler is equal to 108 
    so the TIMER0 counter clock used is 1MHz.
    insert a dead time equal to (64 + 36) * 2 / systemcoreclock =1.85us 
    configure the break feature, active at low level, and using the automatic
    output enable feature.
    use the locking parameters level 0.
    ----------------------------------------------------------------------- */
    
    //timer_break_parameter_struct timer_breakpara;

    rcu_periph_clock_enable(RCU_TIMER2);

    timer_deinit(TIMER2);

    /* TIMER0 configuration */
    timer_initpara.prescaler         = 1080 - 1;//((1+TIM_Prescaler )/72M)*(1+TIM_Period )=((1+7199)/72M)*(1+9999)=1秒*/
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 60;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);

    /* TIMER0 channel control update interrupt enable */
    timer_interrupt_enable(TIMER2,TIMER_INT_UP);

    /* TIMER0 counter enable */
    //timer_enable(TIMER2);
}

 //红尘情歌
uint8_t music[]={5,5,6,8,7,6,5,6,13,13,//音调
                5,5,6,8,7,6,5,3,13,13,
                2,2,3,5,3,5,6,3,2,1,
                6,6,5,6,5,3,6,5,13,13,

                5,5,6,8,7,6,5,6,13,13,
                5,5,6,8,7,6,5,3,13,13,
                2,2,3,5,3,5,6,3,2,1,
                6,6,5,6,5,3,6,1,    

                13,8,9,10,10,9,8,10,9,8,6,
                13,6,8,9,9,8,6,9,8,6,5,
                13,2,3,5,5,3,5,5,6,8,7,6,
                6,10,9,9,8,6,5,6,8};    
uint8_t time[] = {2,4,2,2,2,2,2,8,4, 4, //时间
                2,4,2,2,2,2,2,8,4, 4, 
                2,4,2,4,2,2,4,2,2,8,
                2,4,2,2,2,2,2,8,4 ,4, 

                2,4,2,2,2,2,2,8,4, 4, 
                2,4,2,2,2,2,2,8,4, 4, 
                2,4,2,4,2,2,4,2,2,8,
                2,4,2,2,2,2,2,8,

                4, 2,2,2, 4, 2,2,2, 2,2,8,
                4, 2,2,2,4,2,2,2,2,2,8,
                4, 2,2,2,4,2,2,5,2,6,2,4,
                2,2 ,2,4,2,4,2,2,12};   

void audio_init(void) {
    rcu_periph_clock_enable(RCU_GPIOB);

    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_9);

    gpio_bit_write(GPIOB,GPIO_PIN_9,RESET);
    
    nvic_configuration();
    timer_config();
    
    static uint16_t ci = 0;
//    while(1) {
//        //static uint16_t hz = 60;
//        delay_ms(time[ci]*100);
//        timer_initpara.period = music[ci]*10;
//        timer_init(TIMER2,&timer_initpara);
//        if(ci < sizeof(music)/sizeof(music[0])) {
//            ci++;
//        } else {
//            ci = 0;
//        }    
//    }
}

void audio_set(uint8_t mode) {
    switch(mode) {
        case 0: {
            timer_enable(TIMER2);
            //for(int x_i = 0; x_i < 1; x_i++) {
                timer_initpara.period = 40;
                timer_init(TIMER2,&timer_initpara);
                vTaskDelay(30 / portTICK_RATE_MS);
           // }
            timer_disable(TIMER2);
            vTaskDelay(1 / portTICK_RATE_MS);
            gpio_bit_write(GPIOB,GPIO_PIN_9,RESET);
        } break;
    }
}

void TIMER2_IRQHandler(void) {
    /* clear TIMER interrupt flag */
    timer_interrupt_flag_clear(TIMER2,TIMER_INT_UP);
    static FlagStatus dr = SET;
    dr = !dr;
    gpio_bit_write(GPIOB,GPIO_PIN_9,dr);
}
