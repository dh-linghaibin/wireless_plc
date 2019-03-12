#ifndef __OUTSIGNAL_H
#define __OUTSIGNAL_H

/* ����ͷ�ļ� *****************************************************************/
#include "stm32f0xx.h"

/* �ⲿ�������� ***************************************************************/
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
    uint8_t all_pin_var;//���еĿ�״̬
    uint8_t coil_val;//��Ȧ
    uint8_t stop_flag;//��ͣ��־
}Stdoutsignal_n; 

typedef struct {   
    Stdoutsignal_n outsignal_n;
    void (*Init)(Stdoutsignal_n* outsignal);
    void (*setout)(Stdoutsignal_n* outsignal,uint8_t num,uint8_t var);
    uint8_t (*readout)(Stdoutsignal_n* outsignal,uint8_t num);
    uint8_t (*readstop)(Stdoutsignal_n* outsignal);
}Stdoutsignal; 
/* �ⲿ������ *****************************************************************/
/* �ⲿ������ *****************************************************************/
/* �ⲿ���� *******************************************************************/
/* �������� *******************************************************************/
void OutsignedInit(Stdoutsignal_n* outsignal);
void OutsignalSetout(Stdoutsignal_n* outsignal,uint8_t num,uint8_t var);
uint8_t OutsignalReadout(Stdoutsignal_n* outsignal,uint8_t num);
uint8_t OutsignalEmergencyStop(Stdoutsignal_n* outsignal);

#endif
/***************************************************************END OF FILE****/
