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
    C_FLAG = 0x08003C00,
    C_ADDR = 0x08003C00+2,
    C_DEVICE_VAL = 0x08003C00+4,
    C_BR = 0x08003C00+6,
}flash_code_e;

void flash_read(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord);
void flash_write(uint16_t *pBuffer, uint32_t WriteAddr, uint16_t nWord);

#endif
/***************************************************************END OF FILE****/
