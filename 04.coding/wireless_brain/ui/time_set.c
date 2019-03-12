/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "time_set.h"
#include "../lvgl.h"
#include "rtc.h"
#include "stdio.h"

static lv_res_t lv_win_close_action_animation(lv_obj_t * btn)
{
    lv_obj_t * win = lv_win_get_from_btn(btn);
    lv_obj_del(win);
    return LV_RES_INV;
}

static lv_obj_t * roller[6];
static lv_obj_t * lab_roller[6];

//保存
static lv_res_t btn_save_action(lv_obj_t * btn) {
    lv_obj_animate(btn, LV_ANIM_GROW_H , 300, 50, NULL);
    rtc_set_time(lv_roller_get_selected(roller[3]),lv_roller_get_selected(roller[4]),lv_roller_get_selected(roller[5]),RTC_H12_AM);
    rtc_set_date(lv_roller_get_selected(roller[0])+18,lv_roller_get_selected(roller[1]),lv_roller_get_selected(roller[2]),6);//设置日期
    lv_obj_t * mbox1 = lv_mbox_create(lv_scr_act(), NULL);
    lv_mbox_set_text(mbox1, "时间设置成功!\n");                    /*Set the text*/
    /*Add two buttons*/
    static const char * btns[] ={"\221确定", ""}; /*Button description. '\221' lv_btnm like control char*/
    lv_mbox_add_btns(mbox1, btns, NULL);
    lv_obj_set_width(mbox1, 250);
    lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the corner*/
   // lv_mbox_set_action(mbox1,mbox_apply_action);
}

