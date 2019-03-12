#ifndef __FLASH_H
#define __FLASH_H

/* ����ͷ�ļ� *****************************************************************/
#include "stm32f0xx.h"

/* �궨�� --------------------------------------------------------------------*/
#define FLASH_PAGE_SIZE           ((uint16_t)0x400)         //ҳ��С   - 1K
#define FLASH_TYPE_LENGTH         ((uint16_t)0x002)         //���ʹ�С - 2�ֽ�
#define FLASH_PAGE_LENGTH         (FLASH_PAGE_SIZE/FLASH_TYPE_LENGTH)
#define FLAG_OK                   0x00
#define FLAG_NOOK                 0x01
#define FLASH_ADDR_FLAG(x)           0x08003c00+x*2     //FLASH��ַ - ��־λ0x08003000
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* �ⲿ�������� ***************************************************************/
typedef struct {   
    void (*write)(uint16_t* pBuffer, uint32_t WriteAddr, uint16_t nWord);
    void (*read)(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord);
    uint16_t (*readbit)(uint32_t ReadAddr);
}Stdflash; 
/* �ⲿ������ *****************************************************************/
/* �ⲿ������ *****************************************************************/
/* �ⲿ���� *******************************************************************/
/* �������� *******************************************************************/
void FLASH_WriteNWord(uint16_t* pBuffer, uint32_t WriteAddr, uint16_t nWord);
void FLASH_ReadNWord(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord);
uint16_t FLASH_ReadNBit(uint32_t ReadAddr);

#endif
/***************************************************************END OF FILE****/
