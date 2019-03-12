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

#include "gd32f10x.h"

void bxcan_init(uint16_t br);
void bxcan_send(can_trasnmit_message_struct send_msg);
void bxcan_set_id(uint8_t id);
void bxcan_set_br(uint16_t br);
uint8_t bxcan_get_id(void);

#ifdef __cplusplus
}
#endif

#endif

