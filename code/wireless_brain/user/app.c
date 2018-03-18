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
//lvgl
#include "../lv_hal/lv_hal_disp.h"
#include "../lv_misc/lv_mem.h"
#include "../lv_core/lv_obj.h"
#include "../lvgl.h"
//LWIP
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"

#include "gui_demo.h"


const char logo[] = "\
/*\n\
 * This file is part of the \n\
 *\n\
 * Copyright (c) 2017-2018 linghaibin\n\
 *\n\
 */\n\
 Welcome to use device!\n";

/*Called when a new value id set on the slider*/
static lv_res_t slider_action(lv_obj_t * slider)
{
    printf("New slider value: %d\n", lv_slider_get_value(slider));

    return LV_RES_OK;
}

lv_obj_t * slider1;
lv_obj_t * gauge1;
static lv_res_t btn_click_action(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn);

    printf("Button %d is released\n", id);

    /* The button is released.
     * Make something here */

    return LV_RES_OK; /*Return OK if the button is not deleted*/
}

/*Called when a button is released ot long pressed*/
static lv_res_t btnm_action(lv_obj_t * btnm, const char *txt)
{
    printf("Button: %s released\n", txt);

    return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}

/*Create a button descriptor string array*/
static const char * btnm_map[] = {"1", "2", "3", "4", "5", "\n",
                           "6", "7", "8", "9", "0", "\n",
                           "\202Action1", "Action2", ""};
