/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _DELAY_H_
#define _DELAY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"
#include "stm32f4x7_eth.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define LAN8720_PHY_ADDRESS 0x01 /* Relative to STM324xG-EVAL Board */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void  eth_init(void);
    
#ifdef __cplusplus
}
#endif

#endif


