/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#include "lcd.h"
#include "delay.h"

#define Bank1_LCD_D    ((u32)0x60020000)    //Disp Data ADDR
#define Bank1_LCD_C    ((u32)0x60000000)   //Disp Reg ADDR

static void lcd_rst(void) {
    GPIO_ResetBits(GPIOA, GPIO_Pin_7);// RST = 1 GPIO_SetBits
    delay_ms(100);
    GPIO_SetBits(GPIOA, GPIO_Pin_7);// RST = 1  GPIO_ResetBits
    delay_ms(100);
}

static void lcd_backlight(uint8_t cmd) {
    if(cmd) {
        GPIO_SetBits(GPIOA, GPIO_Pin_6);
    } else {
        GPIO_ResetBits(GPIOA, GPIO_Pin_6);
    }
}

static void write_comm(u16 CMD) {
    *(__IO u16 *) (Bank1_LCD_C) = CMD;
}
void write_data(u16 tem_data) {
    *(__IO u16 *) (Bank1_LCD_D) = tem_data;
}

/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart、yStart、Xend、Yend随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的起始点
          Xend   y方向的终止点
          Yend   y方向的终止点
返回值：无
***********************************************/
void block_write(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) {
    //R61581
    write_comm(0x2a);   
    write_data(Xstart>>8);
    write_data(Xstart&0xff);
    write_data(Xend>>8);
    write_data(Xend&0xff);

    write_comm(0x2b);   
    write_data(Ystart>>8);
    write_data(Ystart&0xff);
    write_data(Yend>>8);
    write_data(Yend&0xff);

    write_comm(0x2c);
}

/**********************************************
函数名：Lcd块选函数
功能：选定Lcd上指定的矩形区域

注意：xStart和 yStart随着屏幕的旋转而改变，位置是矩形框的四个角

入口参数：xStart x方向的起始点
          ySrart y方向的终止点
          xLong 要选定矩形的x方向长度
          yLong  要选定矩形的y方向长度
返回值：无
***********************************************/
void lcd_clolor_box(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color) {
    u32 temp;

    block_write(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
    for (temp=0; temp<xLong*yLong; temp++) {
        *(__IO u16 *) (Bank1_LCD_D) = Color;
    }
}


static void gpio_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    /* Enable the FSMC Clock */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
    /* config lcd gpio clock base on FSMC */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉

    /* config tft rst gpio */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;        
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* config tft back_light gpio base on the PT4101 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;      
    GPIO_Init(GPIOA, &GPIO_InitStructure);             
    /* tft control gpio init */     

    GPIO_SetBits(GPIOA, GPIO_Pin_7);         // RST = 1

    //使能FSMC时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD |RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG,   ENABLE);    
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);
    //PORTD复用推挽输出  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15;            
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
    GPIO_Init(GPIOD, &GPIO_InitStructure); 
    //PORTE复用推挽输出  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    //       //PORTD复用推挽输出  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_13;     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
    GPIO_Init(GPIOD, &GPIO_InitStructure); 

    GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);//PD0,AF12 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);//PD1,AF12 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);   
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FSMC);        
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);   
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FSMC);        
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);//PD15,AF12 

    GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);//PE7,AF12 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);//PE15,AF12 
}

