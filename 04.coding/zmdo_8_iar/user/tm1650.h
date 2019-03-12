#ifndef __TM1650_H
#define __TM1650_H

/* ����ͷ�ļ� *****************************************************************/
#include "stm32f0xx.h"
/* �ⲿ�������� ***************************************************************/

typedef struct {
	GPIO_TypeDef* 	port;
	uint16_t        pin;
} Stdtm1650_pin_TypeDef;

typedef struct {   
    Stdtm1650_pin_TypeDef scl;
    Stdtm1650_pin_TypeDef sda;
    const uint8_t show_code[15];
    uint8_t key_count[11];//����״̬
    uint8_t show_merry[4];//led״̬
    uint8_t key_down_num;//�ĸ���ť��������
}Stdtm1650_n; 

typedef struct {  
    Stdtm1650_n tm1650_n;
    void (*init)(Stdtm1650_n * tm1650n);
    void (*show_nex)(Stdtm1650_n * tm1650n,uint8_t num1,uint8_t num2);
    void (*show_led)(Stdtm1650_n * tm1650n,uint8_t num,uint8_t var);
    uint8_t (*readkey)(Stdtm1650_n * tm1650n);
}Stdtm1650; 
/* �ⲿ������ *****************************************************************/
/* �ⲿ������ *****************************************************************/
/* �ⲿ���� *******************************************************************/
/* �������� *******************************************************************/
void Tm1650Init(Stdtm1650_n * tm1650n);
void Tm1650ShowNex(Stdtm1650_n * tm1650n,uint8_t num1,uint8_t num2);
void Tm1650ShowLed(Stdtm1650_n * tm1650n,uint8_t num,uint8_t var);
uint8_t TM1650ScanKey(Stdtm1650_n * tm1650n);

#endif
/***************************************************************END OF FILE****/
