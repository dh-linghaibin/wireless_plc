/* 引用头文件 *****************************************************************/
#include "wdog.h"
/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
/* 私有动作宏 *****************************************************************/
/* 私有变量 *******************************************************************/
/* 全局变量 */
/* 私有函数声明 ***************************************************************/
/* 私有函数  ******************************************************************/
/* 变量 ----------------------------------------------------------------------*/
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
    /* 从新导入IWDG计数器 */
    fwdgt_counter_reload();
}
/***************************************************************END OF FILE****/
