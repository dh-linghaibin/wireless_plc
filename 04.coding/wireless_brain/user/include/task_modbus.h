/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _TSAK_MODBUS_H_
#define _TSAK_MODBUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define REG_INPUT_START       0// ����Ĵ�����ʼ��ַ
#define REG_INPUT_NREGS       99// ����Ĵ�������

#define REG_HOLDING_START     0// ���ּĴ�����ʼ��ַ
#define REG_HOLDING_NREGS     99// ���ּĴ�������

#define REG_COILS_START       0// ��Ȧ��ʼ��ַ
#define REG_COILS_SIZE        99// ��Ȧ����

#define REG_DISCRETE_START    0// ���ؼĴ�����ʼ��ַ
#define REG_DISCRETE_SIZE     99// ���ؼĴ�������

void task_modbus_init(void);
void task_modbus_set_coil(uint16_t address,uint8_t val);
void task_modbus_set_coil_bit(uint16_t address,uint8_t num,uint8_t val);
void task_modbus_set_input(uint16_t address,uint8_t val);
void task_modbus_set_input_bit(uint16_t address,uint8_t num,uint8_t val);
uint8_t task_modbus_get_input(uint16_t address);
uint8_t task_modbus_get_input_bit(uint16_t address,uint8_t num);
void task_modbus_set_holding(uint16_t adr,uint16_t dat);
uint16_t task_modbus_get_holding(uint16_t adr);
uint8_t task_modbus_get_coil_bit(uint16_t address,uint8_t num);

#ifdef __cplusplus
}
#endif

#endif


