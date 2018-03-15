/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "tft.h"
#include "spi.h"
#include "delay.h"
//void tft_init(const char* name) {

//}

//LCD_RS//dc  
#define SPILCD_RS_SET  GPIO_SetBits(GPIOA,GPIO_Pin_9)//PC4 
#define SPILCD_RS_RESET  GPIO_ResetBits(GPIOA,GPIO_Pin_9)//PC4 
//LCD_CS  
#define SPILCD_CS_SET GPIO_SetBits(GPIOD,GPIO_Pin_2)//PA4 
#define SPILCD_CS_RESET  GPIO_ResetBits(GPIOD,GPIO_Pin_2)//PA4 



typedef struct lcd_dev{
    u16 width;			//LCD 宽度
    u16 height;			//LCD 高度
    u16 id;				//LCD ID
    u8	wramcmd;		//开始写gram指令
    u8  setxcmd;		//设置x坐标指令
    u8  setycmd;		//设置y坐标指令	 
} _lcd_dev; 


_lcd_dev lcddev;

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(u16 regval)
{ 
	SPILCD_CS_RESET;  //LCD_CS=0
    SPILCD_RS_RESET;
	SPI_WriteByte(SPI3,regval&0x00FF);
	SPILCD_CS_SET;  //LCD_CS=1	   		 
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA(u16 data)
{
 	SPILCD_CS_RESET;  //LCD_CS=0
	SPILCD_RS_SET;
	SPI_WriteByte(SPI3,data>>8);
	SPI_WriteByte(SPI3,data);
	SPILCD_CS_SET;  //LCD_CS=1		
}
void LCD_WR_DATA8(u8 da)   //写8位数据
{
	SPILCD_CS_RESET;  //LCD_CS=0
	SPILCD_RS_SET;				    	   
	SPI_WriteByte(SPI3,da);	
	SPILCD_CS_SET;  //LCD_CS=1   			 
}					   
//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WR_REG_DATA(u8 LCD_Reg, u16 LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}
//开始写GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);  
}	 
//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
	while(i--);
}  

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA8(Xpos>>8); 
	LCD_WR_DATA8(Xpos&0XFF);	 
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA8(Ypos>>8); 
	LCD_WR_DATA8(Ypos&0XFF);
} 	  

void TFT_Scan_Dir(u8 dir)	 //20171123  OK
{
	switch(dir)
		{
			case 0:	//  屏排线与ＩＣ在下方　　　默认模式
				{	
					lcddev.width=240;    //LCD 宽度
					lcddev.height=320;   //LCD 高度
					lcddev.setxcmd=0X2A;  //设置x坐标指令
					lcddev.setycmd=0X2B;  //设置y坐标指令
					lcddev.wramcmd=0X2C;  //开始写gram指令
//					LCD_WR_REG(0xB6);
//					LCD_WR_DATA8(0x02);//RGB 32, MCU 02
//					LCD_WR_DATA8(0x22);//22,42  GS,SS
//					LCD_WR_DATA8(0x3B);
					
					LCD_WR_REG(0x36);
					LCD_WR_DATA8(0x08);  //正常看排线与ＩＣ在下方：起点在左上角，从上往下扫，从左往右扫  08
				}break;
				
			case 1:	//　 屏排线与ＩＣ在左方　
				{	
					lcddev.width=320;    //LCD 宽度
					lcddev.height=240;   //LCD 高度
					lcddev.setxcmd=0X2A;  //设置x坐标指令2A
					lcddev.setycmd=0X2B;  //设置y坐标指令2B
					lcddev.wramcmd=0X2C;  //开始写gram指令
//					LCD_WR_REG(0xB6);
//					LCD_WR_DATA8(0x02);//RGB 32, MCU 02
//					LCD_WR_DATA8(0x22);//22,42  GS,SS
//					LCD_WR_DATA8(0x3B);
					
					LCD_WR_REG(0x36);
					LCD_WR_DATA8(0xA8);  //正常看排线与ＩＣ在左方：起点在左下角，从下往上扫，从左往右扫 　A8, 
				}break;
				
			case 2:	//	 屏排线与ＩＣ在上方  俗称倒装使用
				{	
					lcddev.width=240;    //LCD 宽度
					lcddev.height=320;   //LCD 高度
					lcddev.setxcmd=0X2A;  //设置x坐标指令
					lcddev.setycmd=0X2B;  //设置y坐标指令
					lcddev.wramcmd=0X2C;  //开始写gram指令
//					LCD_WR_REG(0xB6);
//					LCD_WR_DATA8(0x02);//RGB 32, MCU 02
//					LCD_WR_DATA8(0x22);//22,42  GS,SS
//					LCD_WR_DATA8(0x3B);
					
					LCD_WR_REG(0x36);
					LCD_WR_DATA8(0xC8);  //正常看排线与ＩＣ在下方：起点在右上角，从下往上扫，从右往左扫  C8
				}break;
				
			case 3:	//	 屏排线与ＩＣ在右方
				{	
					lcddev.width=320;    //LCD 宽度
					lcddev.height=240;   //LCD 高度
					lcddev.setxcmd=0X2A;  //设置x坐标指令
					lcddev.setycmd=0X2B;  //设置y坐标指令
					lcddev.wramcmd=0X2C;  //开始写gram指令
//					LCD_WR_REG(0xB6);
//					LCD_WR_DATA8(0x02);//RGB 32, MCU 02
//					LCD_WR_DATA8(0x22);//22,42  GS,SS
//					LCD_WR_DATA8(0x3B);
					
					LCD_WR_REG(0x36);
					LCD_WR_DATA8(0x68);  //正常看排线与ＩＣ在右方：起点在左下角，从下往上扫，从左往右扫  68  78
				}break;
				
		}					
}

