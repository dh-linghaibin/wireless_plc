/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "ui_menu.h"
#include "../lvgl.h"
#include "time_set.h"
#include "ui_debug.h"
#include "ui_net_set.h"
#include "ui_can_set.h"

#include "persistence.h"
#include "ui_password.h"
#include <string.h>
#include <stdlib.h>
#include "buzzer.h"
#include "can.h"

extern const lv_img_t benchmark_bg;
extern const lv_img_t l_back;
extern const lv_img_t img_alarm;
extern const lv_img_t img_cpu;
extern const lv_img_t img_rss;
extern const lv_img_t lock_open;
extern const lv_img_t lock_un;
extern const lv_img_t img_home;

static uint8_t lock_flag = 0; /* 操作锁 */

static lv_res_t btn_alarm_action(lv_obj_t *obj) {
    if( 0 == lock_flag ) {
        time_set_create();
    }
}

static lv_res_t btn_set_action(lv_obj_t *obj) {
    if( 0 == lock_flag ) {
        ui_debug_create();
    }
}

static lv_res_t btn_set_net(lv_obj_t *obj) {
    if( 0 == lock_flag ) {
        ui_net_set_create();
    }
}

static lv_res_t btn_set_can(lv_obj_t *obj) {
    if( 0 == lock_flag ) {
        ui_can_set_create();
    }
}


/* 密码 */
extern const char * btnm_map[];

static lv_res_t paw_close(lv_obj_t * btn) {
    lv_obj_t * win = lv_win_get_from_btn(btn);
    lv_obj_del(win);
    return LV_RES_INV;
}

static lv_obj_t * pas_win;
static lv_obj_t * img_lock;
static lv_obj_t * pas;
/*Called when a button is released ot long pressed*/
static lv_res_t btnm_action(lv_obj_t * btnm, const char *txt) {
    if(strcmp(txt,"删除")==0) {
        lv_ta_del_char(pas);
    } else if(strcmp(txt,"确定")==0) {
        char pas_code[6];
        uint8_t pas_s[6];
        persistence_get_password(pas_s);
        strcpy(pas_code, lv_ta_get_text(pas));
        for( uint8_t pi = 0; pi < 6;pi++ ) {
            if(pas_code[pi] == pas_s[pi]) {
                if(pi == 5) {
                    lock_flag = 0;
                    lv_img_set_src(img_lock, &lock_open);
                    persistence_set_lock(&lock_flag);
                    lv_obj_del(pas_win);
                }
            } else {
                break;
            }
        }
    } else {
        if( 6 > lv_ta_get_cursor_pos(pas) ) {
            lv_ta_add_char(pas,txt[0]);
        }
    }
    return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}

static lv_res_t btn_click_change(lv_obj_t * btn) {
    ui_password_change();
    return LV_RES_OK; /*Return OK if the button is not deleted*/
}

void ui_pas_create(void) {
    pas_win = lv_win_create(lv_scr_act(),NULL);
    lv_win_add_btn(pas_win,SYMBOL_CLOSE,paw_close);
    lv_win_set_title(pas_win,"输入密码");
    lv_win_set_btn_size(pas_win,30);
    
    /*Create a one lined test are with password mode*/
    pas = lv_ta_create(pas_win, NULL);
    lv_obj_set_pos (pas,50, 0);
    lv_ta_set_one_line(pas, true);
    lv_ta_set_cursor_type(pas, LV_CURSOR_LINE);
    lv_ta_set_pwd_mode(pas, true);
    lv_ta_set_text(pas,"");
    
    /*Create a second button matrix with the new styles*/
    lv_obj_t * key_board = lv_btnm_create(pas_win, NULL);
    lv_btnm_set_map(key_board, btnm_map);
    lv_btnm_set_action(key_board, btnm_action);
    lv_obj_set_size(key_board,200,100);
    lv_obj_align(key_board,pas, LV_ALIGN_OUT_BOTTOM_MID ,0,5);
    
    lv_obj_t* btn1 = lv_btn_create(pas_win, NULL);
    lv_obj_set_size(btn1, 100, 40);
    lv_obj_align(btn1, NULL, LV_ALIGN_IN_BOTTOM_MID, 40, 30);
    lv_btn_set_action(btn1, LV_BTN_ACTION_CLICK, btn_click_change);
    
    lv_obj_t* label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "#ffff00 修改密码#");
    lv_label_set_anim_speed(label, 30);
    lv_label_set_long_mode(label, LV_LABEL_LONG_DOT);
    lv_label_set_align(label, LV_LABEL_ALIGN_CENTER);
    lv_label_set_recolor(label, true);
}

