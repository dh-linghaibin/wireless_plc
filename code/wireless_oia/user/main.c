/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "gd32f10x.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "semphr.h"
#include "croutine.h"

#include "tm1650.h"

void vTaskLED(void *pvParameters);
void vTaskLEDFlash(void *pvParameters);

static Stdtm1650 TM1650 = {
	{
		{GPIOB,GPIO_PIN_6},
		{GPIOB,GPIO_PIN_7},
		{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D,
		0x07, 0x7F, 0x6F, 0x76, 0x40,0x79, 0x00},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
		{0x00,0x00,0x00,0x00},
	},
	&Tm1650Init,
	&Tm1650ShowNex,
	&Tm1650ShowLed,
	&TM1650ScanKey,
};

/**
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
  */
void gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);

    /*Configure PA1 PA2 PA3(TIMER1 CH1 CH2 CH3) as alternate function*/
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
}

/**
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
void timer_config(void)
{
    /* -----------------------------------------------------------------------
    TIMER1 configuration: generate 3 PWM signals with 3 different duty cycles:
    TIMER1CLK = SystemCoreClock / 108 = 1MHz

    TIMER1 channel1 duty cycle = (4000/ 16000)* 100  = 25%
    TIMER1 channel2 duty cycle = (8000/ 16000)* 100  = 50%
    TIMER1 channel3 duty cycle = (12000/ 16000)* 100 = 75%
    ----------------------------------------------------------------------- */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER1);

    timer_deinit(TIMER1);

    /* TIMER1 configuration */
    timer_initpara.prescaler         = 107;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 15999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);

    /* CH1,CH2 and CH3 configuration in PWM mode1 */
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER1,TIMER_CH_1,&timer_ocintpara);
    timer_channel_output_config(TIMER1,TIMER_CH_2,&timer_ocintpara);
    timer_channel_output_config(TIMER1,TIMER_CH_3,&timer_ocintpara);

    /* CH1 configuration in PWM mode1,duty cycle 25% */
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_1,16000);
    timer_channel_output_mode_config(TIMER1,TIMER_CH_1,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);

    /* CH2 configuration in PWM mode1,duty cycle 50% */
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_2,16000);
    timer_channel_output_mode_config(TIMER1,TIMER_CH_2,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);

    /* CH3 configuration in PWM mode1,duty cycle 75% */
    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_3,16000);
    timer_channel_output_mode_config(TIMER1,TIMER_CH_3,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_3,TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER1);
    /* auto-reload preload enable */
    timer_enable(TIMER1);
}

int main(void) {
	rcu_periph_clock_enable(RCU_GPIOA);

	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_4);
	gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_5);

	gpio_init(GPIOC,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_14);
	gpio_init(GPIOC,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_15);
	
	gpio_bit_write(GPIOC,GPIO_PIN_14,RESET);
	gpio_bit_write(GPIOC,GPIO_PIN_15,RESET);
	
	gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_0);
	gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_1);
	gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_2);
	gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_3);

	gpio_bit_write(GPIOA,GPIO_PIN_0,SET);
	gpio_bit_write(GPIOA,GPIO_PIN_1,SET);
	gpio_bit_write(GPIOA,GPIO_PIN_2,SET);
	gpio_bit_write(GPIOA,GPIO_PIN_3,SET);
	gpio_bit_write(GPIOB,GPIO_PIN_5,SET);

	TM1650.init(&TM1650.tm1650_n); 			/* 显示初始化 */
	TM1650.show_nex(&TM1650.tm1650_n,0,8);/* 显示设备地址 */
	TM1650.show_nex(&TM1650.tm1650_n,1,8);/* 显示设备地址 */

	gpio_config();
    timer_config();

	xTaskCreate(vTaskLED, "Task LED", 10, NULL, 1, NULL);    
	xTaskCreate(vTaskLEDFlash, "Task LED Flash", 10, NULL, 2, NULL);
    vTaskStartScheduler();
    while (1) {       
    }
}

/**
  * @brief  LED task, turn on all the LED one by one and then turn off them.
  * @param  pvParameters: passing parameter when the task is created
  * @retval None
  */
void vTaskLED(void *pvParameters) {
	uint16_t val = 16000;
    while (1) {
        /* Turn on the LED one by one */
		//gpio_bit_write(GPIOA,GPIO_PIN_3,SET);
        vTaskDelay(1 / portTICK_RATE_MS);
		if(val == 0) {
			val = 16000;
		} else {  
			val -= 10;
		}
		timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_3,val);
		//gpio_bit_write(GPIOA,GPIO_PIN_3,RESET);
        //vTaskDelay(500 / portTICK_RATE_MS);
    }
}

void vTaskLEDFlash(void *pvParameters) {
    while (1) {
        /* Turn on the LED */
        vTaskDelay(500 / portTICK_RATE_MS);
        vTaskDelay(500 / portTICK_RATE_MS);
    }
}