void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{

	//设置法二
	TFT_Scan_Dir(1);//按顺时针转：0:0度，1：90度，2：180度，3：270度， OK20171124
	
	LCD_WR_REG(lcddev.setxcmd);   
	LCD_WR_DATA8(Xstart>>8);
	LCD_WR_DATA8(Xstart&0xff);
	LCD_WR_DATA8(Xend>>8);
	LCD_WR_DATA8(Xend&0xff);

	LCD_WR_REG(lcddev.setycmd);   
	LCD_WR_DATA8(Ystart>>8);
	LCD_WR_DATA8(Ystart&0xff);
	LCD_WR_DATA8(Yend>>8);
	LCD_WR_DATA8(Yend&0xff);
	
	LCD_WR_REG(0x2C);
	
	//设置法三
//	LCDBlock2A2B( Xstart,Xend,Ystart,Yend); 	//9163,9331,9340,9341,7735,7789,8357D,9327,9488,HX8357C//OK

}


void LCD_WriteRAM(u16 RGB_Code)
{
//	LCD->LCD_RAM = RGB_Code;//写十六位GRAM

LCD_WR_DATA8(RGB_Code>>8);
LCD_WR_DATA8(RGB_Code&0xFF);	
}

void LCD_Clear(u16 color)
{
	u16 i,j;
	u32 index=0;     
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//得到总点数
	
	BlockWrite(0,lcddev.width,0,lcddev.height);

	for(index=0;index<totalpoint;index++)
	{
		LCD_WriteRAM(color);
//		LCD->LCD_RAM=color;
	
	}	
	
} 

void LCD_Display(u16 color)
{
	u16 i,j;
	
	u16 sx=0,sy=0;
	u16 twidth,theight;
	twidth=sx+lcddev.width-1;
	theight=sy+lcddev.height-1;

	BlockWrite(0,lcddev.width,0,lcddev.height);

	for(i=0;i<lcddev.height;i++)
	{
		for(j=0;j<lcddev.width;j++)
		{	
			LCD_WriteRAM(color);
//			LCD->LCD_RAM=color;			
		}
	
	}
	
}

