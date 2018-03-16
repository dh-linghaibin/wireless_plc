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
#include "xpt2046.h"
#include "w25qxx.h"
#include "ff.h"
#include "malloc.h"
#include "stdio.h"
//LWIP
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"

const char logo[] = "\
/*\n\
 * This file is part of the \n\
 *\n\
 * Copyright (c) 2017-2018 linghaibin\n\
 *\n\
 */\n\
 Welcome to use device!\n";

int main(void) {
    delay_init(168);
    FSMC_SRAM_Init();//初始化外部SRAM
    my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池
	my_mem_init(SRAMCCM);	//初始化CCM内存池
    uart_init(115200);
    printf("%s",logo);
    SPI3_Init();
    xpt2046_init();
    LCD_Init();
    W25QXX_Init();
    if(W25QXX_ReadID()!=W25Q128) {
        printf("W25Q128 Check Failed! \n");
    } else {
        printf("W25Q128 Check ok! \n");
    }
    FATFS fs;
    u8 res=f_mount(&fs,"1:",1);                 //挂载FLASH.    
    if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
    {
        printf("Flash Disk Formatting...\n");    //格式化FLASH
        res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
        if(res==0)
        {
            f_setlabel((const TCHAR *)"1:ALIENTEK");    //设置Flash磁盘的名字为：ALIENTEK
            printf("Flash Disk Format Finish\n");    //格式化完成
        } else { 
            printf("Flash Disk Format Error \n");    //格式化失败
        }
        delay_ms(1000);
    } else {
        printf("Flash Disk ok \n");
    }
//    if(lwip_comm_init()!=0) {
//        printf("lwIP Init failed!");
//    } else {
//        printf("lwIP Init ok!");
//    }
//    while((lwipdev.dhcpstatus!=2)&&(lwipdev.dhcpstatus!=0XFF))//等待DHCP获取成功/超时溢出
//	{
//		lwip_periodic_handle();
//	}
    while(1) {
        delay_ms(300);
        int x = xpt2046_get_x();
       // printf("触摸 %d \n",x);
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
