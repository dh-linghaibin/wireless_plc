/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
#include "delay.h"

static uint8_t  fac_us=0;                     //us��ʱ������            
static uint16_t fac_ms=0;                     //ms��ʱ������,��os��,����ÿ�����ĵ�ms��

void delay_init(uint8_t sys_clk) {
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
    fac_us=sys_clk/8;                  //�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
    fac_ms=(uint16_t)fac_us*1000;            //��OS��,����ÿ��ms��Ҫ��systickʱ����   
}   

void delay_us(uint32_t nus) {      
   uint32_t temp;           
   SysTick->LOAD=nus*fac_us;             //ʱ�����            
   SysTick->VAL=0x00;                    //��ռ�����
   SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //��ʼ����     
   do
   {
      temp=SysTick->CTRL;
   }while((temp&0x01)&&!(temp&(1<<16)));   //�ȴ�ʱ�䵽��   
   SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //�رռ�����
   SysTick->VAL =0X00;                   //��ռ����� 
}

//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��168M������,nms<=798ms 
void delay_xms(uint16_t nms) {                 
   uint32_t temp;         
   SysTick->LOAD=(uint32_t)nms*fac_ms;         //ʱ�����(SysTick->LOADΪ24bit)
   SysTick->VAL =0x00;                    //��ռ�����
   SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ���� 
   do
   {
      temp=SysTick->CTRL;
   }while((temp&0x01)&&!(temp&(1<<16)));   //�ȴ�ʱ�䵽��   
   SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
   SysTick->VAL =0X00;                   //��ռ�����            
} 
//��ʱnms 
//nms:0~65535
void delay_ms(uint16_t nms) {        
   uint8_t repeat=nms/540;                  //������540,�ǿ��ǵ�ĳЩ�ͻ����ܳ�Ƶʹ��,
                                 //���糬Ƶ��248M��ʱ��,delay_xms���ֻ����ʱ541ms������
   uint16_t remain=nms%540;
   while(repeat)
   {
      delay_xms(540);
      repeat--;
   }
   if(remain)delay_xms(remain);
} 
