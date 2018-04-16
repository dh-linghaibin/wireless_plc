/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

/* 引用头文件 *****************************************************************/
#include "flash.h"

#define FLASH_PAGE_SIZE           ((uint16_t)1024)         //页大小   - 1K 1024
#define FLASH_TYPE_LENGTH         ((uint16_t)0x002)         //类型大小 - 2字节
#define FLASH_PAGE_LENGTH         (FLASH_PAGE_SIZE/FLASH_TYPE_LENGTH)
#define FLAG_OK                   0x00
#define FLAG_NOOK                 0x01
#define FLASH_ADDR_FLAG(x)        0x08003C00+x*2//FLASH地址 - 标志位
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

void flash_page_write(uint16_t *pBuffer, uint32_t WriteAddr) {
    uint16_t cnt_tmp;
    for(cnt_tmp=0; (cnt_tmp<FLASH_PAGE_LENGTH) && (FLASHStatus == FLASH_COMPLETE); cnt_tmp++) {
        FLASHStatus = FLASH_ProgramHalfWord(WriteAddr, *pBuffer);
        WriteAddr += 2;
        pBuffer++;
    }
}

void flash_write(uint16_t *pBuffer, uint32_t WriteAddr, uint16_t nWord) {
    uint16_t buf_tmp[FLASH_PAGE_LENGTH];
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
    if(0 == (length_beyond_start + nWord)%FLASH_PAGE_LENGTH) {
        flag_last_page_fill = FLAG_OK;               //最后一页刚好
        NbrOfPage = (length_beyond_start + nWord) / FLASH_PAGE_LENGTH;
    } else {
        flag_last_page_fill = FLAG_NOOK;             //???跨页
        NbrOfPage = (length_beyond_start + nWord) / FLASH_PAGE_LENGTH + 1;
    }
    
    /* 解锁 */
    FLASH_Unlock();
    
    /* 清除标志位 */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    
    /* 操作第一页 */
    flash_read(buf_tmp, addr_first_page, FLASH_PAGE_LENGTH);
    FLASHStatus = FLASH_ErasePage(addr_first_page);
    /* 只有1页 */
    if(1 == NbrOfPage) {
        pbuf_tmp = pBuffer;                          //复制地址(指针)
        for(cnt_length=length_beyond_start; cnt_length<(length_beyond_start + nWord); cnt_length++) {
            buf_tmp[cnt_length] = *pbuf_tmp;
            pbuf_tmp++;
        }
        flash_page_write(buf_tmp, addr_first_page);
    } else {
        /* 第一页 */
        pbuf_tmp = pBuffer;
        for(cnt_length=length_beyond_start; cnt_length<FLASH_PAGE_LENGTH; cnt_length++) {
            buf_tmp[cnt_length] = *pbuf_tmp;
            pbuf_tmp++;
        }
        flash_page_write(buf_tmp, addr_first_page);
        
        /* 最后一页刚好装满，不用读取最后一页数据 */
        if(FLAG_OK == flag_last_page_fill) {
            for(cnt_page=1; (cnt_page<NbrOfPage)  && (FLASHStatus == FLASH_COMPLETE); cnt_page++) {
                //这里编程地址为字节地址(故FLASH_PAGE_SIZE)
                prog_addr_start = addr_first_page + cnt_page*FLASH_PAGE_SIZE;
                FLASHStatus = FLASH_ErasePage(prog_addr_start);
                //(cnt_page - 1):为下一页地址
                flash_page_write((pBuffer + length_remain_start + (cnt_page - 1)*FLASH_PAGE_LENGTH), prog_addr_start);
            }
        } else {
            /* 中间页 */
            for(cnt_page=1; (cnt_page<(NbrOfPage - 1))  && (FLASHStatus == FLASH_COMPLETE); cnt_page++)
            {                                          //这里编程地址为字节地址(故FLASH_PAGE_SIZE)
                prog_addr_start = addr_first_page + cnt_page*FLASH_PAGE_SIZE;
                FLASHStatus = FLASH_ErasePage(prog_addr_start);
                //(cnt_page - 1):为下一页地址
                flash_page_write((pBuffer + length_remain_start + (cnt_page - 1)*FLASH_PAGE_LENGTH), prog_addr_start);
            }
            
            /* 最后一页 */
            addr_last_page = addr_first_page + (NbrOfPage - 1)*FLASH_PAGE_SIZE;
            
            flash_read(buf_tmp, addr_last_page, FLASH_PAGE_LENGTH);
            FLASHStatus = FLASH_ErasePage(addr_last_page);
            //NbrOfPage - 2: 首页 + 最后一页 共两页(-2)
            pbuf_tmp = pBuffer + length_remain_start + (NbrOfPage - 2)*(FLASH_PAGE_SIZE/2);
            length_beyond_last   = (nWord - length_remain_start) % FLASH_PAGE_LENGTH;
            for(cnt_length=0; cnt_length<length_beyond_last; cnt_length++)
            {
                buf_tmp[cnt_length] = *pbuf_tmp;
                pbuf_tmp++;
            }
            flash_page_write(buf_tmp, addr_last_page);
        }
    }
}

void flash_read(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord) {
    while(nWord--) {
        *pBuffer = (*(__IO uint16_t*)ReadAddr);
        ReadAddr += 2;
        pBuffer++;
    }
}

uint16_t flash_read_bit(uint32_t ReadAddr) {
    uint16_t pBuffer = 0;
    pBuffer = (*(__IO uint16_t*)ReadAddr );
    return pBuffer;
}



////觉得好的点下推荐，感恩啦！
uint32_t FLASH_START_ADDR = 0x08000000+1024*15; //1024   一页大小1k  ,   63   flash有64K在最后一页首地址开始写
/*****************************************************************************
功能：flash写

输入：addr:写地址 0-一页大小  data ：4字节数据

返回：无
******************************************************************************/
void WriteFlash(uint32_t addr,uint32_t data)
{
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPERR);
    FLASH_ErasePage(FLASH_START_ADDR); 
    FLASH_ProgramWord(FLASH_START_ADDR+(addr*4),data);
    FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPERR);
    FLASH_Lock();
}

/*****************************************************************************
功能：读flash

输入：读地址

返回：读到的4字节数据
******************************************************************************/
uint32_t ReadFlash(uint16_t addr)
{
    uint32_t value;
    value = *(uint32_t*)(FLASH_START_ADDR+(addr*4));
    return value;
}

/***************************************************************END OF FILE****/
