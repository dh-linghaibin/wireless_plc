/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "buzzer.h"
#include "delay.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

static TimerHandle_t xtime_buzz; /* 定义句柄 */

void buzzer_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  	//TIM14时钟使能    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTF时钟	

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM9); //GPIOF9复用为定时器14

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;           //GPIOF9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PF9

    TIM_TimeBaseStructure.TIM_Prescaler=499;  //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period=83;   //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);//初始化定时器14

    //初始化TIM14 Channel1 PWM模式	 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性低
    TIM_OC2Init(TIM9, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

    TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器

    TIM_ARRPreloadConfig(TIM9,ENABLE);//ARPE使能 

    TIM_Cmd(TIM9, ENABLE);  //使能TIM14
    
    TIM_SetCompare2(TIM9,41);
    delay_ms(100);
    TIM_SetCompare2(TIM9,0);
}

// 定时器回调函数格式
static void buzzer_callback( TimerHandle_t xTimer ) {
    uint32_t ulTimerID;
    configASSERT(xTimer);
    TIM_SetCompare2(TIM9,0);
    xTimerDelete( xTimer ,0 );
    xtime_buzz = NULL;
}

/* 时间 次数 */
void buzzer_set(uint16_t time,uint8_t fre) {
    // 判断定时器是否处于运行状态
    if( xTimerIsTimerActive( xtime_buzz ) != pdFALSE ) {
        xtime_buzz = xTimerCreate("can_tic",         /* 定时器名字 */
                                  time,             /* 定时器周期,单位时钟节拍 */
                                  pdTRUE,           /* 周期性 */
                                  (void *) 0,       /* 定时器ID */
                                  buzzer_callback); /* 定时器回调函数 */

        if(xtime_buzz == NULL) {
            /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
        } else {
            /* 启动定时器，系统启动后才开始工作  0 表示不阻塞*/
            if(xTimerStart(xtime_buzz, 0) == pdPASS) {
                 TIM_SetCompare2(TIM9,41);
            }
        }
    }
}

