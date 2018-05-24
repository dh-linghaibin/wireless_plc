/* 引用头文件 *****************************************************************/
#include "sys.h"
/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
/* 私有动作宏 *****************************************************************/
/* 私有变量 *******************************************************************/
/* 全局变量 */
/* 私有函数声明 ***************************************************************/
/* 私有函数  ******************************************************************/

/** -- 设置系统内部时钟为48M -- by lhb_steven -- 2017/6/17**/
void SysInitRcc(void) {
    RCC_DeInit();  
    /* enable HSI */  
    RCC_HSICmd(ENABLE);  
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)  
    {}  
    
    /* Enable Prefetch Buffer */  
    FLASH_PrefetchBufferCmd(ENABLE);  
    /* set Flash Latency, 0-24M: FLASH_Latency_0, 24M < sysclk <48M: FLASH_Latency_1*/  
    FLASH_SetLatency(FLASH_Latency_1);  
    
    /* Configures the AHB clock (HCLK)  */  
    RCC_HCLKConfig(RCC_CFGR_HPRE_DIV1);  
    
    /* Configures the APB clock (PCLK) */  
    RCC_PCLKConfig(RCC_CFGR_PPRE_DIV1);  
    
    /* Configures PLL source, (8M/2) * 12 = 48M */  
    RCC_PLLConfig(RCC_CFGR_PLLSRC_HSI_DIV2, RCC_CFGR_PLLMULL12);   
    RCC_PLLCmd(ENABLE);  
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)  
    {}  
    
    /* Configures system clock source */  
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  
    /* Wait till PLL is used as system clock source */  
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)  
    {}  
}
/***************************************************************END OF FILE****/
