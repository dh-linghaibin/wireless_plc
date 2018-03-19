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
#include "timer.h"
#include "stdio.h"
//lvgl
#include "../lv_hal/lv_hal_disp.h"
#include "../lv_misc/lv_mem.h"
#include "../lv_core/lv_obj.h"
#include "../lvgl.h"
//LWIP
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"
//lua
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "string.h"

#include "gui_demo.h"


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
    //c接口函数都可以放在这里在lua中声明  
    {"Delay_ms", Delay_ms},  
    {"print", print},  
  
    {NULL, NULL}   
};  


/* 测试的Lua代码字符串 */  
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

void Lua_task(void)  
{   
    lua_State* L;  
    L = luaL_newstate();  
    luaL_openlibs(L);  
  
    luaL_newlibtable(L, LuaLib1_0);  
    luaL_setfuncs(L, LuaLib1_0, 0);  
    //这里定义全局变量把栈顶的table赋值给LuaLib1_0，这个方式使用模块是因为无法将c模块生成动态链接库.so或.dll给LUA_CPATH_DEFAULT加载  
    lua_setglobal(L, "lhb");  
    //dostring(L,"print('hello my name is a linghaibin haha ')","Test_lua");
    luaL_dostring(L, lua_test); /* 运行Lua脚本 */  
    lua_close(L);
}  

void lv_test_roller_1(void)
{
    /* Default object*/
    lv_obj_t *roller1 = lv_roller_create(lv_scr_act(), NULL);
    lv_obj_set_pos(roller1, 10, 10);


    static lv_style_t bg;
    lv_style_copy(&bg, &lv_style_pretty);
    bg.body.main_color = LV_COLOR_GRAY;
    bg.body.grad_color = LV_COLOR_WHITE;
    bg.body.shadow.width = 5;
    bg.text.line_space = 10;
    bg.text.opa = LV_OPA_60;
    bg.text.color = LV_COLOR_GRAY;

    lv_obj_t *roller2 = lv_roller_create(lv_scr_act(), NULL);
    lv_obj_set_size(roller2, 80, 120);
    lv_roller_set_options(roller2, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9");
    lv_obj_align(roller2, roller1, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_roller_set_anim_time(roller2, 500);
    lv_roller_set_style(roller2, LV_ROLLER_STYLE_BG, &bg);
    lv_roller_set_style(roller2, LV_ROLLER_STYLE_SEL, &lv_style_plain);
    lv_roller_set_selected(roller2, 4, true);

    lv_obj_t *roller3 = lv_roller_create(lv_scr_act(), roller2);
    lv_obj_align(roller3, roller2, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
    lv_roller_set_hor_fit(roller3, false);
    lv_obj_set_width(roller3, LV_DPI);

}

void lcd_test(void) {
    int c = 100;
    for(int i = 0;i < 500;i++) {
        tft_fill(0,0,100,300,c);
        c+= 10;
        xpt2046_loop();
    }
}

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
    Lua_task();
    SPI3_Init();
    xpt2046_init();
    tft_init();
    W25QXX_Init();
    if(W25QXX_ReadID()!=W25Q128) {
        //printf("W25Q128 Check Failed! \n");
    } else {
       // printf("W25Q128 Check ok! \n");
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
    lv_vdb_init();
    lv_init();
    lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/
    /*Set up the functions to access to your display*/
    disp_drv.disp_flush = monitor_flush;            /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/
    //disp_drv.disp_fill = tft_fill;              /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
    //disp_drv.disp_map = tft_fill;                /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_drv_register(&indev_drv);

    lv_disp_drv_register(&disp_drv);
    /*Create a simple base object*/
    //  lv_test_object_1();
    demo_create();
    //lv_test_roller_1();
    TIM3_Int_Init(999,839); //100khz的频率,计数1000为10ms
//    if(lwip_comm_init()!=0) {
//        printf("lwIP Init failed!");
//    } else {
//        printf("lwIP Init ok!");
//    }
//    while((lwipdev.dhcpstatus!=2)&&(lwipdev.dhcpstatus!=0XFF))//等待DHCP获取成功/超时溢出
//    {
//        lwip_periodic_handle();
//    }
    //lcd_test();
    while(1) {
       
		//delay_ms(2);
         static uint16_t num2 = 0;
        static uint8_t num = 0;
        //delay_ms(1);
         lv_task_handler();
        
        if(num2 < 100) {
            num2++;
        } else {
            num2=0;
            if(num < 200) {
            num ++;
            } else {
                num = 0;
            }
             //lwip_periodic_handle();
             lv_tick_inc(1);
        }
        xpt2046_loop();
//        int x = xpt2046_get_x();
//        int y = xpt2046_get_y();
      //  printf("触摸 %d  %d\n",x,y);
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
