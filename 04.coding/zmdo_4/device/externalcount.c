/* 引用头文件 *****************************************************************/
#include "externalcount.h"
/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
/* 私有动作宏 *****************************************************************/
#define ADC1_DR_Address    0x40012440
/* 私有变量 *******************************************************************/
/* 全局变量 */
/* 私有函数声明 ***************************************************************/
/* 私有函数  ******************************************************************/
static void ExternalcountClock(GPIO_TypeDef* port){
    if (port == GPIOA)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    else if(port == GPIOB)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    else if(port == GPIOC)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    else if(port == GPIOD)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
    else if(port == GPIOE)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
    else if(port == GPIOF)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
}

static void ExternalcountAf(Stdexternalcount_pin_TypeDef P) {
    uint8_t af_num = 0;
    switch(P.pin) {
        case GPIO_Pin_0:
            af_num = 0;
        break;
        case GPIO_Pin_1:
            af_num = 1;
        break;
        case GPIO_Pin_2:
            af_num = 2;
        break;
        case GPIO_Pin_3:
            af_num = 3;
        break;
        case GPIO_Pin_4:
            af_num = 4;
        break;
        case GPIO_Pin_5:
            af_num = 5;
        break;
        case GPIO_Pin_6:
            af_num = 6;
        break;
        case GPIO_Pin_7:
            af_num = 7;
        break;
        case GPIO_Pin_8:
            af_num = 8;
        break;
    }
    if(P.port == GPIOA) {
        GPIO_PinAFConfig(P.port,af_num,GPIO_AF_2);
    } else if(P.port == GPIOB) {
        GPIO_PinAFConfig(P.port,af_num,GPIO_AF_1);
    }
}

void ExternalcountInit(Stdexternalcount_n* externalcount) {
    GPIO_InitTypeDef    GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    ExternalcountClock(externalcount->XC1.port);
    ExternalcountClock(externalcount->XC2.port);
    ExternalcountClock(externalcount->XC3.port);
    ExternalcountClock(externalcount->XC4.port);
    ExternalcountClock(externalcount->XC5.port);
    ExternalcountClock(externalcount->XC6.port);
    ExternalcountClock(externalcount->XC7.port);
    ExternalcountClock(externalcount->XC8.port);
    
    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    
    GPIO_InitStructure.GPIO_Pin = externalcount->XC1.pin;
    GPIO_Init(externalcount->XC1.port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = externalcount->XC2.pin;
    GPIO_Init(externalcount->XC2.port, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = externalcount->XC3.pin;
    GPIO_Init(externalcount->XC3.port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = externalcount->XC4.pin;
    GPIO_Init(externalcount->XC4.port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = externalcount->XC5.pin;
    GPIO_Init(externalcount->XC5.port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = externalcount->XC6.pin;
    GPIO_Init(externalcount->XC6.port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = externalcount->XC7.pin;
    GPIO_Init(externalcount->XC7.port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = externalcount->XC8.pin;
    GPIO_Init(externalcount->XC8.port, &GPIO_InitStructure);
    //打开端口复用
    ExternalcountAf(externalcount->XC1);
    ExternalcountAf(externalcount->XC2);
    ExternalcountAf(externalcount->XC3);
    ExternalcountAf(externalcount->XC4);
    ExternalcountAf(externalcount->XC5);
    ExternalcountAf(externalcount->XC6);
    ExternalcountAf(externalcount->XC7);
    ExternalcountAf(externalcount->XC8);
    
    
    TIM_TimeBaseStructure.TIM_Prescaler = 170 - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 0xffffffff;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_TimeBaseStructure.TIM_Prescaler = 170 - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 0xffffffff;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    /* Enable the TIM1 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
     /* Enable the TIM1 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* TIM1 configuration: Input Capture mode ---------------------
    The external signal is connected to TIM1 CH2 pin (PA.09)  
    The Rising edge is used as active edge,
    The TIM1 CCR2 is used to compute the frequency value 
    ------------------------------------------------------------ */
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
    
    /* TIM enable counter */
    TIM_Cmd(TIM2, ENABLE);
    
    /* Enable the CC2 Interrupt Request */
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
    
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    /* TIM enable counter */
    TIM_Cmd(TIM3, ENABLE);
    
    /* Enable the CC2 Interrupt Request */
    TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
    TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
    TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
    TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);
    
    /** -- 定时器 -- by lhb_steven -- 2017/7/6**/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseStructure.TIM_Period = 8000-1;
    TIM_TimeBaseStructure.TIM_Prescaler = 45000-1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    
    TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
    
    TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
    TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);
    NVIC_SetPriority(TIM16_IRQn, 0);
    
    TIM_Cmd(TIM16, ENABLE);
}

static uint32_t electric_current[8];
static uint32_t Rc_Pwm_In[8];

void ExternalcountRead(Stdexternalcount_n* externalcount,uint8_t num) {
    externalcount->XC1.val = electric_current[0]*576/1193;
    externalcount->XC2.val = electric_current[1]*576/1193;
    externalcount->XC3.val = electric_current[2]*576/1193;
    externalcount->XC4.val = electric_current[3]*576/1193;
    externalcount->XC5.val = electric_current[4]*576/1193;
    externalcount->XC6.val = electric_current[5]*576/1193;
    externalcount->XC7.val = electric_current[6]*576/1193;
    externalcount->XC8.val = electric_current[7]*576/1193;
}

void TIM16_IRQHandler(void) {
    TIM_ClearITPendingBit(TIM16,TIM_IT_Update);
    for(uint8_t i = 0;i < 8;i++) {
        electric_current[i] = Rc_Pwm_In[i];
    }
    for(uint8_t i = 0;i < 8;i++) {
        Rc_Pwm_In[i] = 0;
    }
}

void TIM3_IRQHandler(void) {
    if(TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET) {
        /* Clear TIM1 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
        Rc_Pwm_In[0]++;
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == 1) {
            TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);
        } else {
            TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising);
        }
    } else if(TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
        Rc_Pwm_In[1]++;
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == 1) {
            TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);
        } else {
            TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising);
        }
    } else if(TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
        Rc_Pwm_In[2]++;
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 1) {
            TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);
        } else {
            TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising);
        }
    } else if(TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
        Rc_Pwm_In[3]++;
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 1) {
            TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Falling);
        } else {
            TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Rising);
        }
    }
}


void TIM2_IRQHandler(void) {       
    if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET) {
        /* Clear TIM1 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        Rc_Pwm_In[4]++;
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1) {
            TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);
        } else {
            TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising);
        }
    } else if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET) {
        /* Clear TIM1 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
        Rc_Pwm_In[5]++;
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1) {
            TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);	
        } else {
            TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); 
        }
    } else if(TIM_GetITStatus(TIM2, TIM_IT_CC3) == SET) {
        /* Clear TIM1 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
        Rc_Pwm_In[6]++;
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2) == 1) {
            TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Falling);
        } else {
            TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising);
        }
    } else if(TIM_GetITStatus(TIM2, TIM_IT_CC4) == SET) {
        /* Clear TIM1 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
        Rc_Pwm_In[7]++;
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3) == 1) {
            TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Falling);
        } else {
            TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Rising);
        }
    }
}
/***************************************************************END OF FILE****/