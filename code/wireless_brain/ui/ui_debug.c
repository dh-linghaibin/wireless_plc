/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "ui_debug.h"
#include "time_set.h"
#include "app_configure.h"
#include "persistence.h"

LV_FONT_DECLARE(arial_ascii_20);/*Full ASCII range*/

static lv_obj_t * label = NULL;

static lv_res_t lv_win_close_action_animation(lv_obj_t * btn) {
    lv_obj_t * win = lv_win_get_from_btn(btn);
    lv_obj_del(win);
    label = NULL;
    return LV_RES_INV;
}
static lv_obj_t * mbox1;
/*Called when a button is clicked*/
static lv_res_t mbox_apply_action(lv_obj_t * mbox, const char * txt) {
     if(strcmp(txt,"Êõ¥Êñ∞")==0) {
        uint8_t buf = 0x55;
        persistence_set_pro_flag(&buf);
        __set_FAULTMASK(1);//πÿ±’◊‹÷–∂œ
        NVIC_SystemReset();//«Î«Ûµ•∆¨ª˙÷ÿ∆Ù
     } else {
        lv_obj_del(mbox1);
     }
    return LV_RES_OK; /*Return OK if the message box is not deleted*/
}

static lv_res_t lv_win_download(lv_obj_t * btn) {
     /*Copy the message box (The buttons will be copied too)*/
    mbox1 = lv_mbox_create(lv_scr_act(), NULL);
    lv_mbox_set_text(mbox1, "Êõ¥Êñ∞Á®ãÂ∫è\n");/*Set the text*/
    /*Add two buttons*/
    static const char * btns[] ={"\221Êõ¥Êñ∞", "\221ÈÄÄÂá∫", ""}; /*Button description. '\221' lv_btnm like control char*/
    lv_mbox_add_btns(mbox1, btns, NULL);
    lv_obj_set_width(mbox1, 250);
    lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the corner*/
    lv_mbox_set_action(mbox1,mbox_apply_action);
}

static lv_res_t btn_click_action(lv_obj_t * btn) {
    uint8_t id = lv_obj_get_free_num(btn);
    //printf("Button %d is released\n", id);
    /* The button is released.
     * Make something here */
    //lv_label_ins_text (label,LV_LABEL_POS_LAST,"123");
    __set_FAULTMASK(1);//πÿ±’◊‹÷–∂œ
    NVIC_SystemReset();//«Î«Ûµ•∆¨ª˙÷ÿ∆Ù
    
    return LV_RES_OK; /*Return OK if the button is not deleted*/
}

static lv_style_t style_sb;
static lv_style_t bebug_tes_sty;
void ui_debug_create(void) {
    lv_obj_t * win = lv_win_create(lv_scr_act(),NULL);
    lv_win_add_btn(win,SYMBOL_CLOSE,lv_win_close_action_animation);
    lv_win_add_btn(win,SYMBOL_DOWNLOAD,lv_win_download);
    lv_win_set_title(win,"Ë∞ÉËØï");
    lv_win_set_btn_size(win,WIN_BTN_HEIGHT);
    
    /*Create a scroll bar style*/
    lv_style_copy(&style_sb, &lv_style_plain);
    style_sb.body.main_color = LV_COLOR_BLACK;
    style_sb.body.grad_color = LV_COLOR_BLACK;
    style_sb.body.border.color = LV_COLOR_WHITE;
    style_sb.body.border.width = 1;
    style_sb.body.border.opa = LV_OPA_10;
    style_sb.body.radius = LV_RADIUS_CIRCLE;
    style_sb.body.opa = LV_OPA_60;
    style_sb.body.padding.hor = 3;          /*Horizontal padding on the right*/
    style_sb.body.padding.inner = 8;        /*Scrollbar width*/
    style_sb.text.font = &arial_ascii_20;
    
    /*Create a label to show the test result*/
    lv_style_copy(&bebug_tes_sty, &lv_style_pretty);
    bebug_tes_sty.body.main_color = LV_COLOR_HEX3(0x000);
    bebug_tes_sty.body.grad_color = LV_COLOR_HEX3(0x000);
    bebug_tes_sty.body.border.color = LV_COLOR_HEX3(0x000);
    bebug_tes_sty.body.border.width = 0;
    bebug_tes_sty.body.border.part = LV_BORDER_NONE;
    bebug_tes_sty.body.border.opa = LV_OPA_20;
    bebug_tes_sty.body.shadow.color = LV_COLOR_HEX3(0x000);
    bebug_tes_sty.body.shadow.width = 0;
    bebug_tes_sty.body.radius = 0;
    bebug_tes_sty.body.opa = LV_OPA_20;
    bebug_tes_sty.text.color = LV_COLOR_HEX3(0x000);
    bebug_tes_sty.text.font = &arial_ascii_20;
    
    label = lv_ta_create(win, NULL);
    lv_obj_set_size(label, lv_page_get_scrl_width(win), lv_obj_get_height(win)/1.5);
    lv_ta_set_style(label, LV_TA_STYLE_BG, &bebug_tes_sty);
    lv_ta_set_text(label, "");
    
     /*Create a normal button*/
//    lv_obj_t * btn1 = lv_btn_create(win, NULL);
//    lv_btn_set_action(btn1, LV_BTN_ACTION_CLICK, btn_click_action);
//    lv_obj_t * but_name = lv_label_create(btn1, NULL);
//    lv_label_set_text(but_name, "stop");
}

#include "task_gui.h"

void ui_debug_set_show(const char * text) {
    if(label != NULL) {
        if(task_gui_get_sleep_flag() == 0) {
            lv_ta_add_text(label,text);
        }
        //printf("%s",text);
    }
}
