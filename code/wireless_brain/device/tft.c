/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "tft.h"
#include "spi.h"
#include "delay.h"

//    lcddev.wramcmd=0X2C;
//    lcddev.setxcmd=0X2A;
//    lcddev.setycmd=0X2B;     

//LCD_RS//dc  
#define SPILCD_RS_SET  GPIO_SetBits(GPIOA,GPIO_Pin_9)//PC4 
#define SPILCD_RS_RESET  GPIO_ResetBits(GPIOA,GPIO_Pin_9)//PC4 
//LCD_CS  
#define SPILCD_CS_SET GPIO_SetBits(GPIOD,GPIO_Pin_2)//PA4 
#define SPILCD_CS_RESET  GPIO_ResetBits(GPIOD,GPIO_Pin_2)//PA4 



void spi_init(void) {
    SPI_InitTypeDef SPI_InitStructure;

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;//ILI9341使用16位 RGB 565的模式 一次就传输16位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

    SPI_Init(SPI3,&SPI_InitStructure); 
    SPI_Cmd(SPI3,ENABLE); 
}
void DMA_Configuration(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC->AHB1ENR|=RCC_AHB1Periph_DMA2;

    DMA_InitStructure.DMA_Channel = DMA_Channel_3;  //通道选择SPI1TX
    DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&(SPI3->DR);//DMA外设地址
    //DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
    //DMA_InitStructure.DMA_BufferSize = 7;//数据传输量 
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable ;//DMA_MemoryInc_Enable;//存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据长度:16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储器数据长度:16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输

    DMA_Init(DMA2_Stream5,&DMA_InitStructure);//初始化DMA Stream
    //DMA_ClearITPendingBit(DMA2_Stream5,DMA_IT_TCIF7);
    //DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE); 
}

void SPI_DMA_SendByte(uint16_t Byte ,uint32_t Length)
{
    DMA_Cmd(DMA2_Stream5,DISABLE);
    while(DMA_GetCmdStatus(DMA2_Stream5)==ENABLE);
    DMA2_Stream5->M0AR=(uint32_t)&Byte;
    DMA2_Stream5->NDTR=Length;
    SPI_DMACmd(SPI3,SPI_I2S_DMAReq_Tx,ENABLE);
    DMA_Cmd(DMA2_Stream5,ENABLE);
    while(DMA_GetFlagStatus(DMA2_Stream5,DMA_FLAG_TCIF5)==RESET);
    DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5);
} 

//写寄存器函数
//regval:寄存器值
void tft_wr_reg(u16 regval)
{ 
    SPILCD_RS_RESET;
    SPI_WriteByte(SPI3,regval&0x00FF);         
}
//写LCD数据
//data:要写入的值
void lcd_wr_data8_DATA(u16 data)
{
    SPILCD_RS_SET;
    SPI_WriteByte(SPI3,data>>8);
    SPI_WriteByte(SPI3,data);
}
void lcd_wr_data8(u8 da)   //写8位数据
{
    SPILCD_RS_SET;    
    SPI_WriteByte(SPI3,da);    
}                       
//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void tft_wr_reg_DATA(u8 LCD_Reg, u16 LCD_RegValue)
{
    tft_wr_reg(LCD_Reg);
    lcd_wr_data8_DATA(LCD_RegValue);
}
//开始写GRAM
void lcd_wr_data8iteRAM_Prepare(void)
{
    tft_wr_reg(0X2C);  
}     
//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
    while(i--);
}  
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    tft_wr_reg(0X2A); 
    lcd_wr_data8(Xpos>>8); 
    lcd_wr_data8(Xpos&0XFF);     
    tft_wr_reg(0X2B); 
    lcd_wr_data8(Ypos>>8); 
    lcd_wr_data8(Ypos&0XFF);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  
   tft_wr_reg(0x2a);
   lcd_wr_data8(x1>>8);
   lcd_wr_data8(x1);
   lcd_wr_data8(x2>>8);
   lcd_wr_data8(x2);
  
   tft_wr_reg(0x2b);
   lcd_wr_data8(y1>>8);
   lcd_wr_data8(y1);
   lcd_wr_data8(y2>>8);
   lcd_wr_data8(y2);

   tft_wr_reg(0x2C);

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
             lcd_wr_data8(VH);
             lcd_wr_data8(VL);    

        }

      }
}

void monitor_flush(int32_t sx, int32_t sy, int32_t ex, int32_t ey, const u16 *color_p)
{
    SPILCD_CS_RESET;
    u16 i,j;
    u16 xlen=0;
    xlen=ex-sx+1; 
     SPI3_SetSpeed(SPI_BaudRatePrescaler_2);
    for(i=sy;i<=ey;i++)
    {                                       
        LCD_SetCursor(sx,i);                      //设置光标位置 
        lcd_wr_data8iteRAM_Prepare();                 //开始写入GRAM      
        for(j=0;j<xlen;j++){lcd_wr_data8_DATA(*color_p);    //设置光标位置       
        color_p ++;}
    }
    SPILCD_CS_SET;
}

