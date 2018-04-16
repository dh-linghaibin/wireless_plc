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

#include "app_configure.h"
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

//1 GND ���� ���ߣ����ӵ���Դ�ο���
//2 SDN ���� ģ�鹤��ʹ�ܿ������ţ�����ʱΪ�͵�ƽ����� SI4463 �ֲᣩ
//3 GPIO3 ��� ����ģ���ڲ���Ƶ���صķ��䣬�ɲ����ӣ��� SI4463 ���ܿ���?
//4 GPIO2 ��� ����ģ���ڲ���Ƶ���صĽ��գ��ɲ����ӣ��� SI4463 ���ܿ���
//5 nSEL ���� ģ��Ƭѡ���ţ����ڿ�ʼһ�� SPI ͨ��
//6 MOSI ���� ģ�� SPI ������������
//7 MISO ��� ģ�� SPI �����������
//8 ENT ���� ��Ƶʱ�ӣ�����ʹ�ܣ��ߵ�ƽ�򿪣��͵�ƽ�ضϣ�
//9 SCK ��� ģ�� SPI ʱ������
//10 IRQ ��� ģ���ж�����

#define PORT_SI_CSN     GPIOE
#define PIN_SI_CSN      GPIO_Pin_6

#define PORT_SI_SDN     GPIOC
#define PIN_SI_SDN      GPIO_Pin_13

//#define PORT_SI_GIO0    GPIOB
//#define PIN_SI_GIO0     GPIO_Pin_1

//#define PORT_SI_GIO1    GPIOB
//#define PIN_SI_GIO1     GPIO_Pin_2

#define SI_CSN_LOW()   GPIO_ResetBits(PORT_SI_CSN, PIN_SI_CSN); delay_us(500)
#define SI_CSN_HIGH()  GPIO_SetBits(PORT_SI_CSN, PIN_SI_CSN);delay_us(500)

#define SI_SDN_LOW()   GPIO_ResetBits(PORT_SI_SDN, PIN_SI_SDN);delay_us(500)
#define SI_SDN_HIGH()  GPIO_SetBits(PORT_SI_SDN, PIN_SI_SDN);delay_us(500)

void RF_Initial(uint8_t mode);
void si446x_init(void);

#ifdef __cplusplus
}
#endif

#endif


