/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f0xx.h"

typedef enum {
    C_FLAG = 0,
    C_ADDR = 1,
    C_DEVICE_VAL = 2,
    C_BR = 3,
}flash_code_e;

int flash_read(uint32_t address,uint32_t *read_data) ;
int flash_write(uint32_t address,uint32_t data);

#endif
/***************************************************************END OF FILE****/