void time_set_create(void) {

    lv_obj_t * win = lv_win_create(lv_scr_act(),NULL);
    lv_win_add_btn(win,SYMBOL_CLOSE,lv_win_close_action_animation);
    lv_win_set_title(win,"时间设置");
    lv_win_set_btn_size(win,WIN_BTN_HEIGHT);
    
    roller[0] = lv_roller_create(win,NULL); 
    lv_roller_set_options(roller[0]," 18 \n 19 \n 20 \n 21 \n 22 \n 23 \n 24 \n 25 \n 26 \n 27 \n 28 \n 29 \n 30 \n 31 \n 32 \n 33 \n 34 \n 35 \n 36 \n 37 \n 38 \n 39 \n 40 \n 41 ");
    lv_roller_set_visible_row_count(roller[0],3);    
    lv_obj_align(roller[0],win,LV_ALIGN_IN_LEFT_MID,5,0);
    
    lab_roller[0] = lv_label_create(win,NULL);
    lv_label_set_text(lab_roller[0],"年");
    lv_label_set_recolor(lab_roller[0],true);
    lv_obj_align(lab_roller[0],roller[0],LV_ALIGN_OUT_TOP_MID,0,0);
    
    roller[1] = lv_roller_create(win,NULL);        
    lv_roller_set_options(roller[1]," 00 \n 01 \n 02 \n 03 \n 04 \n 05 \n 06 \n 07 \n 08 \n 09 \n 10 \n 11 \n 12 ");
    lv_roller_set_visible_row_count(roller[1],3);    
    lv_obj_align(roller[1],roller[0],LV_ALIGN_OUT_RIGHT_MID,5,0);
    
    lab_roller[1] = lv_label_create(win,NULL);
    lv_label_set_text(lab_roller[1],"月");
    lv_label_set_recolor(lab_roller[1],true);
    lv_obj_align(lab_roller[1],roller[1],LV_ALIGN_OUT_TOP_MID,0,0);
    
    roller[2] = lv_roller_create(win,NULL);        
    lv_roller_set_options(roller[2], " 00 \n 01 \n 02 \n 03 \n 04 \n 05 \n 06 \n 07 \n 08 \n 09 \n 10 \n 11 \n 12 \n 13 \n 14 \n 15 \n 16 \n 17 \n 18 \n 19 \n 20 \n 21 \n 22 \n 23 \n 24 \n  25 \n 26 \n 27 \n 28 \n 29 \n 30 \n 31 ");
    lv_roller_set_visible_row_count(roller[2],3);    
    lv_obj_align(roller[2],roller[1],LV_ALIGN_OUT_RIGHT_MID,5,0);
    
    lab_roller[2] = lv_label_create(win,NULL);
    lv_label_set_text(lab_roller[2],"日");
    lv_label_set_recolor(lab_roller[2],true);
    lv_obj_align(lab_roller[2],roller[2],LV_ALIGN_OUT_TOP_MID,0,0);
    
    roller[3] = lv_roller_create(win,NULL);
    lv_roller_set_options(roller[3]," 00 \n 01 \n 02 \n 03 \n 04 \n 05 \n 06 \n 07 \n 08 \n 09 \n 10 \n 11 \n 12 \n 13 \n 14 \n 15 \n 16 \n 17 \n 18 \n 19 \n 20 \n 21 \n 22 \n 23 ");
    lv_roller_set_visible_row_count(roller[3],3);
    lv_obj_align(roller[3],roller[0],LV_ALIGN_IN_BOTTOM_MID,0,130);
    
    lab_roller[3] = lv_label_create(win,NULL);
    lv_label_set_text(lab_roller[3],"时");
    lv_label_set_recolor(lab_roller[3],true);
    lv_obj_align(lab_roller[3],roller[3],LV_ALIGN_OUT_TOP_MID,0,0);

    roller[4] = lv_roller_create(win,NULL);
    lv_roller_set_options(roller[4]," 00 \n 01 \n 02 \n 03 \n 04 \n 05 \n 06 \n 07 \n 08 \n 09 \n 10 \n 11 \n 12 \n 13 \n 14 \n 15 \n 16 \n 17 \n 18 \n 19 \n 20 \n 21 \n 22 \n 23 \n 24 \n  25 \n 26 \n 27 \n 28 \n 29 \n 30 \n 31 \n 32 \n 33 \n 34 \n 35 \n 36 \n 37 \n 38 \n 39 \n 40 \n 41 \n 42 \n 43 \n 44 \n 45 \n 46 \n 47 \n 48 \n 49 \n 50 \n 51 \n 52 \n 53 \n 54 \n 55 \n 56 \n 57 \n 58 \n 59 ");
    lv_roller_set_visible_row_count(roller[1],3);
    lv_obj_align(roller[4],roller[3],LV_ALIGN_OUT_RIGHT_MID,5,0);
    
    lab_roller[4] = lv_label_create(win,NULL);
    lv_label_set_text(lab_roller[4],"分");
    lv_label_set_recolor(lab_roller[4],true);
    lv_obj_align(lab_roller[4],roller[4],LV_ALIGN_OUT_TOP_MID,0,0);

    roller[5] = lv_roller_create(win,NULL);
    lv_roller_set_options(roller[5]," 00 \n 01 \n 02 \n 03 \n 04 \n 05 \n 06 \n 07 \n 08 \n 09 \n 10 \n 11 \n 12 \n 13 \n 14 \n 15 \n 16 \n 17 \n 18 \n 19 \n 20 \n 21 \n 22 \n 23 \n 24 \n  25 \n 26 \n 27 \n 28 \n 29 \n 30 \n 31 \n 32 \n 33 \n 34 \n 35 \n 36 \n 37 \n 38 \n 39 \n 40 \n 41 \n 42 \n 43 \n 44 \n 45 \n 46 \n 47 \n 48 \n 49 \n 50 \n 51 \n 52 \n 53 \n 54 \n 55 \n 56 \n 57 \n 58 \n 59 ");
    lv_roller_set_visible_row_count(roller[5],3);
    lv_obj_align(roller[5],roller[4],LV_ALIGN_OUT_RIGHT_MID,5,0);
    
    lab_roller[5] = lv_label_create(win,NULL);
    lv_label_set_text(lab_roller[5],"秒");
    lv_label_set_recolor(lab_roller[5],true);
    lv_obj_align(lab_roller[5],roller[5],LV_ALIGN_OUT_TOP_MID,0,0);
    
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
    
    lv_roller_set_selected(roller[0],RTC_DateStruct.RTC_Year-18,true);
    lv_roller_set_selected(roller[1],RTC_DateStruct.RTC_Month,true);
    lv_roller_set_selected(roller[2],RTC_DateStruct.RTC_Date,true);
    
    lv_roller_set_selected(roller[3],RTC_TimeStruct.RTC_Hours,true);
    lv_roller_set_selected(roller[4],RTC_TimeStruct.RTC_Minutes,true);
    lv_roller_set_selected(roller[5],RTC_TimeStruct.RTC_Seconds,true);
    
    //创建 按键和标签，保存
    lv_obj_t *btn_save = lv_btn_create(win,NULL);
    //lv_obj_set_size(btn_save,100,50);
    lv_obj_set_height(btn_save,40);
    lv_obj_set_width(btn_save,240);
    lv_obj_align(btn_save,roller[4] ,LV_ALIGN_IN_BOTTOM_MID,0,80);
    lv_btn_set_action(btn_save,LV_BTN_ACTION_CLICK,btn_save_action);

    lv_obj_t *label_save = lv_label_create(win,NULL);
    lv_label_set_text(label_save,"  #ffffff 设置#");
    lv_label_set_recolor(label_save,true);
    lv_obj_align(label_save,btn_save,LV_ALIGN_CENTER,0,0);
}
