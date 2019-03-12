/* 引用头文件 *****************************************************************/
#include "flash.h"
/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
/* 私有动作宏 *****************************************************************/
/* 私有变量 *******************************************************************/
/* 全局变量 */
/* 私有函数声明 ***************************************************************/
/* 私有函数  ******************************************************************/
/* 变量 ----------------------------------------------------------------------*/
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

/************************************************
函数名称 ： FLASH_PageWrite
功    能 ： 写一页FLASH
参    数 ： pBuffer ----- 数据缓冲区
WriteAddr --- 写地址
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void FLASH_PageWrite(uint16_t *pBuffer, uint32_t WriteAddr)
{
    uint16_t cnt_tmp;
    for(cnt_tmp=0; (cnt_tmp<FLASH_PAGE_LENGTH) && (FLASHStatus == FLASH_COMPLETE); cnt_tmp++)
    {
        FLASHStatus = FLASH_ProgramHalfWord(WriteAddr, *pBuffer);
        WriteAddr += 2;
        pBuffer++;
    }
}

/************************************************
函数名称 ： FLASH_WriteNWord
功    能 ： 写Flash
参    数 ： pBuffer ----- 数据缓冲区
WriteAddr --- 写地址
nWord ------- 长度
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void FLASH_WriteNWord(uint16_t *pBuffer, uint32_t WriteAddr, uint16_t nWord)
{
    static uint16_t buf_tmp[FLASH_PAGE_LENGTH];
    uint32_t NbrOfPage = 0x00;                     //页数（读写）
    
    uint32_t length_beyond_start;                  //开始页超出的长度(半字)
    uint32_t length_remain_start;                  //开始页剩余的长度(半字)
    
    uint32_t addr_first_page;                      //第一页(起始)地址
    uint32_t addr_last_page;                       //最后页(起始)地址
    uint16_t *pbuf_tmp;                            //buf指针
    uint16_t cnt_length;                           //计数 - 长度
    uint16_t cnt_page;                             //计数 - 页数
    uint32_t prog_addr_start;                      //编程地址
    uint32_t length_beyond_last;                   //最后页超出的长度(半字)
    uint8_t  flag_last_page_fill;                  //最后一页装满标志
    
    
    length_beyond_start = ((WriteAddr % FLASH_PAGE_SIZE) / FLASH_TYPE_LENGTH);
    length_remain_start = FLASH_PAGE_LENGTH - length_beyond_start;
    addr_first_page     = WriteAddr - (WriteAddr % FLASH_PAGE_SIZE);
    
    /* 擦除(写操作)的页数 */
    if(0 == (length_beyond_start + nWord)%FLASH_PAGE_LENGTH)
    {
        flag_last_page_fill = FLAG_OK;               //最后一页刚好
        NbrOfPage = (length_beyond_start + nWord) / FLASH_PAGE_LENGTH;
    }
    else
    {
        flag_last_page_fill = FLAG_NOOK;             //???跨页
        NbrOfPage = (length_beyond_start + nWord) / FLASH_PAGE_LENGTH + 1;
    }
    
    /* 解锁 */
    FLASH_Unlock();
    
    /* 清除标志位 */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    
    /* 操作第一页 */
    FLASH_ReadNWord(buf_tmp, addr_first_page, FLASH_PAGE_LENGTH);
    FLASHStatus = FLASH_ErasePage(addr_first_page);
    /* 只有1页 */
    if(1 == NbrOfPage)
    {
        pbuf_tmp = pBuffer;                          //复制地址(指针)
        for(cnt_length=length_beyond_start; cnt_length<(length_beyond_start + nWord); cnt_length++)
        {
            buf_tmp[cnt_length] = *pbuf_tmp;
            pbuf_tmp++;
        }
        FLASH_PageWrite(buf_tmp, addr_first_page);
    }
    /* 大于1页 */
    else
    {
        /* 第一页 */
        pbuf_tmp = pBuffer;
        for(cnt_length=length_beyond_start; cnt_length<FLASH_PAGE_LENGTH; cnt_length++)
        {
            buf_tmp[cnt_length] = *pbuf_tmp;
            pbuf_tmp++;
        }
        FLASH_PageWrite(buf_tmp, addr_first_page);
        
        /* 最后一页刚好装满，不用读取最后一页数据 */
        if(FLAG_OK == flag_last_page_fill)
        {
            for(cnt_page=1; (cnt_page<NbrOfPage)  && (FLASHStatus == FLASH_COMPLETE); cnt_page++)
            {                                          //这里编程地址为字节地址(故FLASH_PAGE_SIZE)
                prog_addr_start = addr_first_page + cnt_page*FLASH_PAGE_SIZE;
                FLASHStatus = FLASH_ErasePage(prog_addr_start);
                //(cnt_page - 1):为下一页地址
                FLASH_PageWrite((pBuffer + length_remain_start + (cnt_page - 1)*FLASH_PAGE_LENGTH), prog_addr_start);
            }
        }
        else
        {
            /* 中间页 */
            for(cnt_page=1; (cnt_page<(NbrOfPage - 1))  && (FLASHStatus == FLASH_COMPLETE); cnt_page++)
            {                                          //这里编程地址为字节地址(故FLASH_PAGE_SIZE)
                prog_addr_start = addr_first_page + cnt_page*FLASH_PAGE_SIZE;
                FLASHStatus = FLASH_ErasePage(prog_addr_start);
                //(cnt_page - 1):为下一页地址
                FLASH_PageWrite((pBuffer + length_remain_start + (cnt_page - 1)*FLASH_PAGE_LENGTH), prog_addr_start);
            }
            
            /* 最后一页 */
            addr_last_page = addr_first_page + (NbrOfPage - 1)*FLASH_PAGE_SIZE;
            
            FLASH_ReadNWord(buf_tmp, addr_last_page, FLASH_PAGE_LENGTH);
            FLASHStatus = FLASH_ErasePage(addr_last_page);
            //NbrOfPage - 2: 首页 + 最后一页 共两页(-2)
            pbuf_tmp = pBuffer + length_remain_start + (NbrOfPage - 2)*(FLASH_PAGE_SIZE/2);
            length_beyond_last   = (nWord - length_remain_start) % FLASH_PAGE_LENGTH;
            for(cnt_length=0; cnt_length<length_beyond_last; cnt_length++)
            {
                buf_tmp[cnt_length] = *pbuf_tmp;
                pbuf_tmp++;
            }
            FLASH_PageWrite(buf_tmp, addr_last_page);
        }
    }
}

/************************************************
函数名称 ： FLASH_ReadNWord
功    能 ： 读N字
参    数 ： pBuffer ----- 数据缓冲区
ReadAddr ---- 读地址
nWord ------- 长度
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void FLASH_ReadNWord(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord)
{
    while(nWord--)
    {
        *pBuffer = (*(__IO uint16_t*)ReadAddr);
        ReadAddr += 2;
        pBuffer++;
    }
}

uint16_t FLASH_ReadNBit(uint32_t ReadAddr) {
    uint16_t pBuffer = 0;
    pBuffer = (*(__IO uint16_t*)ReadAddr );
    return pBuffer;
}

/***************************************************************END OF FILE****/
