/* 引用头文件 *****************************************************************/
#include "timer.h"
/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
/* 私有动作宏 *****************************************************************/
/* 私有变量 *******************************************************************/
__IO uint32_t sys_heartbeat = 0;
/* 全局变量 */
/* 私有函数声明 ***************************************************************/
void (*updateCallback[8])(void); // 7 + 1 basic timer
/* 私有函数  ******************************************************************/

void TimerInit(void) {
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseStructure.TIM_Period = 40-1;
    TIM_TimeBaseStructure.TIM_Prescaler = 1000-1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
    
    TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
    TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);
    NVIC_SetPriority(TIM14_IRQn, 0);
    
    TIM_Cmd(TIM14, ENABLE);
}

uint32_t TimerGetClock(void) {
    return sys_heartbeat;
}


void TIM14_IRQHandler(void) {
    sys_heartbeat++;
    // Callback
    if(updateCallback[0] != 0) {
        (*updateCallback[0])();
    }
    TIM_ClearITPendingBit(TIM14,TIM_IT_Update);
}
/***************************************************************END OF FILE****/
