/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

/* ����ͷ�ļ� *****************************************************************/
#include "flash.h"
/* ˽���������� ***************************************************************/
/* ˽�г����� *****************************************************************/
/* ˽�ж����� *****************************************************************/
/* ˽�б��� *******************************************************************/
/* ȫ�ֱ��� */
/* ˽�к������� ***************************************************************/
/* ˽�к���  ******************************************************************/
/* ���� ----------------------------------------------------------------------*/
#define PAGE_SIZE        ((uint32_t)(1024))                   /* һҳ���ֽ��� */
#define FLASH_START        ((uint32_t)(0x08000000 + 0x0c800)) /* flash��ȡ��ַ62k */
static uint32_t page_merry[PAGE_SIZE/4];                   /* �ڴ滺�� */

static uint32_t flash_read32(uint32_t address) {
    uint32_t temp1,temp2;
    temp1=*(__IO uint16_t*)address; 
    temp2=*(__IO uint16_t*)(address+2); 
    return (temp2<<16)+temp1;
}


int flash_write(uint32_t address,uint32_t data) {
    uint16_t read_i = 0;
    uint32_t addr  = 0;
    uint8_t page_num = 0;
    uint16_t page_offset = 0;

    fmc_unlock();                          /* unlock the flash program/erase controller */
    page_num = (address/PAGE_SIZE);          /* ����ڼ�ҳ */
    addr = (page_num*1024 + FLASH_START); /* ����� */
    /* ��ȡҳ�������� */
    do {
        page_merry[read_i] = flash_read32(addr);
        addr+=4;
    } while(++read_i < 256);
        
    fmc_page_erase(page_num*1024 + FLASH_START); /* ���� */
    page_offset = address%PAGE_SIZE;
    page_merry[page_offset] = data;
    addr = (page_num*1024 + FLASH_START);
    read_i = 0;
    do{
        fmc_word_program(addr,page_merry[read_i]);
        addr += 4;
    }while(++read_i < 256);
    
    fmc_lock(); /* lock the main FMC operation */
    return 0;
}

int flash_read(uint32_t address,uint32_t *read_data) {
    uint32_t addr = (address/PAGE_SIZE)*1024 + FLASH_START + (address%PAGE_SIZE) * 4;
    *read_data = flash_read32(addr);
    return 0; 
}

/***************************************************************END OF FILE****/
