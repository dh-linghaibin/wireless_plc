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
    FSMC_SRAM_Init();//��ʼ���ⲿSRAM
    my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);	//��ʼ��CCM�ڴ��
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
    u8 res=f_mount(&fs,"1:",1);                 //����FLASH.    
    if(res==0X0D)//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
    {
        printf("Flash Disk Formatting...\n");    //��ʽ��FLASH
        res=f_mkfs("1:",1,4096);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
        if(res==0)
        {
            f_setlabel((const TCHAR *)"1:ALIENTEK");    //����Flash���̵�����Ϊ��ALIENTEK
            printf("Flash Disk Format Finish\n");    //��ʽ�����
        } else { 
            printf("Flash Disk Format Error \n");    //��ʽ��ʧ��
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
//    while((lwipdev.dhcpstatus!=2)&&(lwipdev.dhcpstatus!=0XFF))//�ȴ�DHCP��ȡ�ɹ�/��ʱ���
//	{
//		lwip_periodic_handle();
//	}
    while(1) {
        delay_ms(300);
        int x = xpt2046_get_x();
       // printf("���� %d \n",x);
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
