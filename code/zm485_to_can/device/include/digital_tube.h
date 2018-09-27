/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */


#ifndef __DIGITAL_TUBE_H
#define __DIGITAL_TUBE_H

#ifdef __cplusplus
extern "C" {
#endif

/* 引用头文件 *****************************************************************/
#include "gd32f10x.h"

void digital_tube_init(void);
void digital_tube_refresh(void);
void digital_tube_set_num(uint8_t bit,uint8_t num);
void igital_tube_toggle_point(uint8_t point_num,uint8_t val);

#ifdef __cplusplus
}
#endif

#endif
/***************************************************************END OF FILE****/
