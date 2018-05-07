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

extern const lv_img_t benchmark_bg;
extern const lv_img_t l_back;
extern const lv_img_t img_alarm;
extern const lv_img_t img_cpu;
extern const lv_img_t img_rss;

static lv_res_t btn_alarm_action(lv_obj_t *obj) {
    time_set_create();
}

static lv_res_t btn_set_action(lv_obj_t *obj) {
    ui_debug_create();
}

static lv_res_t btn_set_net(lv_obj_t *obj) {
    ui_net_set_create();
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

    lv_obj_t * btn[9];
    lv_obj_t * img[9] ;
   
    lv_style_copy(&btn_style_rel,&lv_style_transp);
    lv_style_copy(&btn_style_pr,&lv_style_plain_color);
    btn_style_pr.body.opa = 50;
    
     lv_style_copy(&style_wp, &lv_style_plain);
    style_wp.image.color = LV_COLOR_RED;
    
    /*Create a wallpaper on the page*/
    wp = lv_img_create(lv_scr_act(), NULL);
    lv_obj_set_protect(wp, LV_PROTECT_PARENT);          /*Don't let to move the wallpaper by the layout */
    //  	lv_obj_set_parent(wp, holder_page);
    //  	lv_obj_set_parent(lv_page_get_scrl(holder_page), holder_page);
    lv_img_set_src(wp, &benchmark_bg);
    lv_obj_set_size(wp, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(wp, 0, 0);
    lv_obj_set_hidden(wp, false);
    lv_img_set_style(wp, &style_wp);
    lv_img_set_auto_size(wp, false);

    for(uint8_t j=0;j<1;j++)
    {
        for(uint8_t i=0;i<3;i++)
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
//    lv_img_set_src(img[3], &img_rss);
//    lv_img_set_src(img[4], &img_music);
//    lv_img_set_src(img[5], &img_home);
//    lv_img_set_src(img[0], &img_alarm);
//    lv_img_set_src(img[7], &img_sd);
//    lv_img_set_src(img[8], &img_cpu);
    lv_btn_set_action(btn[0],LV_BTN_ACTION_CLICK,btn_alarm_action);
    lv_btn_set_action(btn[1],LV_BTN_ACTION_CLICK,btn_set_action);
    lv_btn_set_action(btn[2],LV_BTN_ACTION_CLICK,btn_set_net);
}


//void lv_tutorial_objects(void)
//{

//    /********************
//     * CREATE A SCREEN
//     *******************/
//    /* Create a new screen and load it
//     * Screen can be created from any type object type
//     * Now a Page is used which is an objects with scrollable content*/
//    lv_obj_t *scr = lv_page_create(NULL, NULL);
//    lv_scr_load(scr);

//    /****************
//     * ADD A TITLE
//     ****************/
//    lv_obj_t *label = lv_label_create(scr, NULL);  /*First parameters (scr) is the parent*/
//    lv_label_set_text(label, "Object usage demo");  /*Set the text*/
//    lv_obj_set_x(label, 50);                        /*Set the x coordinate*/

//    /***********************
//     * CREATE TWO BUTTONS
//     ***********************/
//    /*Create a button*/
//    lv_obj_t *btn1 = lv_btn_create(lv_scr_act(), NULL);          /*Create a button on the currently loaded screen*/
//    lv_btn_set_action(btn1, LV_BTN_ACTION_CLICK, btn_rel_action); /*Set function to be called when the button is released*/
//    lv_obj_align(btn1, label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);  /*Align below the label*/

//    /*Create a label on the button (the 'label' variable can be reused)*/
//    label = lv_label_create(btn1, NULL);
//    lv_label_set_text(label, "Button 1");

//    /*Copy the previous button*/
//    lv_obj_t *btn2 = lv_btn_create(lv_scr_act(), btn1);         /*Second parameter is an object to copy*/
//    lv_obj_align(btn2, btn1, LV_ALIGN_OUT_RIGHT_MID, 50, 0);    /*Align next to the prev. button.*/

//    /*Create a label on the button*/
//    label = lv_label_create(btn2, NULL);
//    lv_label_set_text(label, "Button 2");

//    /****************
//     * ADD A SLIDER
//     ****************/
//    lv_obj_t *slider = lv_slider_create(scr, NULL);                             /*Create a slider*/
//    lv_obj_set_size(slider, lv_obj_get_width(lv_scr_act())  / 3, LV_DPI / 3);   /*Set the size*/
//    lv_obj_align(slider, btn1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);                /*Align below the first button*/
//    lv_slider_set_value(slider, 30);                                            /*Set the current value*/

//    /***********************
//     * ADD A DROP DOWN LIST
//     ************************/
//    lv_obj_t * ddlist = lv_ddlist_create(lv_scr_act(), NULL);            /*Create a drop down list*/
//    lv_obj_align(ddlist, slider, LV_ALIGN_OUT_RIGHT_TOP, 50, 0);         /*Align next to the slider*/
//    lv_obj_set_free_ptr(ddlist, slider);                                   /*Save the pointer of the slider in the ddlist (used in 'ddlist_action()')*/
//    lv_obj_set_top(ddlist, true);                                        /*Enable to be on the top when clicked*/
//    lv_ddlist_set_options(ddlist, "None\nLittle\nHalf\nA lot\nAll"); /*Set the options*/
//    lv_ddlist_set_action(ddlist, ddlist_action);                         /*Set function to call on new option is chosen*/

//    /****************
//     * CREATE A CHART
//     ****************/
//    lv_obj_t * chart = lv_chart_create(lv_scr_act(), NULL);                         /*Create the chart*/
//    lv_obj_set_size(chart, lv_obj_get_width(scr) / 2, lv_obj_get_width(scr) / 4);   /*Set the size*/
//    lv_obj_align(chart, slider, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 50);                   /*Align below the slider*/
//    lv_chart_set_series_width(chart, 3);                                            /*Set the line width*/

//    /*Add a RED data series and set some points*/
//    lv_chart_series_t * dl1 = lv_chart_add_series(chart, LV_COLOR_RED);
//    lv_chart_set_next(chart, dl1, 10);
//    lv_chart_set_next(chart, dl1, 25);
//    lv_chart_set_next(chart, dl1, 45);
//    lv_chart_set_next(chart, dl1, 80);

//    /*Add a BLUE data series and set some points*/
//    lv_chart_series_t * dl2 = lv_chart_add_series(chart, LV_COLOR_MAKE(0x40, 0x70, 0xC0));
//    lv_chart_set_next(chart, dl2, 10);
//    lv_chart_set_next(chart, dl2, 25);
//    lv_chart_set_next(chart, dl2, 45);
//    lv_chart_set_next(chart, dl2, 80);
//    lv_chart_set_next(chart, dl2, 75);
//    lv_chart_set_next(chart, dl2, 505);

//}
 