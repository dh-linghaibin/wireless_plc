/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef __FLASH_H
#define __FLASH_H

#include "gd32f10x.h"

typedef enum {
    C_FLAG = 0,
    C_1MA_BZ = 1,
    C_10MA_BZ = 2,
    C_VAL_BZ = 3,
    C_CUR_BZ = 3+16,
    C_PT100_BZ = 3+16+16,
    C_PT1000_BZ = 3+16+16+1,
}flash_code_e;

int flash_read(uint32_t address,uint32_t *read_data) ;
int flash_write(uint32_t address,uint32_t data);

#endif
/***************************************************************END OF FILE****/
