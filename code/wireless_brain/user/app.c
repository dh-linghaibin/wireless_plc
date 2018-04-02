/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "app_configure.h"
#include "usart.h"
#include "delay.h"
#include "sram.h"
#include "spi.h"
#include "tft.h"
#include "can.h"
#include "xpt2046.h"
#include "w25qxx.h"
#include "ff.h"
#include "malloc.h"
#include "timer.h"
#include "stdio.h"
//lvgl
#include "../lv_hal/lv_hal_disp.h"
#include "../lv_misc/lv_mem.h"
#include "../lv_core/lv_obj.h"
#include "../lvgl.h"
//LWIP
#include "lan8720.h"

//lua
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "string.h"
//freertos
#include "FreeRTOS.h"

#include "gui_demo.h"
//usb
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h" 

const char logo[] = "\
/*\n\
 * This file is part of the \n\
 *\n\
 * Copyright (c) 2017-2018 linghaibin\n\
 *\n\
 */\n\
 Welcome to use device!\n";

FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;

    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
               // printf("%s/%s\n", path, fno.fname);
            }
            printf("%s/%s\n", path, fno.fname);
        }
        f_closedir(&dir);
    }

    return res;
}

USB_OTG_CORE_HANDLE USB_OTG_dev;

int main(void) {
    delay_init(168);
    uart_init(115200);
    printf("%s",logo);
    
    fsmc_sram_init();//��ʼ���ⲿSRAM
    my_mem_init(SRAMIN);//��ʼ���ڲ��ڴ��
    my_mem_init(SRAMEX);//��ʼ���ⲿ�ڴ��
    my_mem_init(SRAMCCM);//��ʼ��CCM�ڴ��
    W25QXX_Init();
    //W25QXX_Erase_Chip();
    FATFS fs;
    u8 res=f_mount(&fs,"1:",1);                 //����FLASH.    
    if(res==0X0D)//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
    {
        printf("Flash Disk Formatting...\n");    //��ʽ��FLASH
//        res=f_mkfs("1:",0,0);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1���� 4096
//        if(res==0)
//        {
//            f_setlabel((const TCHAR *)"1:lhb");    //����Flash���̵�����Ϊ��ALIENTEK
//            printf("Flash Disk Format Finish\n");    //��ʽ�����
//        } else { 
//            printf("Flash Disk Format Error \n");    //��ʽ��ʧ��
//        }
//        delay_ms(1000);
    } else {
        printf("Flash Disk ok \n");
        f_mkdir("1:sub1");
        f_mkdir("1:sub1/sub2");
        
        char buff[256];
        strcpy(buff, "1:");
        res = scan_files(buff);
        printf("mu: %s \n",buff);
    }
    USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
    
    //eth_init();
    //tcp_server_init();
    
   // vTaskStartScheduler();
    while(1) {
    
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
