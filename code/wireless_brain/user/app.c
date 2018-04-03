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

void vtask_gui_tic(void *pvParameters);

void Lua_task(void);

USB_OTG_CORE_HANDLE USB_OTG_dev;



static int Delay_ms(lua_State *L)  
{  
    delay_ms(luaL_checkinteger(L, -1));  
    return 0;  
}  
static int print(lua_State *L)  
{  
    int n=lua_gettop(L);  
    int i;  
    for (i=1; i<=n; i++)  
    {  
        if (lua_isstring(L,i))  
            printf("%s",lua_tostring(L,i));  
        else if (lua_isnil(L,i))  
            printf("%s","nil");  
        else if (lua_isboolean(L,i))  
            printf("%s",lua_toboolean(L,i) ? "true" : "false");  
        else  
            printf("%s:%p",luaL_typename(L,i),lua_topointer(L,i));  
     }  
     return 0;  
}  
static luaL_Reg LuaLib1_0[] =   
{   
    //c�ӿں��������Է���������lua������  
    {"Delay_ms", Delay_ms},  
    {"print", print},  
  
    {NULL, NULL}   
};  


/* ���Ե�Lua�����ַ��� */  
const char lua_test[] = {   
    "print(\"Hello,I am lua!\\n--this is newline printf\")\n"  
    "function foo()\n"  
    "  local i = 0\n"  
    "  local sum = 1\n"  
    "  while i <= 10 do\n"  
    "    sum = sum * 2\n"  
    "    i = i + 1\n"  
    "  end\n"  
    "return sum\n"  
    "end\n"  
    "print(\"sum =\", foo())\n"  
    "print(\"and sum = 2^11 =\", 2 ^ 11)\n"  
    "print(\"exp(200) =\", math.exp(200))\n"  
};  

uint8_t fatfs_test_mkdir(void) {
    FIL fnew; /* �ļ����� */
    FRESULT res_sd; /* �ļ�������� */
    UINT fnum; /* �ļ��ɹ���д���� */
    BYTE ReadBuffer[1024]= {0}; /* �������� */
    BYTE WriteBuffer[1024] = {0,};//
    BYTE work[100]; /* Work area (larger is better for processing time) */
    /*--------------------- �ļ�ϵͳ���ԣ�д���� -----------------------*/
//    /* ���ļ�������ļ��������򴴽��� */
//    printf("\r\n****** ���������ļ�д�����... ******\r\n");
//    res_sd=f_open(&fnew,"1:test.lua",FA_CREATE_ALWAYS|FA_WRITE);
//    if ( res_sd == FR_OK ) {
//        printf("open ok write \r\n");
//        /* ��ָ���洢������д�뵽�ļ��� */
//        res_sd=f_write(&fnew,lua_test,sizeof(lua_test),&fnum);
//        if (res_sd==FR_OK) {
//            printf("ok %d\n",fnum);
//        } else {
//            printf("fale\n");
//        }
//        /* ���ٶ�д���ر��ļ� */
//        f_close(&fnew);
//    } else {
//        printf("open file\r\n");
//    }
    /*------------------ �ļ�ϵͳ���ԣ������� --------------------------*/
   // printf("file read\r\n");
    res_sd=f_open(&fnew,"1:test.lua",FA_OPEN_EXISTING|FA_READ);
    if (res_sd == FR_OK) {
        printf("open ok\r\n");
        res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if (res_sd==FR_OK) {
            printf("read: %d\r\n",fnum);
            //printf("read-a: \r\n%s \r\n", ReadBuffer);
        } else {
            printf("file (%d)\n",res_sd);
        }
    } else {
        printf("file\r\n");
    }
    /* ���ٶ�д���ر��ļ� */
    f_close(&fnew);
    
    lua_State* L;  
    L = luaL_newstate();  
    luaL_openlibs(L);  
  
    luaL_newlibtable(L, LuaLib1_0);  
    luaL_setfuncs(L, LuaLib1_0, 0);  
    //���ﶨ��ȫ�ֱ�����ջ����table��ֵ��LuaLib1_0�������ʽʹ��ģ������Ϊ�޷���cģ�����ɶ�̬���ӿ�.so��.dll��LUA_CPATH_DEFAULT����  
    lua_setglobal(L, "lhb");  
    //dostring(L,"print('hello my name is a linghaibin haha ')","Test_lua");
    luaL_dostring(L, ReadBuffer); /* ����Lua�ű� */  
    lua_close(L);
}


int main(void) {
    delay_init(168);
    uart_init(115200);
    printf("%s",logo);
    tft_init();
    
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
        //printf("mu: %s \n",buff);
        
       // fatfs_test_mkdir();
    }
    //USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
    
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
   // madLuaMain(lua_argc, (char **)lua_argv);
//    eth_init();
//    tcp_server_init();
//    xTaskCreate(vtask_gui_tic, "ui", 100, NULL, 2, NULL);
//    vTaskStartScheduler();
    while(1) {
        lv_tick_inc(1);
        lv_task_handler();
        delay_ms(1);
    }
}

void Lua_task(void)  
{   
    lua_State* L;  
    L = luaL_newstate();  
    luaL_openlibs(L);
    if (luaL_dofile(L,"1:test.lua")!=0)//����sd���е�Lua.lua������  
          printf(lua_tostring(L,-1));
}  

void vtask_gui_tic(void *pvParameters) {
    while(1) {
        vTaskDelay(1 / portTICK_RATE_MS);
        lv_tick_inc(1);
        lv_task_handler();
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
