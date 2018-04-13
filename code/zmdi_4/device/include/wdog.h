/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */


#ifndef __WDOG_H
#define __WDOG_H

#ifdef __cplusplus
extern "C" {
#endif

/* 引用头文件 *****************************************************************/
#include "stm32f0xx.h"

void wdog_init(void);
void wdog_reload(void); 

#ifdef __cplusplus
}
#endif

#endif
/***************************************************************END OF FILE****/
