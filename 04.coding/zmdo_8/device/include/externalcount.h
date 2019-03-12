#ifndef __EXTERNALCOUNT_H
#define __EXTERNALCOUNT_H

/* ����ͷ�ļ� *****************************************************************/
#include "stm32f0xx.h"

/* �ⲿ�������� ***************************************************************/
typedef struct {
	GPIO_TypeDef* 	port;
	uint16_t        pin;
        uint16_t         val;
} Stdexternalcount_pin_TypeDef;

typedef struct {
    Stdexternalcount_pin_TypeDef XC1;
    Stdexternalcount_pin_TypeDef XC2;
    Stdexternalcount_pin_TypeDef XC3;
    Stdexternalcount_pin_TypeDef XC4;
    Stdexternalcount_pin_TypeDef XC5;
    Stdexternalcount_pin_TypeDef XC6;
    Stdexternalcount_pin_TypeDef XC7;
    Stdexternalcount_pin_TypeDef XC8;
}Stdexternalcount_n; 

typedef struct {   
    Stdexternalcount_n externalcount_n;
    void (*Init)(Stdexternalcount_n* externalcount);
    void (*read)(Stdexternalcount_n* externalcount,uint8_t num);
}Stdexternalcount; 
/* �ⲿ������ *****************************************************************/
/* �ⲿ������ *****************************************************************/
/* �ⲿ���� *******************************************************************/
/* �������� *******************************************************************/
void ExternalcountInit(Stdexternalcount_n* externalcount);
void ExternalcountRead(Stdexternalcount_n* externalcount,uint8_t num);

#endif
/***************************************************************END OF FILE****/