void LCD_Init(void)
{
    
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
    delay_ms(10);
	lcddev.width=240;
	lcddev.height=320;
	lcddev.wramcmd=0X2C;
	lcddev.setxcmd=0X2A;
	lcddev.setycmd=0X2B; 	
//    LCD_WR_REG(0xCB);  
//    LCD_WR_DATA8(0x39); 
//    LCD_WR_DATA8(0x2C); 
//    LCD_WR_DATA8(0x00); 
//    LCD_WR_DATA8(0x34); 
//    LCD_WR_DATA8(0x02); 

//    LCD_WR_REG(0xCF);  
//    LCD_WR_DATA8(0x00); 
//    LCD_WR_DATA8(0XC1); 
//    LCD_WR_DATA8(0X30); 

//    LCD_WR_REG(0xE8);  
//    LCD_WR_DATA8(0x85); 
//    LCD_WR_DATA8(0x00); 
//    LCD_WR_DATA8(0x78); 

//    LCD_WR_REG(0xEA);  
//    LCD_WR_DATA8(0x00); 
//    LCD_WR_DATA8(0x00); 

//    LCD_WR_REG(0xED);  
//    LCD_WR_DATA8(0x64); 
//    LCD_WR_DATA8(0x03); 
//    LCD_WR_DATA8(0X12); 
//    LCD_WR_DATA8(0X81); 

//    LCD_WR_REG(0xF7);  
//    LCD_WR_DATA8(0x20); 

//    LCD_WR_REG(0xC0);    //Power control 
//    LCD_WR_DATA8(0x23);   //VRH[5:0] 

//    LCD_WR_REG(0xC1);    //Power control 
//    LCD_WR_DATA8(0x10);   //SAP[2:0];BT[3:0] 

//    LCD_WR_REG(0xC5);    //VCM control 
//    LCD_WR_DATA8(0x3e); //对比度调节
//    LCD_WR_DATA8(0x28); 

//    LCD_WR_REG(0xC7);    //VCM control2 
//    LCD_WR_DATA8(0x86);  //--

//    LCD_WR_REG(0x36);    // Memory Access Control 
//    LCD_WR_DATA8(0x48); //C8	   //48 68竖屏//28 E8 横屏

//    LCD_WR_REG(0x3A);    
//    LCD_WR_DATA8(0x55); 

//    LCD_WR_REG(0xB1);    
//    LCD_WR_DATA8(0x00);  
//    LCD_WR_DATA8(0x18); 

//    LCD_WR_REG(0xB6);    // Display Function Control 
//    LCD_WR_DATA8(0x08); 
//    LCD_WR_DATA8(0x82);
//    LCD_WR_DATA8(0x27);  

//    LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
//    LCD_WR_DATA8(0x00); 

//    LCD_WR_REG(0x26);    //Gamma curve selected 
//    LCD_WR_DATA8(0x01); 

//    LCD_WR_REG(0xE0);    //Set Gamma 
//    LCD_WR_DATA8(0x0F); 
//    LCD_WR_DATA8(0x31); 
//    LCD_WR_DATA8(0x2B); 
//    LCD_WR_DATA8(0x0C); 
//    LCD_WR_DATA8(0x0E); 
//    LCD_WR_DATA8(0x08); 
//    LCD_WR_DATA8(0x4E); 
//    LCD_WR_DATA8(0xF1); 
//    LCD_WR_DATA8(0x37); 
//    LCD_WR_DATA8(0x07); 
//    LCD_WR_DATA8(0x10); 
//    LCD_WR_DATA8(0x03); 
//    LCD_WR_DATA8(0x0E); 
//    LCD_WR_DATA8(0x09); 
//    LCD_WR_DATA8(0x00); 

//    LCD_WR_REG(0XE1);    //Set Gamma 
//    LCD_WR_DATA8(0x00); 
//    LCD_WR_DATA8(0x0E); 
//    LCD_WR_DATA8(0x14); 
//    LCD_WR_DATA8(0x03); 
//    LCD_WR_DATA8(0x11); 
//    LCD_WR_DATA8(0x07); 
//    LCD_WR_DATA8(0x31); 
//    LCD_WR_DATA8(0xC1); 
//    LCD_WR_DATA8(0x48); 
//    LCD_WR_DATA8(0x08); 
//    LCD_WR_DATA8(0x0F); 
//    LCD_WR_DATA8(0x0C); 
//    LCD_WR_DATA8(0x31); 
//    LCD_WR_DATA8(0x36); 
//    LCD_WR_DATA8(0x0F); 

//    LCD_WR_REG(0x11);    //Exit Sleep 
//    delay_ms(120);

//    LCD_WR_REG(0x29);    //Display on 
   // LCD_WR_REG(0x2c); 
    //LCD_Clear(WHITE);
    
    LCD_WR_REG(0xCF);  
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0xC1); 
    LCD_WR_DATA8(0X30); 

    LCD_WR_REG(0xED);  
    LCD_WR_DATA8(0x64); 
    LCD_WR_DATA8(0x03); 
    LCD_WR_DATA8(0X12); 
    LCD_WR_DATA8(0X81); 

    LCD_WR_REG(0xE8);  
    LCD_WR_DATA8(0x85); 
    LCD_WR_DATA8(0x10); 
    LCD_WR_DATA8(0x78); 

    LCD_WR_REG(0xCB);  
    LCD_WR_DATA8(0x39); 
    LCD_WR_DATA8(0x2C); 
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x34); 
    LCD_WR_DATA8(0x02); 

    LCD_WR_REG(0xF7);  
    LCD_WR_DATA8(0x20); 

    LCD_WR_REG(0xEA);  
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x00); 

    LCD_WR_REG(0xC0);    //Power control 
    LCD_WR_DATA8(0x21);   //VRH[5:0] 

    LCD_WR_REG(0xC1);    //Power control 
    LCD_WR_DATA8(0x12);   //SAP[2:0];BT[3:0] 

    LCD_WR_REG(0xC5);    //VCM control 
    LCD_WR_DATA8(0x32); 
    LCD_WR_DATA8(0x3C); 

    LCD_WR_REG(0xC7);    //VCM control2 
    LCD_WR_DATA8(0XA7); //B4  

    LCD_WR_REG(0x36);    // Memory Access Control 
    LCD_WR_DATA8(0x08); 

    LCD_WR_REG(0x3A);   
    LCD_WR_DATA8(0x55); 

    LCD_WR_REG(0xB1);   
    LCD_WR_DATA8(0x00);   
    LCD_WR_DATA8(0x17); 

    LCD_WR_REG(0xB6);    // Display Function Control 
    LCD_WR_DATA8(0x0A); 
    LCD_WR_DATA8(0xA2); 

    LCD_WR_REG(0xF6);    
    LCD_WR_DATA8(0x01); 
    LCD_WR_DATA8(0x30); 

    LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
    LCD_WR_DATA8(0x00); 

    LCD_WR_REG(0x26);    //Gamma curve selected 
    LCD_WR_DATA8(0x01); 

    LCD_WR_REG(0xE0);    //Set Gamma 
    LCD_WR_DATA8(0x0F); 
    LCD_WR_DATA8(0x20); 
    LCD_WR_DATA8(0x1E); 
    LCD_WR_DATA8(0x07); 
    LCD_WR_DATA8(0x0A); 
    LCD_WR_DATA8(0x03); 
    LCD_WR_DATA8(0x52); 
    LCD_WR_DATA8(0X63); 
    LCD_WR_DATA8(0x44); 
    LCD_WR_DATA8(0x08); 
    LCD_WR_DATA8(0x17); 
    LCD_WR_DATA8(0x09); 
    LCD_WR_DATA8(0x19); 
    LCD_WR_DATA8(0x13); 
    LCD_WR_DATA8(0x00); 

    LCD_WR_REG(0XE1);    //Set Gamma 
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x16); 
    LCD_WR_DATA8(0x19); 
    LCD_WR_DATA8(0x02); 
    LCD_WR_DATA8(0x0F); 
    LCD_WR_DATA8(0x03); 
    LCD_WR_DATA8(0x2F); 
    LCD_WR_DATA8(0x13); 
    LCD_WR_DATA8(0x40); 
    LCD_WR_DATA8(0x01); 
    LCD_WR_DATA8(0x08); 
    LCD_WR_DATA8(0x07); 
    LCD_WR_DATA8(0x2E); 
    LCD_WR_DATA8(0x3C); 
    LCD_WR_DATA8(0x0F); 

    LCD_WR_REG(0x11);    //Exit Sleep 
    delay_ms(20); 
    LCD_WR_REG(0x29);    //Display on 	

    LCD_Clear(0xFFFF);
}