void lv_test_object_1(void)
{
    
  
                    
    /*Create a style*/
    static lv_style_t style;
    lv_style_copy(&style, &lv_style_pretty_color);
    style.body.main_color = LV_COLOR_HEX3(0x666);     /*Line color at the beginning*/
    style.body.grad_color =  LV_COLOR_HEX3(0x666);    /*Line color at the end*/
    style.body.padding.hor = 10;                      /*Scale line length*/
    style.body.padding.inner = 8 ;                    /*Scale label padding*/
    style.body.border.color = LV_COLOR_HEX3(0x333);   /*Needle middle circle color*/
    style.line.width = 3;
    style.text.color = LV_COLOR_HEX3(0x333);
    style.line.color = LV_COLOR_RED;                  /*Line color after the critical value*/


    /*Describe the color for the needles*/
    static lv_color_t needle_colors[] = {LV_COLOR_BLUE, LV_COLOR_ORANGE, LV_COLOR_PURPLE};

    /*Create a gauge*/
    gauge1 = lv_gauge_create(lv_scr_act(), NULL);
    lv_gauge_set_style(gauge1, &style);
    lv_gauge_set_needle_count(gauge1, 3, needle_colors);
    lv_obj_align(gauge1, NULL, LV_ALIGN_CENTER, 0, 20);

    /*Set the values*/
    lv_gauge_set_value(gauge1, 0, 10);
    lv_gauge_set_value(gauge1, 1, 20);
    lv_gauge_set_value(gauge1, 2, 30);
    
    
    /*Create a default slider*/
    slider1 = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_set_size(slider1, 160, 30);
    lv_obj_align(slider1, NULL, LV_ALIGN_IN_TOP_RIGHT, -30, 30);
    lv_slider_set_action(slider1, slider_action);
    lv_bar_set_value(slider1, 70);


    /*Create a label right to the slider*/
    lv_obj_t * slider1_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(slider1_label, "Default");
    lv_obj_align(slider1_label, slider1, LV_ALIGN_OUT_LEFT_MID, -20, 0);

    /*Create a bar, an indicator and a knob style*/
    static lv_style_t style_bg;
    static lv_style_t style_indic;
    static lv_style_t style_knob;

    lv_style_copy(&style_bg, &lv_style_pretty);
    style_bg.body.main_color =  LV_COLOR_BLACK;
    style_bg.body.grad_color =  LV_COLOR_GRAY;
    style_bg.body.radius = LV_RADIUS_CIRCLE;
    style_bg.body.border.color = LV_COLOR_WHITE;

    lv_style_copy(&style_indic, &lv_style_pretty);
    style_indic.body.grad_color =  LV_COLOR_GREEN;
    style_indic.body.main_color =  LV_COLOR_LIME;
    style_indic.body.radius = LV_RADIUS_CIRCLE;
    style_indic.body.shadow.width = 10;
    style_indic.body.shadow.color = LV_COLOR_LIME;
    style_indic.body.padding.hor = 3;
    style_indic.body.padding.ver = 3;

    lv_style_copy(&style_knob, &lv_style_pretty);
    style_knob.body.radius = LV_RADIUS_CIRCLE;
    style_knob.body.opa = LV_OPA_70;
    style_knob.body.padding.ver = 10 ;

    /*Create a second slider*/
    lv_obj_t * slider2 = lv_slider_create(lv_scr_act(), slider1);
    lv_slider_set_style(slider2, LV_SLIDER_STYLE_BG, &style_bg);
    lv_slider_set_style(slider2, LV_SLIDER_STYLE_INDIC,&style_indic);
    lv_slider_set_style(slider2, LV_SLIDER_STYLE_KNOB, &style_knob);
    lv_obj_align(slider2, slider1, LV_ALIGN_OUT_BOTTOM_MID, 0, 30); /*Align below 'bar1'*/

    /*Create a second label*/
    lv_obj_t * slider2_label = lv_label_create(lv_scr_act(), slider1_label);
    lv_label_set_text(slider2_label, "Modified");
    lv_obj_align(slider2_label, slider2, LV_ALIGN_OUT_LEFT_MID, -30, 0);
    
    
    
    
    
    
    /*Create a default button matrix*/
    lv_obj_t * btnm1 = lv_btnm_create(lv_scr_act(), NULL);
    lv_btnm_set_map(btnm1, btnm_map);
    lv_btnm_set_action(btnm1, btnm_action);
    lv_obj_set_size(btnm1, LV_HOR_RES, LV_VER_RES / 2);

    /*Create a new style for the button matrix back ground*/
    //static lv_style_t style_bg;
    lv_style_copy(&style_bg, &lv_style_plain);
    style_bg.body.main_color = LV_COLOR_SILVER;
    style_bg.body.grad_color = LV_COLOR_SILVER;
    style_bg.body.padding.hor = 0;
    style_bg.body.padding.ver = 0;
    style_bg.body.padding.inner = 0;

    /*Create 2 button styles*/
    static lv_style_t style_btn_rel;
    static lv_style_t style_btn_pr;
    lv_style_copy(&style_btn_rel, &lv_style_btn_rel);
    style_btn_rel.body.main_color = LV_COLOR_MAKE(0x30, 0x30, 0x30);
    style_btn_rel.body.grad_color = LV_COLOR_BLACK;
    style_btn_rel.body.border.color = LV_COLOR_SILVER;
    style_btn_rel.body.border.width = 1;
    style_btn_rel.body.border.opa = LV_OPA_50;
    style_btn_rel.body.radius = 0;

    lv_style_copy(&style_btn_pr, &style_btn_rel);
    style_btn_pr.body.main_color = LV_COLOR_MAKE(0x55, 0x96, 0xd8);
    style_btn_pr.body.grad_color = LV_COLOR_MAKE(0x37, 0x62, 0x90);
    style_btn_pr.text.color = LV_COLOR_MAKE(0xbb, 0xd5, 0xf1);
    
    lv_obj_align(btnm1, slider1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

//    /*Create a second button matrix with the new styles*/
//    lv_obj_t * btnm2 = lv_btnm_create(lv_scr_act(), btnm1);
//    lv_btnm_set_style(btnm2, LV_BTNM_STYLE_BG, &style_bg);
//    lv_btnm_set_style(btnm2, LV_BTNM_STYLE_BTN_REL, &style_btn_rel);
//    lv_btnm_set_style(btnm2, LV_BTNM_STYLE_BTN_PR, &style_btn_pr);
//    lv_obj_align(btnm2, btnm1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

      /*Create a title label*/
    lv_obj_t * label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(label, "Default buttons");
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

    /*Create a normal button*/
    lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_cont_set_fit(btn1, true, true); /*Enable resizing horizontally and vertically*/
    lv_obj_align(btn1, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 200);
    lv_obj_set_free_num(btn1, 1);   /*Set a unique number for the button*/
    lv_btn_set_action(btn1, LV_BTN_ACTION_CLICK, btn_click_action);

    /*Add a label to the button*/
    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Normal");
}

int main(void) {
    delay_init(168);
    FSMC_SRAM_Init();//初始化外部SRAM
    my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池
	my_mem_init(SRAMCCM);	//初始化CCM内存池
    uart_init(115200);
    //printf("%s",logo);
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
    //lv_test_object_1();
    demo_create();

//    if(lwip_comm_init()!=0) {
//        printf("lwIP Init failed!");
//    } else {
//        printf("lwIP Init ok!");
//    }
//    while((lwipdev.dhcpstatus!=2)&&(lwipdev.dhcpstatus!=0XFF))//等待DHCP获取成功/超时溢出
//    {
//        lwip_periodic_handle();
//    }
    while(1) {
         static uint16_t num2 = 0;
        static uint8_t num = 0;
        //delay_ms(1);
         lv_task_handler();
         lv_tick_inc(1);
//        if(num2 < 10) {
//            num2++;
//        } else {
//            num2=0;
//            if(num < 100) {
//            num ++;
//            } else {
//                num = 0;
//            }
//            lv_bar_set_value(slider1, num);
//            lv_gauge_set_value(gauge1, 2, num);
//        }
        xpt2046_loop();
//        int x = xpt2046_get_x();
//        int y = xpt2046_get_y();
      //  printf("触摸 %d  %d\n",x,y);
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
