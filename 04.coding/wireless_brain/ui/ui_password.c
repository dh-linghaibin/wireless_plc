/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "ui_password.h"
#include "../lvgl.h"
#include "time_set.h"

static lv_obj_t * pas_win;
static lv_obj_t * pas_old;
static lv_obj_t * pas_new;

static const char * pas_map[] = {"1", "2", "3" ,"4", "\n",
                                 "5", "6", "7" ,"8", "\n",
                                 "9", "0", ".","\n",
                                 "删除", "确定", ""};

static lv_res_t paw_close(lv_obj_t * btn) {
    lv_obj_t * win = lv_win_get_from_btn(btn);
    lv_obj_del(win);
    return LV_RES_INV;
}

/*Called when a button is released ot long pressed*/
static lv_res_t btnm_action(lv_obj_t * btnm, const char *txt) {
    if(strcmp(txt,"删除")==0) {
        lv_ta_del_char(pas_old);
    } else if(strcmp(txt,"确定")==0) {
//        char pas_code[6];
//        uint8_t pas_s[6];
//        persistence_get_password(pas_s);
//        strcpy(pas_code, lv_ta_get_text(pas));
//        for( uint8_t pi = 0; pi < 6;pi++ ) {
//            if(pas_code[pi] == pas_s[pi]) {
//                if(pi == 5) {
//                    lock_flag = 0;
//                    lv_img_set_src(img_lock, &lock_open);
//                    persistence_set_lock(&lock_flag);
//                    lv_obj_del(pas_win);
//                }
//            } else {
//                break;
//            }
//        }
    } else {
        if( 6 > lv_ta_get_cursor_pos(pas_old) ) {
            lv_ta_add_char(pas_old,txt[0]);
        }
    }
    return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}

void ui_password_change(void) {
    pas_win = lv_win_create(lv_scr_act(),NULL);
    lv_win_add_btn(pas_win,SYMBOL_CLOSE,paw_close);
    lv_win_set_title(pas_win,"修改密码");
    lv_win_set_btn_size(pas_win,30);

    /*Create a one lined test are with password mode*/
    pas_old = lv_ta_create(pas_win, NULL);
    lv_obj_set_pos (pas_old,50, 0);
    lv_ta_set_one_line(pas_old, true);
    lv_ta_set_cursor_type(pas_old, LV_CURSOR_LINE);
    lv_ta_set_pwd_mode(pas_old, true);
    lv_ta_set_text(pas_old,"");
    
    pas_new = lv_ta_create(pas_win, NULL);
    lv_obj_align(pas_new,pas_old, LV_ALIGN_OUT_BOTTOM_MID ,0,1);
    lv_ta_set_one_line(pas_new, true);
    lv_ta_set_cursor_type(pas_new, LV_CURSOR_LINE);
    lv_ta_set_pwd_mode(pas_new, true);
    lv_ta_set_text(pas_new,"");
    
    /*Create a second button matrix with the new styles*/
    lv_obj_t * key_board = lv_btnm_create(pas_win, NULL);
    lv_btnm_set_map(key_board, pas_map);
    lv_btnm_set_action(key_board, btnm_action);
    lv_obj_set_size(key_board,150,100);
    lv_obj_align(key_board,pas_new, LV_ALIGN_OUT_BOTTOM_MID ,0,5);
}
