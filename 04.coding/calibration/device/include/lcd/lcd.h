/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */
#ifndef _LCD_H_
#define _LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h" 

#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

typedef enum {
    F_7X8 = 0,
    F_7X8B,
    F_6X8,
    F_8X16,
    F_8X15,
    F_10X16,
} font_type;

#define RGB565CONVERT(red, green, blue)\
(uint16_t)( (( red   >> 3 ) << 11 ) | \
(( green >> 2 ) << 5  ) | \
( blue  >> 3 ))

void lcd_init(void);
void monitor_flush(int32_t sx, int32_t sy, int32_t ex, int32_t ey, const u16 *color_p);

u16 lcd_color565(u32 RGB);
void lcd_clolor_box(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color);
void lcd_string(font_type font,uint16_t sx,uint16_t sy,char * str,uint16_t color, uint16_t bg);
u16 alpha_blend(u16 fr_c,u16 bk_c,u8 alpha);


void block_write(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend);
void write_data(u16 tem_data);

#ifdef __cplusplus
}
#endif

#endif
