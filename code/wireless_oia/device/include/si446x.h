/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _SI446X_H_
#define _SI446X_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32f10x.h"
#include "SI446X_defs.h"

#define GLOBAL_XO_TUNE(x)                 SI446X_SET_PROPERTY_1(0x0000, x)
#define GLOBAL_CLK_CFG(x)                 SI446X_SET_PROPERTY_1(0x0001, x)
#define GLOBAL_LOW_BATT_THRESH(x)         SI446X_SET_PROPERTY_1(0x0002, x)
#define GLOBAL_CONFIG(x)                  SI446X_SET_PROPERTY_1(0x0003, x)
#define GLOBAL_WUT_CONFIG(x)              SI446X_SET_PROPERTY_1(0x0004, x)
#define GLOBAL_WUT_M_15_8(x)              SI446X_SET_PROPERTY_1(0x0005, x)
#define GLOBAL_WUT_M_7_0(x)               SI446X_SET_PROPERTY_1(0x0006, x)
#define GLOBAL_WUT_R(x)                   SI446X_SET_PROPERTY_1(0x0007, x)
#define GLOBAL_WUT_LDC(x)                 SI446X_SET_PROPERTY_1(0x0008, x)
#define GLOBAL_WUT_CAL(x)                 SI446X_SET_PROPERTY_1(0x0009, x)

// INT_CTL(0x01)
#define INT_CTL_ENABLE(x)                 SI446X_SET_PROPERTY_1(0x0100, x)
#define INT_CTL_PH_ENABLE(x)              SI446X_SET_PROPERTY_1(0x0101, x)
#define INT_CTL_MODEM_ENABLE(x)           SI446X_SET_PROPERTY_1(0x0102, x)
#define INT_CTL_CHIP_ENABLE(x)            SI446X_SET_PROPERTY_1(0x0103, x)

//group 0x02, FRR_CTL
#define FRR_CTL_A_MODE(x)                 SI446X_SET_PROPERTY_1(0x0200, x)
#define FRR_CTL_B_MODE(x)                 SI446X_SET_PROPERTY_1(0x0201, x)
#define FRR_CTL_C_MODE(x)                 SI446X_SET_PROPERTY_1(0x0202, x)
#define FRR_CTL_D_MODE(x)                 SI446X_SET_PROPERTY_1(0x0203, x)

#define  PACKET_LENGTH      10 //0-64, if = 0: variable mode, else: fixed mode

//1 GND 输入 地线，连接到电源参考地
//2 SDN 输入 模块工作使能控制引脚，工作时为低电平（详见 SI4463 手册）
//3 GPIO3 输出 连接模块内部射频开关的发射，可不连接，由 SI4463 智能控制?
//4 GPIO2 输出 连接模块内部射频开关的接收，可不连接，由 SI4463 智能控制
//5 nSEL 输入 模块片选引脚，用于开始一个 SPI 通信
//6 MOSI 输入 模块 SPI 数据输入引脚
//7 MISO 输出 模块 SPI 数据输出引脚
//8 ENT 输入 射频时钟（晶振）使能（高电平打开，低电平关断）
//9 SCK 输出 模块 SPI 时钟引脚
//10 IRQ 输出 模块中断引脚

#define PORT_SI_CSN     GPIOC
#define PIN_SI_CSN      GPIO_PIN_6

#define PORT_SI_SDN     GPIOB
#define PIN_SI_SDN      GPIO_PIN_12

//#define PORT_SI_GIO0    GPIOB
//#define PIN_SI_GIO0     GPIO_Pin_1

//#define PORT_SI_GIO1    GPIOB
//#define PIN_SI_GIO1     GPIO_Pin_2

#define SI_CSN_LOW()   gpio_bit_write(PORT_SI_CSN, PIN_SI_CSN,RESET); delay_us(500)
#define SI_CSN_HIGH()  gpio_bit_write(PORT_SI_CSN, PIN_SI_CSN,SET); delay_us(500)

#define SI_SDN_LOW()   gpio_bit_write(PORT_SI_SDN, PIN_SI_SDN,RESET); delay_us(500)
#define SI_SDN_HIGH()  gpio_bit_write(PORT_SI_SDN, PIN_SI_SDN,SET); delay_us(500)

void SI446X_CHANGE_STATE(uint8_t NewState);
void SI446X_RX_FIFO_RESET(void);
void SI446X_TX_FIFO_RESET(void);
void SI446X_START_TX(uint8_t channel, uint8_t condition, uint16_t tx_len);
void SI446X_GPIO_CONFIG(uint8_t G0, uint8_t G1, uint8_t G2, uint8_t G3,
                        uint8_t IRQ, uint8_t SDO, uint8_t GEN_CONFIG);
uint8_t SI446X_GET_DEVICE_STATE(void);

void RF_Initial(uint8_t mode);
void si446x_init(void);

#ifdef __cplusplus
}
#endif

#endif


