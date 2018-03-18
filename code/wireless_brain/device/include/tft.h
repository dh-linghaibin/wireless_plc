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


//void tft_init(const char* name);
void tft_init(void);
void tft_fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void monitor_flush(int32_t sx, int32_t sy, int32_t ex, int32_t ey, const u16 *color_p);

#ifdef __cplusplus
}
#endif

#endif


