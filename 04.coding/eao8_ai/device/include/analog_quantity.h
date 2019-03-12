/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _ANALOG_QUANTITY_H_
#define _ANALOG_QUANTITY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"
#include "read_config.h"

void analog_quantity_init(void);
void analog_quantity_set_ch_mode(ads_read_mode mode,check_channel passageway);
float analog_quantity_get(check_channel ch);
void analog_quantity_calibration(uint8_t * cmd);
void analog_quantity_tic_100ms( void );

uint8_t analog_quantity_calibration_flag_get( void );

#ifdef __cplusplus
}
#endif

#endif


