/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "ui_can_set.h"
#include "../lvgl.h"
#include "rtc.h"
#include "stdio.h"
#include "time_set.h"
#include "persistence.h"
#include "can.h"

static lv_res_t lv_win_close_action_animation(lv_obj_t * btn)
{
    lv_obj_t * win = lv_win_get_from_btn(btn);
    lv_obj_del(win);
    return LV_RES_INV;
}

static lv_obj_t * roller[1];
static lv_obj_t * lab_roller[1];

static lv_obj_t * mbox1;
/*Called when a button is clicked*/
static lv_res_t mbox_apply_action(lv_obj_t * mbox, const char * txt) {
    if(strcmp(txt,"ÈáçÂêØ")==0) {
         __set_FAULTMASK(1);
        NVIC_SystemReset();
    } else {
        lv_obj_del(mbox1);
    }
    return LV_RES_OK; /*Return OK if the message box is not deleted*/
}
//±£¥Ê
static lv_res_t btn_save_action(lv_obj_t * btn) {
    lv_obj_animate(btn, LV_ANIM_GROW_H , 300, 50, NULL);
    uint8_t buad;
    switch(lv_roller_get_selected(roller[0])) {
        case 0: {
            buad = BAUD_RATE_10K;
        } break;
        case 1: {
            buad = BAUD_RATE_20K;
        } break;
        case 2: {
            buad = BAUD_RATE_50K;
        } break;
        case 3: {
            buad = BAUD_RATE_100K;
        } break;
        case 4: {
            buad = BAUD_RATE_125K;
        } break;
        case 5: {
            buad = BAUD_RATE_250K;
        } break;
        case 6: {
            buad = BAUD_RATE_500K;
        } break;
        case 7: {
            buad = BAUD_RATE_1000K;
        } break;
    }
    persistence_set_can_adr(&buad);
    
    /*Copy the message box (The buttons will be copied too)*/
    mbox1 = lv_mbox_create(lv_scr_act(), NULL);
    lv_mbox_set_text(mbox1, "ËÆæÁΩÆÊàêÂäü,ÊòØÂê¶ÈáçÂêØn");/*Set the text*/
    /*Add two buttons*/
    static const char * btns[] ={"\221ÈáçÂêØ", "\221ÈÄÄÂá∫", ""}; /*Button description. '\221' lv_btnm like control char*/
    lv_mbox_add_btns(mbox1, btns, NULL);
    lv_obj_set_width(mbox1, 250);
    lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the corner*/
    lv_mbox_set_action(mbox1,mbox_apply_action);
}

void ui_can_set_create(void) {

    lv_obj_t * win = lv_win_create(lv_scr_act(),NULL);
    lv_win_add_btn(win,SYMBOL_CLOSE,lv_win_close_action_animation);
    lv_win_set_title(win,"Can Set");
    lv_win_set_btn_size(win,WIN_BTN_HEIGHT);
    
    roller[0] = lv_roller_create(win,NULL); 
    lv_roller_set_options(roller[0]," 10k \n 20K \n 50K \n 100K \n 125K \n 250K \n 500K \n 1000K \n");
    lv_roller_set_visible_row_count(roller[0],3);    
    lv_obj_align(roller[0],win,LV_ALIGN_IN_LEFT_MID,5,0);
    
    lab_roller[0] = lv_label_create(win,NULL);
    lv_label_set_text(lab_roller[0],"CAN");
    lv_label_set_recolor(lab_roller[0],true);
    lv_obj_align(lab_roller[0],roller[0],LV_ALIGN_OUT_TOP_MID,0,0);
    
    uint8_t sel;
    uint8_t buad;
    persistence_get_can_adr(&buad);
    switch(buad) {
        case BAUD_RATE_10K: {
            sel = 0;
        } break;
        case BAUD_RATE_20K: {
            sel = 1;
        } break;
        case BAUD_RATE_50K: {
            sel = 2;
        } break;
        case BAUD_RATE_100K: {
            sel = 3;
        } break;
        case BAUD_RATE_125K: {
            sel = 4;
        } break;
        case BAUD_RATE_250K: {
            sel = 5;
        } break;
        case BAUD_RATE_500K: {
            sel = 6;
        } break;
        case BAUD_RATE_1000K: {
            sel = 7;
        } break;
        default: {
            sel = 3;
        } break;
    }
    lv_roller_set_selected(roller[0],sel,true);
    
    //¥¥Ω® ∞¥º¸∫Õ±Í«©£¨±£¥Ê
    lv_obj_t *btn_save = lv_btn_create(win,NULL);
    lv_obj_set_height(btn_save,80);
    lv_obj_set_width(btn_save,80);
    lv_btn_set_action(btn_save,LV_BTN_ACTION_CLICK,btn_save_action);
     lv_obj_align(btn_save,roller[0] ,LV_ALIGN_OUT_RIGHT_MID,60,0);

    lv_obj_t *label_save = lv_label_create(win,NULL);
    lv_label_set_text(label_save,"ËÆæÁΩÆ");
    lv_label_set_recolor(label_save,true);
    lv_obj_align(label_save,btn_save,LV_ALIGN_CENTER,0,0);
}
