#ifndef __MENU_H
#define __MENU_H

/* 引用头文件 *****************************************************************/
#include "stm32f0xx.h"
#include "can.h"
/* 外部数据类型 ***************************************************************/
typedef struct {   
    uint8_t menu_page;//第几层
    uint8_t choice_num;//主页标志
    uint8_t m_var;
    uint8_t lcd_out_num;//闪速几次退出
    uint8_t message_packet_num;//通讯包次数
}StdMenu_n; 

typedef struct {  
    StdMenu_n menu_n;
    void (*addmenu)(StdMenu_n* menu_n);
    void (*submenu)(StdMenu_n* menu_n);
    uint8_t (*getmenu)(StdMenu_n* menu_n);
    
    void (*setvar)(StdMenu_n* menu_n,uint8_t var);
    uint8_t (*getvar)(StdMenu_n* menu_n);
    
}StdMenu; 
/* 外部常数宏 *****************************************************************/
/* 外部动作宏 *****************************************************************/
/* 外部变量 *******************************************************************/
/* 函数声明 *******************************************************************/
void MenuAdd(StdMenu_n* menu_n);
void MenuSub(StdMenu_n* menu_n);
uint8_t MenuGet(StdMenu_n* menu_n);

void MenuSetVar(StdMenu_n* menu_n,uint8_t var);
uint8_t MenuGetVar(StdMenu_n* menu_n);

#endif
/***************************************************************END OF FILE****/