static lv_res_t btn_set_lock(lv_obj_t *obj) {
    if( 0 == lock_flag ) {
        lock_flag = 1;
        lv_img_set_src(img_lock, &lock_un);
        persistence_set_lock(&lock_flag);
    } else {
        //buzzer_set(100,1);
        ui_pas_create();
//        lock_flag = 0;
//        lv_img_set_src(img_lock, &lock_open);
    }
}


static lv_style_t btn_style_rel,btn_style_pr;
static lv_obj_t * wp;
static lv_style_t style_wp;

void ui_menu_create(void) {
    int16_t distance_obj=15;//Í¼±êÖ®¼äµÄ¼ä¾à
    int16_t distance_scr =9;//Í¼±êÓëÆÁÄ»±ß¿òµÄ¼ä¾à
    int16_t img_width = 64;;
    //SCREEN
    lv_obj_t * scr = lv_scr_act();

    lv_obj_t * btn[4];
    lv_obj_t * img[4] ;
   
    lv_style_copy(&btn_style_rel,&lv_style_transp);
    lv_style_copy(&btn_style_pr,&lv_style_plain_color);
    btn_style_pr.body.opa = 50;
    
    lv_style_copy(&style_wp, &lv_style_plain);
    style_wp.image.color = LV_COLOR_RED;
    
    /*Create a wallpaper on the page*/
//    wp = lv_img_create(lv_scr_act(), NULL);
//    lv_obj_set_protect(wp, LV_PROTECT_PARENT);          /*Don't let to move the wallpaper by the layout */
//    lv_img_set_src(wp, &benchmark_bg);
//    lv_obj_set_size(wp, LV_HOR_RES, LV_VER_RES);
//    lv_obj_set_pos(wp, 0, 0);
//    lv_obj_set_hidden(wp, false);
//    lv_img_set_style(wp, &style_wp);
//    lv_img_set_auto_size(wp, false);

    for(uint8_t j=0;j<1;j++)
    {
        for(uint8_t i=0;i<4;i++)
        {
            img[i+j*3] = lv_img_create(scr, NULL);
//            lv_img_set_src(img[i+j*3], &img_camera);
            lv_obj_set_size(img[i+j*3],64,64);
            lv_obj_set_pos(img[i+j*3], distance_scr+ img_width*i+distance_obj*i,distance_scr+ img_width*j+distance_obj*j);

            btn[i+j*3] = lv_btn_create(scr, NULL);
            lv_obj_set_size(btn[i+j*3],64+5,64+5);
            lv_obj_align(btn[i+j*3],img[i+j*3],LV_ALIGN_CENTER,0,0);
            lv_btn_set_style(btn[i+j*3],LV_BTN_STYLE_REL,&btn_style_rel);
            lv_btn_set_style(btn[i+j*3],LV_BTN_STYLE_PR,&btn_style_pr);
        }
    }
    lv_img_set_src(img[0], &img_alarm);
    lv_img_set_src(img[1], &img_cpu);
    lv_img_set_src(img[2], &img_rss);
    lv_img_set_src(img[3], &img_home);
//    lv_img_set_src(img[3], &img_rss);
//    lv_img_set_src(img[4], &img_music);
//    lv_img_set_src(img[5], &img_home);
//    lv_img_set_src(img[0], &img_alarm);
    
//    lv_img_set_src(img[8], &img_set);
    lv_btn_set_action(btn[0],LV_BTN_ACTION_CLICK,btn_alarm_action);
    lv_btn_set_action(btn[1],LV_BTN_ACTION_CLICK,btn_set_action);
    lv_btn_set_action(btn[2],LV_BTN_ACTION_CLICK,btn_set_net);
    lv_btn_set_action(btn[3],LV_BTN_ACTION_CLICK,btn_set_can);
    
    uint8_t g_ip[4];
    persistence_get_ip(g_ip);
    char buf[20];
    sprintf(buf, "ip:%d.%d.%d.%d:502",g_ip[0],g_ip[1],g_ip[2],g_ip[3]);
    lv_obj_t * ip_show = lv_label_create(lv_scr_act(),NULL);
    lv_label_set_text(ip_show,buf);
    lv_obj_align(ip_show,NULL,LV_ALIGN_IN_LEFT_MID,30,0);
    
    
    lv_obj_t * mask_show = lv_label_create(lv_scr_act(),ip_show);
    persistence_get_mask(g_ip);
    sprintf(buf, "mask:%d.%d.%d.%d",g_ip[0],g_ip[1],g_ip[2],g_ip[3]); 
    
    lv_label_set_text(mask_show,buf);
    lv_obj_align(mask_show,ip_show,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    
    
    lv_obj_t * time_show = lv_label_create(lv_scr_act(),ip_show);
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
    sprintf(buf, "时间:%d-%d-%d-%d-%d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes);
    lv_label_set_text(time_show,buf);
    lv_obj_align(time_show,mask_show,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    
    lv_obj_t * can_show = lv_label_create(lv_scr_act(),ip_show);
    persistence_get_can_adr(g_ip);
    switch(g_ip[0]) {
        case BAUD_RATE_10K: {
            sprintf(buf, "CAN:10K");
        } break;
        case BAUD_RATE_20K: {
            sprintf(buf, "CAN:20K");
        } break;
        case BAUD_RATE_50K: {
            sprintf(buf, "CAN:50K");
        } break;
        case BAUD_RATE_100K: {
            sprintf(buf, "CAN:100K");
        } break;
        case BAUD_RATE_125K: {
            sprintf(buf, "CAN:125K");
        } break;
        case BAUD_RATE_250K: {
            sprintf(buf, "CAN:250K");
        } break;
        case BAUD_RATE_500K: {
            sprintf(buf, "CAN:500K");
        } break;
        case BAUD_RATE_1000K: {
            sprintf(buf, "CAN:1000K");
        } break;
        default: {
            sprintf(buf, "CAN:50K");
        } break;
    }
    lv_label_set_text(can_show,buf);
    lv_obj_align(can_show,time_show,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    
    lv_obj_t * edition = lv_label_create(lv_scr_act(),can_show);
    lv_label_set_text(edition,"T-V2.5-NET");
    lv_obj_align(edition,can_show,LV_ALIGN_OUT_BOTTOM_LEFT,0,5);
    
    lv_obj_t * btn_lock;
    img_lock = lv_img_create(scr, NULL);
    lv_obj_set_size(img_lock,48,48);
    lv_obj_set_pos(img_lock, 260,180);

    btn_lock = lv_btn_create(scr, NULL);
    lv_obj_set_size(btn_lock,64+5,64+5);
    lv_obj_align(btn_lock,img_lock,LV_ALIGN_CENTER,0,0);
    lv_btn_set_style(btn_lock,LV_BTN_STYLE_REL,&btn_style_rel);
    lv_btn_set_style(btn_lock,LV_BTN_STYLE_PR,&btn_style_pr);
    persistence_get_lock(&lock_flag);
    if( 0 == lock_flag ) {
        lv_img_set_src(img_lock, &lock_open);
    } else {
        lv_img_set_src(img_lock, &lock_un);
    }
    lv_btn_set_action(btn_lock,LV_BTN_ACTION_CLICK,btn_set_lock);
}


 