static void fsmc_init(void) {
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p; 

    p.FSMC_AddressSetupTime = 0x02;     //地址建立时间
    p.FSMC_AddressHoldTime = 0x00;     //地址保持时间
    p.FSMC_DataSetupTime = 0x05;         //数据建立时间
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;

    p.FSMC_AccessMode = FSMC_AccessMode_B;     // 一般使用模式B来控制LCD

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

    /* Enable FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}

void lcd_init(void) {
    gpio_init();
    fsmc_init();
    lcd_rst();
    //S6D05A1初始化
    write_comm(0x13);
    write_comm(0x35);
    write_data(0x00);
    write_comm(0x36);
    write_data(0x48);
    write_comm(0xD0);
    write_data(0x00);
    write_data(0x05);

    write_comm(0xEF);
    write_data(0x07);
    
    write_comm(0xF2);
    write_data(0x1B);
    write_data(0x16);
    write_data(0x0F);
    write_data(0x08);
    write_data(0x08);
    write_data(0x08);
    write_data(0x08);
    write_data(0x10);
    write_data(0x00);
    write_data(0x1C);
    write_data(0x16);

    write_comm(0xF3);
    write_data(0x01);
    write_data(0x41);
    write_data(0x15);
    write_data(0x0D);
    write_data(0x33);
    write_data(0x63);
    write_data(0x46);
    write_data(0x10);

    write_comm(0xF4);
    write_data(0x5B);
    write_data(0x5B);
    write_data(0x55);
    write_data(0x55);
    write_data(0x44);

    write_comm(0xF5);
    write_data(0x12);
    write_data(0x11);
    write_data(0x06);
    write_data(0xF0);
    write_data(0x00);
    write_data(0x1F);

    write_comm(0xF6);
    write_data(0x80);
    write_data(0x10);
    write_data(0x00);

    write_comm(0xFD);
    write_data(0x11);
    write_data(0x1D);
    write_data(0x00);

    write_comm(0xF8); //Positive Gamma Control
    write_data(0x00);
    write_data(0x15);
    write_data(0x01);
    write_data(0x08);
    write_data(0x15);
    write_data(0x22);
    write_data(0x25);
    write_data(0x28);
    write_data(0x14);
    write_data(0x13);
    write_data(0x10);
    write_data(0x11);
    write_data(0x09);
    write_data(0x24);
    write_data(0x28);

    write_comm(0xF9); //Positive Gamma Control
    write_data(0x00);
    write_data(0x15);
    write_data(0x01);
    write_data(0x08);
    write_data(0x15);
    write_data(0x22);
    write_data(0x25);
    write_data(0x28);
    write_data(0x14);
    write_data(0x13);
    write_data(0x10);
    write_data(0x11);
    write_data(0x09);
    write_data(0x24);
    write_data(0x28);

    write_comm(0xFC); //Positive Gamma Control
    write_data(0x00);
    write_data(0x15);
    write_data(0x01);
    write_data(0x08);
    write_data(0x15);
    write_data(0x22);
    write_data(0x25);
    write_data(0x28);
    write_data(0x14);
    write_data(0x13);
    write_data(0x10);
    write_data(0x11);
    write_data(0x09);
    write_data(0x24);
    write_data(0x28);

    write_comm(0x36);//Memory Data Access Control
    write_data(0x48);

    write_comm(0x3A);//SET 65K Color
    write_data(0x55);
    
    write_comm(0x11);
    delay_ms(20);
    write_comm(0x29);//Display on
    write_comm(0x2C);//Write GRAM
    delay_ms(20);
    lcd_clolor_box(0,0,320,480,Black);
    lcd_backlight(1);
}

void LCD_SetCursor(u16 Xpos, u16 Ypos)
{ 
    write_comm(0X2A); 
    write_data(Xpos>>8);write_data(Xpos&0XFF); 
    write_comm(0X2B); 
    write_data(Ypos>>8);write_data(Ypos&0XFF);
}

//开始写GRAM
void LCD_WriteRAM_Prepare(void) {
   write_comm(0X2C);
}


#include "font_term_8x16.h"
#include "font_b7x8.h"
#include "font_7x8.h"
#include "font_6x8.h"
#include "font_term_8x16.h"
#include "font_fxs_8x15.h"
#include "font_term_10x16.h"

static void lcd_draw_char(font_type font,int16_t x, int16_t y, unsigned char c,
              uint16_t color, uint16_t bg, uint8_t sx, uint8_t sy)  {
  if(c>127) return;
    uint16_t *fontRects = (uint16_t *)wlcd_font_term_8x16_0_127_Rects;
    uint16_t *fontOffs = (uint16_t *)wlcd_font_term_8x16_0_127_CharOffs;
    int screenWd = 320;
    int screenHt = 480;
    int charWd = 8;
    int charHt = 16;
    int charYoffs = 1;
    int bold = 0;
    switch(font) {
        case F_7X8: {
            fontRects = (uint16_t *)font_7x8_Rects;
            fontOffs = (uint16_t *)font_7x8_CharOffs;
            charWd = 7;
            charHt = 10; // real 8
            charYoffs = 1;
        } break;
        case F_7X8B: {
            fontRects = (uint16_t *)font_b7x8_Rects;
            fontOffs = (uint16_t *)font_b7x8_CharOffs;
            charWd = 7;
            charHt = 10; // real 8
            charYoffs = 1;
        } break;
        case F_6X8: {
            fontRects = (uint16_t *)font_6x8_Rects;
            fontOffs = (uint16_t *)font_6x8_CharOffs;
            charWd = 6;
            charHt = 9; // real 8
            charYoffs = 1;
        } break;
        case F_8X16: {
            fontRects = (uint16_t *)wlcd_font_term_8x16_0_127_Rects;
            fontOffs = (uint16_t *)wlcd_font_term_8x16_0_127_CharOffs;
            charWd = 8;
            charHt = 16;
            charYoffs = 0;
        } break;
        case F_8X15: {
            fontRects = (uint16_t *)wlcd_font_fxs_8x15_16_127_Rects;
            fontOffs = (uint16_t *)wlcd_font_fxs_8x15_16_127_CharOffs;
            charWd = 8;
            charHt = 15; // real 15
            charYoffs = 0;
        } break;
        case F_10X16: {
            fontRects = (uint16_t *)font_term_10x16_Rects;
            fontOffs = (uint16_t *)font_term_10x16_CharOffs;
            charWd = 10;
            charHt = 16;
            charYoffs = 0;
        } break;
    }
  
    if((x >= screenWd)              || // Clip right
        (y >= screenHt)              || // Clip bottom
        ((x + charWd * sx - 1) < 0)  || // Clip left
        ((y + charHt * sy - 1) < 0))    // Clip top
        return;
  
    uint16_t recIdx = fontOffs[c];
    uint16_t recNum = fontOffs[c+1]-recIdx;
    if(bg && bg!=color) lcd_clolor_box(x, y, charWd*sx, charHt*sy, bg);
        if(charWd<=16 && charHt<=16)
            for(int i=0; i<recNum; i++) {
                int v = fontRects[i+recIdx];
                int xf = v & 0xf;
                int yf = charYoffs+((v & 0xf0)>>4);
                int wf = 1+((v & 0xf00)>>8);
                int hf = 1+((v & 0xf000)>>12);
                lcd_clolor_box(x+xf*sx, y+yf*sy, bold+wf*sx, hf*sy, color);
            }
        else
            for(int i=0; i<recNum; i++) {
                uint8_t *rects = (uint8_t*)fontRects;
                int idx = (i+recIdx)*3;
                int xf = rects[idx+0] & 0x3f;
                int yf = rects[idx+1] & 0x3f;
                int wf = 1+rects[idx+2] & 0x3f;
                int hf = 1+(((rects[idx+0] & 0xc0)>>6) | ((rects[idx+1] & 0xc0)>>4) | ((rects[idx+2] & 0xc0)>>2));
                lcd_clolor_box(x+xf*sx, y+yf*sy, bold+wf*sx, hf*sy, color);
            }
}

void lcd_string(font_type font,uint16_t sx,uint16_t sy,char * str,uint16_t color, uint16_t bg) {
    int x = sx;
    while(*str) {
        //lcd_clolor_box(x,sy,x+8,8,bg);
        lcd_draw_char(font,x,sy,*str++,color,bg,1,1);
        switch(font) {
            case F_7X8: {
                x += 7;
            } break;
            case F_7X8B: {
                x += 7;
            } break;
            case F_6X8: {
                x += 6;
            } break;
            case F_8X16: {
                x += 8;
            } break;
            case F_8X15: {
                x += 8;
            } break;
            case F_10X16: {
                x += 10;
            } break;
        }
    }
}

u16 lcd_color565(u32 RGB) {
  u8  r, g, b;
  b = ( RGB >> (0+3) ) & 0x1f;// 取B色的高5位
  g = ( RGB >> (8+2) ) & 0x3f;// 取G色的高6位
  r = ( RGB >> (16+3)) & 0x1f;// 取R色的高5位
  return( (r<<11) + (g<<5) + (b<<0) );
}

//定义一个 共用体 + 结构体 的方法可以实现RGB值的合并与分离
typedef union {
    struct {
        u16 r : 5;
        u16 g : 6;
        u16 b : 5;
    } rgb;
    u16 rgb565;
} color565;

/****************************************************************************
*函数名：AlphaBlend
*功能  : 将两个颜色值混合
*输入  ：新fr_c叠加上去的颜色值，bk_c背景颜色基值，alpha透明度：0为不透明，即bk_c值。255为全透明，即fr_c值
*创建时间：2011.11.23
*说明  ：
*****************************************************************************/
u16 alpha_blend(u16 fr_c,u16 bk_c,u8 alpha) {
    color565 color, color_fr_c,color_bk_c ;
    color_fr_c.rgb565 = fr_c;
    color_bk_c.rgb565 = bk_c; 
    color.rgb.r = ( color_fr_c.rgb.r * alpha + color_bk_c.rgb.r * (0xff-alpha) ) >> 8;
    color.rgb.g = ( color_fr_c.rgb.g * alpha + color_bk_c.rgb.g * (0xff-alpha) ) >> 8;
    color.rgb.b = ( color_fr_c.rgb.b * alpha + color_bk_c.rgb.b * (0xff-alpha) ) >> 8;
    return color.rgb565;
}

//你的算法本身可以更优化,下面是我做的
u16 make_alpha16(u32 c1, u32 c2, u8 al) {
    c1 = (c1 | c1<<16) & 0x07e0f81f;
    c2 = (c2 | c2<<16) & 0x07e0f81f;
    c1 = ((c2-c1)*al/32 + c1) & 0x07e0f81f;
    return (u16)(c1|(c1>>16));
}

