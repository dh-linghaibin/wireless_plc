/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "ui_net_set.h"
#include "time_set.h"
#include "../lvgl.h"
#include "string.h"
#include "stdio.h"
#include "persistence.h"
#include <string.h>
#include <stdlib.h>

/*Create a button descriptor string array*/
const char * btnm_map[] = {"1", "2", "3", "4", "5", "\n",
                           "6", "7", "8", "9", "0", "\n",
                           ".","删除", "\202确定", ""};


static lv_res_t lv_win_close_action_animation(lv_obj_t * btn) {
    lv_obj_t * win = lv_win_get_from_btn(btn);
    lv_obj_del(win);
    return LV_RES_INV;
}

static lv_obj_t * mbox1;
/*Called when a button is clicked*/
static lv_res_t mbox_apply_action(lv_obj_t * mbox, const char * txt) {
    if(strcmp(txt,"重启")==0) {
         __set_FAULTMASK(1);
        NVIC_SystemReset();
    } else {
        lv_obj_del(mbox1);
    }
    return LV_RES_OK; /*Return OK if the message box is not deleted*/
}

lv_obj_t * ip[0];
static lv_style_t style_bg;
/*Create 2 button styles*/
static lv_style_t style_btn_rel;
static lv_style_t style_btn_pr;

/*Called when a button is released ot long pressed*/
static lv_res_t btnm_action(lv_obj_t * btnm, const char *txt) {
    if(strcmp(txt,"删除")==0) {
        lv_ta_del_char (ip[0]);
    } else if(strcmp(txt,"确定")==0) {
            char g_ip[30];
            strcpy(g_ip, lv_ta_get_text(ip[0]));
            char num[20];
            uint8_t num_count = 0;
            uint8_t g_ip_count = 0;
            uint8_t save_ip[4];
            uint8_t save_ip_count = 0;
            while(strlen(g_ip)+1 > g_ip_count) {
                if( (g_ip[g_ip_count] == '.') || (strlen(g_ip) == g_ip_count) ){
                    save_ip[save_ip_count] = atoi(num);
                    save_ip_count++;
                    if(save_ip_count == 4) {
                        persistence_set_ip(save_ip);
                        
                        /*Copy the message box (The buttons will be copied too)*/
                        mbox1 = lv_mbox_create(lv_scr_act(), NULL);
                        lv_mbox_set_text(mbox1, "设置成功,是否重启\n");/*Set the text*/
                        /*Add two buttons*/
                        static const char * btns[] ={"\221重启", "\221退出", ""}; /*Button description. '\221' lv_btnm like control char*/
                        lv_mbox_add_btns(mbox1, btns, NULL);
                        lv_obj_set_width(mbox1, 250);
                        lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the corner*/
                        lv_mbox_set_action(mbox1,mbox_apply_action);
                        break;
                    }
                    memset(num, 0, 20);
                    num_count = 0;
                } else {
                    num[num_count] = g_ip[g_ip_count];
                    num_count++;
                }
                g_ip_count++;
            }
    } else {
        lv_ta_add_char(ip[0],txt[0]);
    }
    return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}

void ui_net_set_create(void) {
    lv_obj_t * win = lv_win_create(lv_scr_act(),NULL);
    lv_win_add_btn(win,SYMBOL_CLOSE,lv_win_close_action_animation);
    lv_win_set_title(win,"网络设置");
    lv_win_set_btn_size(win,WIN_BTN_HEIGHT);
    
    lv_style_copy(&style_bg, &lv_style_plain);
    style_bg.body.main_color = LV_COLOR_SILVER;
    style_bg.body.grad_color = LV_COLOR_SILVER;
    style_bg.body.padding.hor = 0;
    style_bg.body.padding.ver = 0;
    style_bg.body.padding.inner = 0;

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
    
    /*Create a one lined test are with password mode*/
    ip[0] = lv_ta_create(win, NULL);
    lv_obj_set_pos (ip[0],50, 0);
    lv_ta_set_style(ip[0],LV_TA_STYLE_BG, &style_bg);           /*Apply the background style*/
    lv_ta_set_one_line(ip[0], true);
    lv_ta_set_cursor_type(ip[0], LV_CURSOR_LINE);
    lv_ta_set_pwd_mode(ip[0], false);
    
    uint8_t g_ip[4];
    persistence_get_ip(g_ip);
    char buf[20];
    sprintf(buf, "%d.%d.%d.%d",g_ip[0],g_ip[1],g_ip[2],g_ip[3]);
    lv_ta_set_text(ip[0],buf);
    
    /*Create a second button matrix with the new styles*/
    lv_obj_t * key_board = lv_btnm_create(win, NULL);
    lv_btnm_set_map(key_board, btnm_map);
    lv_btnm_set_action(key_board, btnm_action);
    lv_btnm_set_style(key_board, LV_BTNM_STYLE_BG, &style_bg);
    lv_btnm_set_style(key_board, LV_BTNM_STYLE_BTN_REL, &style_btn_rel);
    lv_btnm_set_style(key_board, LV_BTNM_STYLE_BTN_PR, &style_btn_pr);
    lv_obj_set_size(key_board,300,130);
    lv_obj_align(key_board,ip[0], LV_ALIGN_OUT_BOTTOM_MID ,0,10);
}
 