void tft_fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
    printf("%d %d %d %d \n",sx,sy);
    SPILCD_CS_RESET;
    u16 i,j;
    u16 xlen=0;
    xlen=ex-sx+1;       
    for(i=sy;i<=ey;i++)
    {                                       
        LCD_SetCursor(sx,i);                      //设置光标位置 
        lcd_wr_data8iteRAM_Prepare();                 //开始写入GRAM      
        for(j=0;j<xlen;j++)lcd_wr_data8_DATA(color);    //设置光标位置         
    }
    SPILCD_CS_SET;
}  

void tft_init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOB时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOB时钟

    //GPIOFB3,4,5初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PB3~5复用功能输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
    
    //GPIOFB3,4,5初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PB3~5复用功能输出   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOF时钟

    //GPIOF9,F10初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化

    GPIO_SetBits(GPIOD,GPIO_Pin_3);//GPIOF9,F10设置高，灯灭
    
    SPI3_SetSpeed(SPI_BaudRatePrescaler_16);
    SPILCD_CS_RESET;

    tft_wr_reg(0xCB);  
    lcd_wr_data8(0x39); 
    lcd_wr_data8(0x2C); 
    lcd_wr_data8(0x00); 
    lcd_wr_data8(0x34); 
    lcd_wr_data8(0x02); 

    tft_wr_reg(0xCF);  
    lcd_wr_data8(0x00); 
    lcd_wr_data8(0XC1); 
    lcd_wr_data8(0X30); 

    tft_wr_reg(0xE8);  
    lcd_wr_data8(0x85); 
    lcd_wr_data8(0x00); 
    lcd_wr_data8(0x78); 

    tft_wr_reg(0xEA);  
    lcd_wr_data8(0x00); 
    lcd_wr_data8(0x00); 

    tft_wr_reg(0xED);  
    lcd_wr_data8(0x64); 
    lcd_wr_data8(0x03); 
    lcd_wr_data8(0X12); 
    lcd_wr_data8(0X81); 

    tft_wr_reg(0xF7);  
    lcd_wr_data8(0x20); 

    tft_wr_reg(0xC0);    //Power control 
    lcd_wr_data8(0x23);   //VRH[5:0] 

    tft_wr_reg(0xC1);    //Power control 
    lcd_wr_data8(0x10);   //SAP[2:0];BT[3:0] 

    tft_wr_reg(0xC5);    //VCM control 
    lcd_wr_data8(0x3e); //对比度调节
    lcd_wr_data8(0x28); 

    tft_wr_reg(0xC7);    //VCM control2 
    lcd_wr_data8(0x86);  //--

    tft_wr_reg(0x36);    // Memory Access Control 
    lcd_wr_data8(0x48); //C8       //48 68竖屏//28 E8 横屏

    tft_wr_reg(0x3A);    
    lcd_wr_data8(0x55); 

    tft_wr_reg(0xB1);    
    lcd_wr_data8(0x00);  
    lcd_wr_data8(0x18); 

    tft_wr_reg(0xB6);    // Display Function Control 
    lcd_wr_data8(0x08); 
    lcd_wr_data8(0x82);
    lcd_wr_data8(0x27);  

    tft_wr_reg(0xF2);    // 3Gamma Function Disable 
    lcd_wr_data8(0x00); 

    tft_wr_reg(0x26);    //Gamma curve selected 
    lcd_wr_data8(0x01); 

    tft_wr_reg(0xE0);    //Set Gamma 
    lcd_wr_data8(0x0F); 
    lcd_wr_data8(0x31); 
    lcd_wr_data8(0x2B); 
    lcd_wr_data8(0x0C); 
    lcd_wr_data8(0x0E); 
    lcd_wr_data8(0x08); 
    lcd_wr_data8(0x4E); 
    lcd_wr_data8(0xF1); 
    lcd_wr_data8(0x37); 
    lcd_wr_data8(0x07); 
    lcd_wr_data8(0x10); 
    lcd_wr_data8(0x03); 
    lcd_wr_data8(0x0E); 
    lcd_wr_data8(0x09); 
    lcd_wr_data8(0x00); 

    tft_wr_reg(0XE1);    //Set Gamma 
    lcd_wr_data8(0x00); 
    lcd_wr_data8(0x0E); 
    lcd_wr_data8(0x14); 
    lcd_wr_data8(0x03); 
    lcd_wr_data8(0x11); 
    lcd_wr_data8(0x07); 
    lcd_wr_data8(0x31); 
    lcd_wr_data8(0xC1); 
    lcd_wr_data8(0x48); 
    lcd_wr_data8(0x08); 
    lcd_wr_data8(0x0F); 
    lcd_wr_data8(0x0C); 
    lcd_wr_data8(0x31); 
    lcd_wr_data8(0x36); 
    lcd_wr_data8(0x0F); 

    tft_wr_reg(0x11);    //Exit Sleep 
    delay_ms(120);
        
    tft_wr_reg(0x29);    //Display on 
    tft_wr_reg(0x2c); 
    
    SPILCD_CS_SET;
}


