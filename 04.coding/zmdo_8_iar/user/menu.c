/* ����ͷ�ļ� *****************************************************************/
#include "menu.h"
/* ˽���������� ***************************************************************/
/* ˽�г����� *****************************************************************/
/* ˽�ж����� *****************************************************************/
/* ˽�б��� *******************************************************************/
/* ȫ�ֱ��� */
/* ˽�к������� ***************************************************************/
/* ˽�к���  ******************************************************************/
void MenuAdd(StdMenu_n* menu_n) {
    menu_n->menu_page++;
}

void MenuSub(StdMenu_n* menu_n) {
    if(menu_n->menu_page > 0)
        menu_n->menu_page--;
}

uint8_t MenuGet(StdMenu_n* menu_n) {
    return menu_n->menu_page;
}

void MenuSetVar(StdMenu_n* menu_n,uint8_t var) {
    menu_n->m_var = var;
    menu_n->lcd_out_num = 0;//����������ٴβ��˳�
}

uint8_t MenuGetVar(StdMenu_n* menu_n) {
    return menu_n->m_var;
}
/***************************************************************END OF FILE****/