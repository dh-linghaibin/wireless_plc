/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "tft.h"
#include "spi.h"
#include "delay.h"

//LCD_RS//dc  
#define SPILCD_RS_SET  GPIO_SetBits(GPIOA,GPIO_Pin_9)//PC4 
#define SPILCD_RS_RESET  GPIO_ResetBits(GPIOA,GPIO_Pin_9)//PC4 
//LCD_CS  
#define SPILCD_CS_SET GPIO_SetBits(GPIOD,GPIO_Pin_2)//PA4 
#define SPILCD_CS_RESET  GPIO_ResetBits(GPIOD,GPIO_Pin_2)//PA4 

typedef struct lcd_dev{
    u16 width;            //LCD ���
    u16 height;            //LCD �߶�
    u16 id;                //LCD ID
    u8    wramcmd;        //��ʼдgramָ��
    u8  setxcmd;        //����x����ָ��
    u8  setycmd;        //����y����ָ��     
} _lcd_dev; 


_lcd_dev lcddev;

//д�Ĵ�������
//regval:�Ĵ���ֵ
void LCD_WR_REG(u16 regval)
{ 
    SPILCD_CS_RESET;  //LCD_CS=0
    SPILCD_RS_RESET;
    SPI_WriteByte(SPI3,regval&0x00FF);
    SPILCD_CS_SET;  //LCD_CS=1                
}
//дLCD����
//data:Ҫд���ֵ
void LCD_WR_DATA(u16 data)
{
     SPILCD_CS_RESET;  //LCD_CS=0
    SPILCD_RS_SET;
    SPI_WriteByte(SPI3,data>>8);
    SPI_WriteByte(SPI3,data);
    SPILCD_CS_SET;  //LCD_CS=1        
}
void LCD_WR_DATA8(u8 da)   //д8λ����
{
    SPILCD_CS_RESET;  //LCD_CS=0
    SPILCD_RS_SET;    
    SPI_WriteByte(SPI3,da);    
    SPILCD_CS_SET;  //LCD_CS=1                
}                       
//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WR_REG_DATA(u8 LCD_Reg, u16 LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);  
}     
//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(u8 i)
{
    while(i--);
}  
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_WR_REG(lcddev.setxcmd); 
    LCD_WR_DATA8(Xpos>>8); 
    LCD_WR_DATA8(Xpos&0XFF);     
    LCD_WR_REG(lcddev.setycmd); 
    LCD_WR_DATA8(Ypos>>8); 
    LCD_WR_DATA8(Ypos&0XFF);
}
//��������
//color:Ҫ���������ɫ
//void LCD_Clear(u16 color)
//{
//    u32 index=0;      
//    u32 totalpoint=lcddev.width;
//    totalpoint*=lcddev.height;     //�õ��ܵ���
//    LCD_SetCursor(0x00,0x0000);    //���ù��λ�� 
//    LCD_WriteRAM_Prepare();     //��ʼд��GRAM           
//    for(index=0;index<totalpoint;index++)
//    {
//        LCD_WR_DATA(color);
//    }
//}  

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  
   LCD_WR_REG(0x2a);
   LCD_WR_DATA8(x1>>8);
   LCD_WR_DATA8(x1);
   LCD_WR_DATA8(x2>>8);
   LCD_WR_DATA8(x2);
  
   LCD_WR_REG(0x2b);
   LCD_WR_DATA8(y1>>8);
   LCD_WR_DATA8(y1);
   LCD_WR_DATA8(y2>>8);
   LCD_WR_DATA8(y2);

   LCD_WR_REG(0x2C);

}

#define LCD_W 240
#define LCD_H 320

void LCD_Clear(u16 Color)
{
    u8 VH,VL;
    u16 i,j;
    VH=Color>>8;
    VL=Color;    
    Address_set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
     {
      for (j=0;j<LCD_H;j++)
           {
             LCD_WR_DATA8(VH);
             LCD_WR_DATA8(VL);    

        }

      }
}

void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
    u16 i,j;
    u16 xlen=0;
    xlen=ex-sx+1;       
    for(i=sy;i<=ey;i++)
    {                                       
         LCD_SetCursor(sx,i);                      //���ù��λ�� 
        LCD_WriteRAM_Prepare();                 //��ʼд��GRAM      
        for(j=0;j<xlen;j++)LCD_WR_DATA(color);    //���ù��λ��         
    }
}  

