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
#include "gui_demo.h"
//LWIP
#include "lan8720.h"
//lua
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "string.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
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

//FRESULT scan_files (
//    char* path        /* Start node to be scanned (***also used as work area***) */
//)
//{
//    FRESULT res;
//    DIR dir;
//    UINT i;
//    static FILINFO fno;

//    res = f_opendir(&dir, path);                       /* Open the directory */
//    if (res == FR_OK) {
//        for (;;) {
//            res = f_readdir(&dir, &fno);                   /* Read a directory item */
//            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
//            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
//                i = strlen(path);
//                sprintf(&path[i], "/%s", fno.fname);
//                res = scan_files(path);                    /* Enter the directory */
//                if (res != FR_OK) break;
//                path[i] = 0;
//            } else {                                       /* It is a file. */
//               // printf("%s/%s\n", path, fno.fname);
//            }
//            printf("%s/%s\n", path, fno.fname);
//        }
//        f_closedir(&dir);
//    }

//    return res;
//}

void vtask_gui_tic(void *pvParameters);

void Lua_task(void);

USB_OTG_CORE_HANDLE USB_OTG_dev;


/* 队列句柄 */  
xQueueHandle MsgQueue;    
void TaskA( void *pvParameters );  
void TaskB( void *pvParameters );  


int main(void) {
    delay_init(168);
    uart_init(115200);
    printf("%s",logo);
    tft_init();
    
    fsmc_sram_init();//初始化外部SRAM
    my_mem_init(SRAMIN);//初始化内部内存池
    my_mem_init(SRAMEX);//初始化外部内存池
    my_mem_init(SRAMCCM);//初始化CCM内存池
    SPI3_Init();
    xpt2046_init();
    
    
    W25QXX_Init();
    //W25QXX_Erase_Chip();
    FATFS fs;
    u8 res=f_mount(&fs,"1:",1);                 //挂载FLASH.    
    if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
    {
        printf("Flash Disk Formatting...\n");    //格式化FLASH
//        res=f_mkfs("1:",0,0);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇 4096
//        if(res==0)
//        {
//            f_setlabel((const TCHAR *)"1:lhb");    //设置Flash磁盘的名字为：ALIENTEK
//            printf("Flash Disk Format Finish\n");    //格式化完成
//        } else { 
//            printf("Flash Disk Format Error \n");    //格式化失败
//        }
//        delay_ms(1000);
    } else {
//        printf("Flash Disk ok \n");
//        f_mkdir("1:sub1");
//        f_mkdir("1:sub1/sub2");
//        
//        char buff[256];
//        strcpy(buff, "1:");
//        res = scan_files(buff);
        //printf("mu: %s \n",buff);
        
       // fatfs_test_mkdir();
    }
    USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
    
    lv_vdb_init();
    lv_init();
    lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/
    /*Set up the functions to access to your display*/
    disp_drv.disp_flush = monitor_flush;            /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_drv_register(&indev_drv);

    lv_disp_drv_register(&disp_drv);
    /*Create a simple base object*/
    demo_create();
    Lua_task();
  //  eth_init();
    //tcp_server_init();
    xTaskCreate(vtask_gui_tic, "ui", 512, NULL, 2, NULL);
     /* 建立队列 */  
    MsgQueue = xQueueCreate( 5 , sizeof( int16_t ) );  
    /* 建立任务 */  
    xTaskCreate( TaskA, ( signed portCHAR * ) "TaskA", 512,  
                            NULL, tskIDLE_PRIORITY+1, NULL );  
    xTaskCreate( TaskB, ( signed portCHAR * ) "TaskB", 512,  
                            NULL, tskIDLE_PRIORITY+1, NULL );  
    vTaskStartScheduler();
    while(1);
}

/*-----------------------------------------------------------*/  
void TaskA( void *pvParameters )  
{  
    int16_t SendNum = 1;  
    for( ;; )  
    {  
        vTaskDelay( 1000/portTICK_RATE_MS );  
        /* 向队列中填充内容 */  
        xQueueSend( MsgQueue, ( void* )&SendNum, 0 );  
        SendNum++;  
    }  
}  
void TaskB( void *pvParameters )  
{  
    int16_t ReceiveNum = 0;  
    for( ;; )  
    {  
        /* 从队列中获取内容 */  
        if( xQueueReceive( MsgQueue, &ReceiveNum, 100/portTICK_RATE_MS ) == pdPASS)  
        {  
            printf("ReceiveNum:%d\r\n",ReceiveNum);  
        }  
    }  
}  


void Lua_task(void)  
{   
    lua_State* L;
    L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_dofile(L,"1:test.lua")!=0)//加载sd卡中的Lua.lua并运行  
          printf(lua_tostring(L,-1));
}  

void vtask_gui_tic(void *pvParameters) {
    while(1) {
        vTaskDelay(1 / portTICK_RATE_MS);
        lv_tick_inc(1);
        lv_task_handler();
        xpt2046_loop();
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
