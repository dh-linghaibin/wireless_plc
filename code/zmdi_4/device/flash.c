/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

/* ����ͷ�ļ� *****************************************************************/
#include "flash.h"

#define FLASH_PAGE_SIZE           ((uint16_t)1024)         //ҳ��С   - 1K 1024
#define FLASH_TYPE_LENGTH         ((uint16_t)0x002)         //���ʹ�С - 2�ֽ�
#define FLASH_PAGE_LENGTH         (FLASH_PAGE_SIZE/FLASH_TYPE_LENGTH)
#define FLAG_OK                   0x00
#define FLAG_NOOK                 0x01
#define FLASH_ADDR_FLAG(x)        0x08003C00+x*2//FLASH��ַ - ��־λ
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
    uint32_t NbrOfPage = 0x00;                     //ҳ������д��
    
    uint32_t length_beyond_start;                  //��ʼҳ�����ĳ���(����)
    uint32_t length_remain_start;                  //��ʼҳʣ��ĳ���(����)
    
    uint32_t addr_first_page;                      //��һҳ(��ʼ)��ַ
    uint32_t addr_last_page;                       //���ҳ(��ʼ)��ַ
    uint16_t *pbuf_tmp;                            //bufָ��
    uint16_t cnt_length;                           //���� - ����
    uint16_t cnt_page;                             //���� - ҳ��
    uint32_t prog_addr_start;                      //��̵�ַ
    uint32_t length_beyond_last;                   //���ҳ�����ĳ���(����)
    uint8_t  flag_last_page_fill;                  //���һҳװ����־
    
    
    length_beyond_start = ((WriteAddr % FLASH_PAGE_SIZE) / FLASH_TYPE_LENGTH);
    length_remain_start = FLASH_PAGE_LENGTH - length_beyond_start;
    addr_first_page     = WriteAddr - (WriteAddr % FLASH_PAGE_SIZE);
    
    /* ����(д����)��ҳ�� */
    if(0 == (length_beyond_start + nWord)%FLASH_PAGE_LENGTH) {
        flag_last_page_fill = FLAG_OK;               //���һҳ�պ�
        NbrOfPage = (length_beyond_start + nWord) / FLASH_PAGE_LENGTH;
    } else {
        flag_last_page_fill = FLAG_NOOK;             //???��ҳ
        NbrOfPage = (length_beyond_start + nWord) / FLASH_PAGE_LENGTH + 1;
    }
    
    /* ���� */
    FLASH_Unlock();
    
    /* �����־λ */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    
    /* ������һҳ */
    flash_read(buf_tmp, addr_first_page, FLASH_PAGE_LENGTH);
    FLASHStatus = FLASH_ErasePage(addr_first_page);
    /* ֻ��1ҳ */
    if(1 == NbrOfPage) {
        pbuf_tmp = pBuffer;                          //���Ƶ�ַ(ָ��)
        for(cnt_length=length_beyond_start; cnt_length<(length_beyond_start + nWord); cnt_length++) {
            buf_tmp[cnt_length] = *pbuf_tmp;
            pbuf_tmp++;
        }
        flash_page_write(buf_tmp, addr_first_page);
    } else {
        /* ��һҳ */
        pbuf_tmp = pBuffer;
        for(cnt_length=length_beyond_start; cnt_length<FLASH_PAGE_LENGTH; cnt_length++) {
            buf_tmp[cnt_length] = *pbuf_tmp;
            pbuf_tmp++;
        }
        flash_page_write(buf_tmp, addr_first_page);
        
        /* ���һҳ�պ�װ�������ö�ȡ���һҳ���� */
        if(FLAG_OK == flag_last_page_fill) {
            for(cnt_page=1; (cnt_page<NbrOfPage)  && (FLASHStatus == FLASH_COMPLETE); cnt_page++) {
                //�����̵�ַΪ�ֽڵ�ַ(��FLASH_PAGE_SIZE)
                prog_addr_start = addr_first_page + cnt_page*FLASH_PAGE_SIZE;
                FLASHStatus = FLASH_ErasePage(prog_addr_start);
                //(cnt_page - 1):Ϊ��һҳ��ַ
                flash_page_write((pBuffer + length_remain_start + (cnt_page - 1)*FLASH_PAGE_LENGTH), prog_addr_start);
            }
        } else {
            /* �м�ҳ */
            for(cnt_page=1; (cnt_page<(NbrOfPage - 1))  && (FLASHStatus == FLASH_COMPLETE); cnt_page++)
            {                                          //�����̵�ַΪ�ֽڵ�ַ(��FLASH_PAGE_SIZE)
                prog_addr_start = addr_first_page + cnt_page*FLASH_PAGE_SIZE;
                FLASHStatus = FLASH_ErasePage(prog_addr_start);
                //(cnt_page - 1):Ϊ��һҳ��ַ
                flash_page_write((pBuffer + length_remain_start + (cnt_page - 1)*FLASH_PAGE_LENGTH), prog_addr_start);
            }
            
            /* ���һҳ */
            addr_last_page = addr_first_page + (NbrOfPage - 1)*FLASH_PAGE_SIZE;
            
            flash_read(buf_tmp, addr_last_page, FLASH_PAGE_LENGTH);
            FLASHStatus = FLASH_ErasePage(addr_last_page);
            //NbrOfPage - 2: ��ҳ + ���һҳ ����ҳ(-2)
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



////���úõĵ����Ƽ����ж�����
uint32_t FLASH_START_ADDR = 0x08000000+1024*15; //1024   һҳ��С1k  ,   63   flash��64K�����һҳ�׵�ַ��ʼд
/*****************************************************************************
���ܣ�flashд

���룺addr:д��ַ 0-һҳ��С  data ��4�ֽ�����

���أ���
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
���ܣ���flash

���룺����ַ

���أ�������4�ֽ�����
******************************************************************************/
uint32_t ReadFlash(uint16_t addr)
{
    uint32_t value;
    value = *(uint32_t*)(FLASH_START_ADDR+(addr*4));
    return value;
}

/***************************************************************END OF FILE****/