void LCD_Init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOBʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOBʱ��

    //GPIOFB3,4,5��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PB3~5���ù������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
    
    //GPIOFB3,4,5��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PB3~5���ù������   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
    GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOFʱ��

    //GPIOF9,F10��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��

    GPIO_SetBits(GPIOD,GPIO_Pin_3);//GPIOF9,F10���øߣ�����
    
    lcddev.width=240;
    lcddev.height=320;
    lcddev.wramcmd=0X2C;
    lcddev.setxcmd=0X2A;
    lcddev.setycmd=0X2B;     

    LCD_WR_REG(0xCB);  
    LCD_WR_DATA8(0x39); 
    LCD_WR_DATA8(0x2C); 
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x34); 
    LCD_WR_DATA8(0x02); 

    LCD_WR_REG(0xCF);  
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0XC1); 
    LCD_WR_DATA8(0X30); 

    LCD_WR_REG(0xE8);  
    LCD_WR_DATA8(0x85); 
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x78); 

    LCD_WR_REG(0xEA);  
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x00); 

    LCD_WR_REG(0xED);  
    LCD_WR_DATA8(0x64); 
    LCD_WR_DATA8(0x03); 
    LCD_WR_DATA8(0X12); 
    LCD_WR_DATA8(0X81); 

    LCD_WR_REG(0xF7);  
    LCD_WR_DATA8(0x20); 

    LCD_WR_REG(0xC0);    //Power control 
    LCD_WR_DATA8(0x23);   //VRH[5:0] 

    LCD_WR_REG(0xC1);    //Power control 
    LCD_WR_DATA8(0x10);   //SAP[2:0];BT[3:0] 

    LCD_WR_REG(0xC5);    //VCM control 
    LCD_WR_DATA8(0x3e); //�Աȶȵ���
    LCD_WR_DATA8(0x28); 

    LCD_WR_REG(0xC7);    //VCM control2 
    LCD_WR_DATA8(0x86);  //--

    LCD_WR_REG(0x36);    // Memory Access Control 
    LCD_WR_DATA8(0x48); //C8       //48 68����//28 E8 ����

    LCD_WR_REG(0x3A);    
    LCD_WR_DATA8(0x55); 

    LCD_WR_REG(0xB1);    
    LCD_WR_DATA8(0x00);  
    LCD_WR_DATA8(0x18); 

    LCD_WR_REG(0xB6);    // Display Function Control 
    LCD_WR_DATA8(0x08); 
    LCD_WR_DATA8(0x82);
    LCD_WR_DATA8(0x27);  

    LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
    LCD_WR_DATA8(0x00); 

    LCD_WR_REG(0x26);    //Gamma curve selected 
    LCD_WR_DATA8(0x01); 

    LCD_WR_REG(0xE0);    //Set Gamma 
    LCD_WR_DATA8(0x0F); 
    LCD_WR_DATA8(0x31); 
    LCD_WR_DATA8(0x2B); 
    LCD_WR_DATA8(0x0C); 
    LCD_WR_DATA8(0x0E); 
    LCD_WR_DATA8(0x08); 
    LCD_WR_DATA8(0x4E); 
    LCD_WR_DATA8(0xF1); 
    LCD_WR_DATA8(0x37); 
    LCD_WR_DATA8(0x07); 
    LCD_WR_DATA8(0x10); 
    LCD_WR_DATA8(0x03); 
    LCD_WR_DATA8(0x0E); 
    LCD_WR_DATA8(0x09); 
    LCD_WR_DATA8(0x00); 

    LCD_WR_REG(0XE1);    //Set Gamma 
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x0E); 
    LCD_WR_DATA8(0x14); 
    LCD_WR_DATA8(0x03); 
    LCD_WR_DATA8(0x11); 
    LCD_WR_DATA8(0x07); 
    LCD_WR_DATA8(0x31); 
    LCD_WR_DATA8(0xC1); 
    LCD_WR_DATA8(0x48); 
    LCD_WR_DATA8(0x08); 
    LCD_WR_DATA8(0x0F); 
    LCD_WR_DATA8(0x0C); 
    LCD_WR_DATA8(0x31); 
    LCD_WR_DATA8(0x36); 
    LCD_WR_DATA8(0x0F); 

    LCD_WR_REG(0x11);    //Exit Sleep 
    delay_ms(120);
        
    LCD_WR_REG(0x29);    //Display on 
    LCD_WR_REG(0x2c); 
    LCD_Clear(0xF810);

    //LCD_Fill(20,20,40,40,0xF800);

   // LCD_WR_REG(0x28);
}


