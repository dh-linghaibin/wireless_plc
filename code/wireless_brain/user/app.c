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

  
u32 testsram[250000] __attribute__((at(0X68000000)));//����������
//�ⲿ�ڴ����(���֧��1M�ֽ��ڴ����)        
void fsmc_sram_test(u16 x,u16 y)
{  
    u32 i=0;        
    u8 temp=0;       
    u8 sval=0;    //�ڵ�ַ0����������                         
    //ÿ��4K�ֽ�,д��һ������,�ܹ�д��256������,�պ���1M�ֽ�
    for(i=0;i<1024*1024;i+=4096)
    {
        FSMC_SRAM_WriteBuffer(&temp,i,1);
        temp++;
    }
    //���ζ���֮ǰд�������,����У��          
    for(i=0;i<1024*1024;i+=4096) 
    {
        FSMC_SRAM_ReadBuffer(&temp,i,1);
        if(i==0)sval=temp;
        else if(temp<=sval)break;//�������������һ��Ҫ�ȵ�һ�ζ��������ݴ�. 
        printf("%d ",temp);
    }                     
}    

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//����ջ����ַ
//addr:ջ����ַ
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

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOFʱ��

    //GPIOF9,F10��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��

    GPIO_SetBits(GPIOD,GPIO_Pin_3);//GPIOF9,F10���øߣ�����
    //GPIO_ResetBits(GPIOD,GPIO_Pin_3);
    SPI3_Init();
    LCD_Init();
    
    u32 ts=0;
    FSMC_SRAM_Init();            //��ʼ���ⲿSRAM     
    for(ts=0;ts<250000;ts++)testsram[ts]=ts;//Ԥ���������     
    fsmc_sram_test(60,170);
    
    while(1) {
        
    }
    return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
