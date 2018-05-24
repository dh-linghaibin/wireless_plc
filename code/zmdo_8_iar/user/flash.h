#ifndef __FLASH_H
#define __FLASH_H

/* 引用头文件 *****************************************************************/
#include "stm32f0xx.h"

/* 宏定义 --------------------------------------------------------------------*/
#define FLASH_PAGE_SIZE           ((uint16_t)0x400)         //页大小   - 1K
#define FLASH_TYPE_LENGTH         ((uint16_t)0x002)         //类型大小 - 2字节
#define FLASH_PAGE_LENGTH         (FLASH_PAGE_SIZE/FLASH_TYPE_LENGTH)
#define FLAG_OK                   0x00
#define FLAG_NOOK                 0x01
#define FLASH_ADDR_FLAG(x)           0x08003c00+x*2     //FLASH地址 - 标志位0x08003000
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* 外部数据类型 ***************************************************************/
typedef struct {   
    void (*write)(uint16_t* pBuffer, uint32_t WriteAddr, uint16_t nWord);
    void (*read)(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord);
    uint16_t (*readbit)(uint32_t ReadAddr);
}Stdflash; 
/* 外部常数宏 *****************************************************************/
/* 外部动作宏 *****************************************************************/
/* 外部变量 *******************************************************************/
/* 函数声明 *******************************************************************/
void FLASH_WriteNWord(uint16_t* pBuffer, uint32_t WriteAddr, uint16_t nWord);
void FLASH_ReadNWord(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord);
uint16_t FLASH_ReadNBit(uint32_t ReadAddr);

#endif
/***************************************************************END OF FILE****/
