/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#ifndef _CAN_H_
#define _CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx.h"
//10k 300   1_13  2_4
//20k 150
//50k 60
//125k 24
//250k  12
//500  6
//800 
//1000 3
#define BR_10K 300
#define BR_20K 150
#define BR_50K 60
#define BR_125K 24
#define BR_250K 12
#define BR_500K 6
#define BR_800K  4
#define BR_1000K 3
    
void bxcan_init(void);
void bxcan_send(CanTxMsg send_msg);
void bxcan_set_id(uint8_t id);
void bxcan_set_br(uint8_t br);
uint8_t bxcan_get_id(void);

#ifdef __cplusplus
}
#endif

#endif

