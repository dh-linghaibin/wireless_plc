/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h" 

void simulation_init(void);
void simulation_set_24v(uint8_t val);
void simulation_set_enab(uint8_t val);
void simulation_set_venab(uint8_t val);
void simulation_set_pt100(uint8_t val);
void simulation_set_pt1000(uint8_t val);
void simulation_set_ntc10k(uint8_t val);
void simulation_set_ad(uint8_t val);

#ifdef __cplusplus
}
#endif

#endif


