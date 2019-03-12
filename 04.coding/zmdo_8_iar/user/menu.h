#ifndef __MENU_H
#define __MENU_H

/* ����ͷ�ļ� *****************************************************************/
#include "stm32f0xx.h"
#include "can.h"
/* �ⲿ�������� ***************************************************************/
typedef struct {   
    uint8_t menu_page;//�ڼ���
    uint8_t choice_num;//��ҳ��־
    uint8_t m_var;
    uint8_t lcd_out_num;//���ټ����˳�
    uint8_t message_packet_num;//ͨѶ������
}StdMenu_n; 

typedef struct {  
    StdMenu_n menu_n;
    void (*addmenu)(StdMenu_n* menu_n);
    void (*submenu)(StdMenu_n* menu_n);
    uint8_t (*getmenu)(StdMenu_n* menu_n);
    
    void (*setvar)(StdMenu_n* menu_n,uint8_t var);
    uint8_t (*getvar)(StdMenu_n* menu_n);
    
}StdMenu; 
/* �ⲿ������ *****************************************************************/
/* �ⲿ������ *****************************************************************/
/* �ⲿ���� *******************************************************************/
/* �������� *******************************************************************/
void MenuAdd(StdMenu_n* menu_n);
void MenuSub(StdMenu_n* menu_n);
uint8_t MenuGet(StdMenu_n* menu_n);

void MenuSetVar(StdMenu_n* menu_n,uint8_t var);
uint8_t MenuGetVar(StdMenu_n* menu_n);

#endif
/***************************************************************END OF FILE****/