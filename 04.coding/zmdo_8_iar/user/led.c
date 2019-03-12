/* 引用头文件 *****************************************************************/
#include "led.h"
/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
/* 私有动作宏 *****************************************************************/
/* 私有变量 *******************************************************************/
/* 全局变量 */
/* 私有函数声明 ***************************************************************/
/* 私有函数  ******************************************************************/

void LedInit(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOA,GPIO_Pin_7);
    //GPIO_SetBits(GPIOA,GPIO_Pin_7);	
}

void LedSet(typeLedSet var) {
    GPIO_WriteBit(GPIOA, GPIO_Pin_7,(BitAction)var);
}

void LedTiger(void) {
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, 
		               (BitAction)((1-GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_7))));
}
/***************************************************************END OF FILE****/
