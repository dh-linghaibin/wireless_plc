#ifndef __OUTSIGNAL_H
#define __OUTSIGNAL_H

/* 引用头文件 *****************************************************************/
#include "stm32f0xx.h"

/* 外部数据类型 ***************************************************************/
typedef struct {
	GPIO_TypeDef* 	port;
	uint16_t        pin;
} Stdoutsignal_pin_TypeDef;

typedef struct {
    Stdoutsignal_pin_TypeDef DO1;
    Stdoutsignal_pin_TypeDef DO2;
    Stdoutsignal_pin_TypeDef DO3;
    Stdoutsignal_pin_TypeDef DO4;
    Stdoutsignal_pin_TypeDef DO5;
    Stdoutsignal_pin_TypeDef DO6;
    Stdoutsignal_pin_TypeDef DO7;
    Stdoutsignal_pin_TypeDef DO8;
    
    Stdoutsignal_pin_TypeDef urgent;
    uint8_t all_pin_var;//所有的口状态
    uint8_t coil_val;//线圈
    uint8_t stop_flag;//急停标志
}Stdoutsignal_n; 

typedef struct {   
    Stdoutsignal_n outsignal_n;
    void (*Init)(Stdoutsignal_n* outsignal);
    void (*setout)(Stdoutsignal_n* outsignal,uint8_t num,uint8_t var);
    uint8_t (*readout)(Stdoutsignal_n* outsignal,uint8_t num);
    uint8_t (*readstop)(Stdoutsignal_n* outsignal);
}Stdoutsignal; 
/* 外部常数宏 *****************************************************************/
/* 外部动作宏 *****************************************************************/
/* 外部变量 *******************************************************************/
/* 函数声明 *******************************************************************/
void OutsignedInit(Stdoutsignal_n* outsignal);
void OutsignalSetout(Stdoutsignal_n* outsignal,uint8_t num,uint8_t var);
uint8_t OutsignalReadout(Stdoutsignal_n* outsignal,uint8_t num);
uint8_t OutsignalEmergencyStop(Stdoutsignal_n* outsignal);

#endif
/***************************************************************END OF FILE****/
