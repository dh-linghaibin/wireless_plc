/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "app_configure.h"
#include "usart.h"
#include "delay.h"
#include "sram.h"
#include "spi.h"
#include "tft.h"

#include "stdio.h"

  
u32 testsram[250000] __attribute__((at(0X68000000)));//测试用数组
//外部内存测试(最大支持1M字节内存测试)        
void fsmc_sram_test(u16 x,u16 y)
{  
    u32 i=0;        
    u8 temp=0;       
    u8 sval=0;    //在地址0读到的数据                         
    //每隔4K字节,写入一个数据,总共写入256个数据,刚好是1M字节
    for(i=0;i<1024*1024;i+=4096)
    {
        FSMC_SRAM_WriteBuffer(&temp,i,1);
        temp++;
    }
    //依次读出之前写入的数据,进行校验          
    for(i=0;i<1024*1024;i+=4096) 
    {
        FSMC_SRAM_ReadBuffer(&temp,i,1);
        if(i==0)sval=temp;
        else if(temp<=sval)break;//后面读出的数据一定要比第一次读到的数据大. 
        printf("%d ",temp);
    }                     
}    

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}

int main(void) {
    delay_init(168);
    uart_init(115200);
    printf("hahahha \n");
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOF时钟

    //GPIOF9,F10初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化

    GPIO_SetBits(GPIOD,GPIO_Pin_3);//GPIOF9,F10设置高，灯灭
    //GPIO_ResetBits(GPIOD,GPIO_Pin_3);
    SPI3_Init();
    LCD_Init();
    
    u32 ts=0;
    FSMC_SRAM_Init();            //初始化外部SRAM     
    for(ts=0;ts<250000;ts++)testsram[ts]=ts;//预存测试数据     
    fsmc_sram_test(60,170);
    
    while(1) {
        
    }
    return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
