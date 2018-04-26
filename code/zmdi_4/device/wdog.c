/* ����ͷ�ļ� *****************************************************************/
#include "wdog.h"
/* ˽���������� ***************************************************************/
/* ˽�г����� *****************************************************************/
/* ˽�ж����� *****************************************************************/
/* ˽�б��� *******************************************************************/
/* ȫ�ֱ��� */
/* ˽�к������� ***************************************************************/
/* ˽�к���  ******************************************************************/
/* ���� ----------------------------------------------------------------------*/
void wdog_init(void) {
    /* enable IRC40K */
    rcu_osci_on(RCU_IRC40K);
  
    /* wait till IRC40K is ready */
    while(SUCCESS != rcu_osci_stab_wait(RCU_IRC40K)){}  
    /* confiure FWDGT counter clock: 40KHz(IRC40K) / 64 = 0.625 KHz */
    fwdgt_config(2*500, FWDGT_PSC_DIV256);  
    /* after 1.6 seconds to generate a reset */
    fwdgt_enable();

    /* check if the system has resumed from FWDGT reset */
    if(RESET != rcu_flag_get(RCU_FLAG_FWDGTRST)){
        /* clear the FWDGT reset flag */
        rcu_all_reset_flag_clear();
    } else {
        /* not fwdog reset */
    }
}

void wdog_reload(void) {
    /* ���µ���IWDG������ */
    fwdgt_counter_reload();
}
/***************************************************************END OF FILE****/
