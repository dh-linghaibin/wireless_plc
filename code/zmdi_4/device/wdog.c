/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#include "wdog.h"
/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
/* 私有动作宏 *****************************************************************/
/* 私有变量 *******************************************************************/
/* 全局变量 */
/* 私有函数声明 ***************************************************************/
/* 私有函数  ******************************************************************/
/* 变量 ----------------------------------------------------------------------*/
void wdog_init(void) {
    /* 检测系统是否由独立看门狗喂狗复位 */
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) { 
        /* 清除复位标志位 */
        RCC_ClearFlag();
    }
    /* 使能写访问IWDG_PR and IWDG_RLR 寄存器 */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    /* IWDG计数器的时钟: LSI/32 */
    IWDG_SetPrescaler(IWDG_Prescaler_32);
    /* 设置重导计数器的值 获得250ms的IWDG超时可以按下面算式计算：  
    Counter Reload Value = 250ms/IWDG counter clock period
    = 250ms / (LSI/32)
    = 0.25s / (LsiFreq/32)
    = LsiFreq/(32 * 4)
    = LsiFreq/128
    */
    IWDG_SetReload(0x0fff);
    /* 从新导入IWDG计数器 */
    IWDG_ReloadCounter();
    /* 使能 IWDG (LSI 被硬件使能) */
    IWDG_Enable();
}

void wdog_reload(void) {
    /* 从新导入IWDG计数器 */
    IWDG_ReloadCounter(); 
}
/***************************************************************END OF FILE****/
