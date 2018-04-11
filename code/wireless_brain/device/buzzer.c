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

static TimerHandle_t xtime_buzz; /* ������ */

void buzzer_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);  	//TIM14ʱ��ʹ��    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTFʱ��	

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM9); //GPIOF9����Ϊ��ʱ��14

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;           //GPIOF9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PF9

    TIM_TimeBaseStructure.TIM_Prescaler=499;  //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period=83;   //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

    TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);//��ʼ����ʱ��14

    //��ʼ��TIM14 Channel1 PWMģʽ	 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
    TIM_OC2Init(TIM9, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1

    TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���

    TIM_ARRPreloadConfig(TIM9,ENABLE);//ARPEʹ�� 

    TIM_Cmd(TIM9, ENABLE);  //ʹ��TIM14
    
    TIM_SetCompare2(TIM9,41);
    delay_ms(100);
    TIM_SetCompare2(TIM9,0);
}

// ��ʱ���ص�������ʽ
static void buzzer_callback( TimerHandle_t xTimer ) {
    uint32_t ulTimerID;
    configASSERT(xTimer);
    TIM_SetCompare2(TIM9,0);
    xTimerDelete( xTimer ,0 );
    xtime_buzz = NULL;
}

/* ʱ�� ���� */
void buzzer_set(uint16_t time,uint8_t fre) {
    // �ж϶�ʱ���Ƿ�������״̬
    if( xTimerIsTimerActive( xtime_buzz ) != pdFALSE ) {
        xtime_buzz = xTimerCreate("can_tic",         /* ��ʱ������ */
                                  time,             /* ��ʱ������,��λʱ�ӽ��� */
                                  pdTRUE,           /* ������ */
                                  (void *) 0,       /* ��ʱ��ID */
                                  buzzer_callback); /* ��ʱ���ص����� */

        if(xtime_buzz == NULL) {
            /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
        } else {
            /* ������ʱ����ϵͳ������ſ�ʼ����  0 ��ʾ������*/
            if(xTimerStart(xtime_buzz, 0) == pdPASS) {
                 TIM_SetCompare2(TIM9,41);
            }
        }
    }
}

