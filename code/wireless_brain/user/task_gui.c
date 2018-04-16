/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "task_gui.h"
#include "../lv_hal/lv_hal_disp.h"
#include "../lv_misc/lv_mem.h"
#include "../lv_core/lv_obj.h"
#include "../lvgl.h"
#include "spi.h"
#include "tft.h"
#include "xpt2046.h"
#include "buzzer.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdio.h"
#include "delay.h"
//usb
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h" 

#include "task_lua.h"
#include "task_can.h"

LV_IMG_DECLARE(l_logo);
LV_IMG_DECLARE(jt_logo);

static void vtask_gui_tic(void *pvParameters);
static void demo_init(void);
static void vtask_show_device(void *pvParameters);

void task_gui_init(void) {
    SPI3_Init();
    xpt2046_init();
    tft_init();
    lv_vdb_init();
    lv_init();
    lv_disp_drv_t disp_drv;  
    lv_disp_drv_init(&disp_drv);  
    disp_drv.disp_flush = monitor_flush; /* 显示屏接口 */

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv); 
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = mouse_read;     /* 触摸接口 */
    lv_indev_drv_register(&indev_drv);

    lv_disp_drv_register(&disp_drv);
    /*Create a simple base object*/
    demo_init();
}

void task_gui_create(void) {
    xTaskCreate(vtask_gui_tic, "ui", 512, NULL, 3, NULL);
    xTaskCreate(vtask_show_device, "device", 512, NULL, 3, NULL);
}

static void vtask_gui_tic(void *pvParameters) {
    for( ;; ) {
        vTaskDelay(5 / portTICK_RATE_MS);
        lv_tick_inc(5);
        lv_task_handler();
        xpt2046_loop();
    }
}

//USB_OTG_CORE_HANDLE USB_OTG_dev;

static lv_res_t change_action(lv_obj_t * sw) {
    if(lv_sw_get_state(sw) == true) {
        task_lua_set(false);
        //taskENTER_CRITICAL();   /* 进入临界区 */
        //USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
        //delay_ms(2800);
        //taskEXIT_CRITICAL();    /* 退出临界区 */
    } else {
        task_lua_set(true);
        //DCD_DevDisconnect(&USB_OTG_dev);
    }
    buzzer_set(50,1);
    return LV_RES_OK;
}

/*Will be called on click of a button of a list*/
static lv_res_t list_release_action(lv_obj_t * list_btn) {
    printf("List element click:%s\n", lv_list_get_btn_text(list_btn));

    return LV_RES_OK; /*Return OK because the list is not deleted*/
}

lv_obj_t * list1;
lv_obj_t* label3;

static void vtask_show_device(void *pvParameters) {
    for( ;; ) {
        vTaskDelay(1000 / portTICK_RATE_MS);
        lv_obj_del(list1);
        list1 = lv_list_create(lv_scr_act(), NULL);
        lv_obj_set_size(list1, 130, 170);
        device_online *temp;
        for (int i = 0; i < list_len(task_can_get_device()); i++){ //list_len获取链表的长度
            list_get(task_can_get_device(), i, (void **)&temp); //取得位置为i的结点的数据
            char *buf = l_malloc(sizeof(char)*10);
            switch(temp->type) {
                case DO_8: {
                    sprintf(buf, "DI_8  %d", temp->address);
                } break;
                case DO_4: {
                    sprintf(buf, "DO_4  %d", temp->address);
                } break;
                case DI_4: {
                    sprintf(buf, "DI_4  %d", temp->address);
                } break;
            }
            lv_list_add(list1, SYMBOL_FILE, buf, list_release_action);
            l_free(buf);
        }
    }
}

static void demo_init(void) {
    lv_obj_t *tab;
    lv_obj_t *tab1;
    lv_obj_t *tab2;
    lv_obj_t *tab3;

    // lv_theme_t* theme = lv_theme_zen_init(22, NULL);
    lv_theme_t* theme = lv_theme_material_init(16, NULL);
    lv_theme_set_current(theme);

    lv_obj_t* win = lv_win_create(lv_scr_act(), NULL);
    lv_win_set_title(win, "wireless_b");

    //	lv_win_add_btn(win, SYMBOL_CLOSE, NULL);
    lv_win_add_btn(win, SYMBOL_UPLOAD, NULL);
    lv_win_add_btn(win, SYMBOL_BLUETOOTH, NULL);
    lv_win_set_btn_size(win,LV_VER_RES/8);
    lv_win_set_sb_mode(win,LV_SB_MODE_DRAG);

    tab = lv_tabview_create(win, NULL);
    tab1 = lv_tabview_add_tab(tab, "tabv1");
    tab2 = lv_tabview_add_tab(tab, "tabv2");
    tab3 = lv_tabview_add_tab(tab, "tabv3");
    lv_tabview_set_anim_time(tab, 200);


    lv_obj_t* btn1 = lv_btn_create(tab1, NULL);
    lv_obj_set_size(btn1, 160, 60);
    lv_obj_align(btn1, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

    lv_obj_t* label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Hello!\n#ffff00 LittlevGl2RTT#");
    lv_label_set_anim_speed(label, 30);
    lv_label_set_long_mode(label, LV_LABEL_LONG_DOT);
    lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_recolor(label, true);

    lv_obj_t* label2 = lv_label_create(tab1, NULL);
    lv_obj_set_pos(label2, 20, 100);
    lv_label_set_text(label2, "bluetooth "SYMBOL_BLUETOOTH);
    
    /*Create a switch and apply the styles*/
    lv_obj_t *sw1 = lv_sw_create(tab2, NULL);
    lv_obj_align(sw1, NULL, LV_ALIGN_CENTER, 0, -50);
    lv_sw_set_action(sw1, change_action);

    label3 = lv_label_create(tab2, NULL);
    lv_obj_set_pos(label3, 20, 100);
    lv_label_set_text(label3, "pro_lua "SYMBOL_OK);
    
    /*Crate the list*/
    list1 = lv_list_create(lv_scr_act(), NULL);
    lv_obj_set_size(list1, 130, 170);
    lv_obj_align(list1, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 40);

    /*Add list elements*/
    lv_list_add(list1, SYMBOL_FILE, "New", list_release_action);
    lv_list_add(list1, SYMBOL_DIRECTORY, "Open", list_release_action);
    lv_list_add(list1, SYMBOL_CLOSE, "Delete", list_release_action);
    lv_list_add(list1, SYMBOL_EDIT, "Edit", list_release_action);
    lv_list_add(list1, SYMBOL_SAVE, "Save", list_release_action);
    
//    lv_obj_t *img1 = lv_img_create(tab3, NULL);
//    lv_img_set_src(img1, &jt_logo);
//    lv_obj_set_pos(img1, 0, 0);
//    
//    /*Create an animation to move the button continuously left to right*/
//    lv_anim_t a;
//    a.var = img1;
//    a.start = lv_obj_get_x(img1);
//    a.end = a.start + (100 );
//    a.fp = (lv_anim_fp_t)lv_obj_set_x;
//    a.path = lv_anim_path_linear;
//    a.end_cb = NULL;
//    a.act_time = -1000;                         /*Negative number to set a delay*/
//    a.time = 400;                               /*Animate in 400 ms*/
//    a.playback = 1;                             /*Make the animation backward too when it's ready*/
//    a.playback_pause = 0;                       /*Wait before playback*/
//    a.repeat = 1;                               /*Repeat the animation*/
//    a.repeat_pause = 500;                       /*Wait before repeat*/
//    lv_anim_create(&a);
}

