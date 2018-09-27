/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _TFT_H_
#define _TFT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"

#define BLACK       0x0000      
#define NAVY        0x000F      
#define DARKGREEN   0x03E0      
#define DARKCYAN    0x03EF      
#define MAROON      0x7800      
#define PURPLE      0x780F      
#define OLIVE       0x7BE0      
#define LIGHTGREY   0xC618      
#define DARKGREY    0x7BEF      
#define BLUE        0x001F      
#define GREEN       0x07E0      
#define CYAN        0x07FF      
#define RED         0xF800     
#define MAGENTA     0xF81F      
#define YELLOW      0xFFE0      
#define WHITE       0xFFFF      
#define ORANGE      0xFD20      
#define GREENYELLOW 0xAFE5     
#define PINK        0xF81F


//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	 

#define DFT_SCAN_DIR  D2U_L2R  //Ĭ�ϵ�ɨ�跽��

//void tft_init(const char* name);
void tft_init(void);
void tft_set_backlight(uint8_t cmd);
void tft_set(uint8_t cmd);

void lcd_set_dir(u8 dir);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void lcd_fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void monitor_flush(int32_t sx, int32_t sy, int32_t ex, int32_t ey, const u16 *color_p);

void fold_line_draw(uint16_t s_x,uint16_t x_y,uint16_t line_length,uint16_t date,uint32_t line_color,uint32_t back_color);
void boxing_com(u32 start_x,u32 start_y,u32 end_x,u32 date,u32 setting_color);

#ifdef __cplusplus
}
#endif

#endif


