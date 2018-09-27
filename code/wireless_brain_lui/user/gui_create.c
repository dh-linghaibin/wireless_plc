#include "gui_create.h"



static void setting_create(void);
static void sd_create(void);
static void alarm_create(void);





extern const lv_img_t img_camera;
extern const lv_img_t img_user;
extern const lv_img_t img_statistics;
extern const lv_img_t img_rss;
extern const lv_img_t img_music;
extern const lv_img_t img_home;
extern const lv_img_t img_comment;
extern const lv_img_t img_sd;
extern const lv_img_t img_cpu;
extern const lv_img_t img_alarm;
//关闭窗口动画的回调函数
static lv_res_t lv_win_close_action_animation_cb(lv_obj_t * win)
{
    lv_obj_del(win);
//    OUTPUT_STR("close win\n");
    return LV_RES_INV;
}
//关闭窗口动画
static lv_res_t lv_win_close_action_animation(lv_obj_t * btn)
{
    lv_obj_t * win = lv_win_get_from_btn(btn);
    /* Float in the button using a built-in function
     * Delay the animation with 50 ms and float in 800 ms. NULL means no end callback*/
    lv_obj_animate(win, LV_ANIM_FLOAT_TOP | LV_ANIM_OUT, 300, 50, lv_win_close_action_animation_cb);



}
static lv_res_t btn_alarm_action(lv_obj_t *obj)
{
    alarm_create();
}

static lv_res_t btn_sd_action(lv_obj_t *obj)
{
    sd_create();
}


static lv_res_t btn_setting_action(lv_obj_t *obj)
{
        setting_create();
}

void home_create(void)
{
    lv_theme_t *th = lv_theme_night_init(200, NULL);
    lv_theme_set_current(th);

    int16_t distance_obj=15;//图标之间的间距
    int16_t distance_scr =9;//图标与屏幕边框的间距
    int16_t img_width = 64;;
    //SCREEN
    lv_obj_t * scr = lv_scr_act();

    lv_obj_t * btn[9];
    lv_obj_t * img[9] ;
    static lv_style_t btn_style_rel,btn_style_pr;
    lv_style_copy(&btn_style_rel,&lv_style_transp);
    lv_style_copy(&btn_style_pr,&lv_style_plain_color);
    btn_style_pr.body.opa = 50;

    for(uint8_t j=0;j<3;j++)
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
    lv_img_set_src(img[0], &img_camera);
    lv_img_set_src(img[1], &img_user);
    lv_img_set_src(img[2], &img_statistics);
    lv_img_set_src(img[3], &img_rss);
    lv_img_set_src(img[4], &img_music);
    lv_img_set_src(img[5], &img_home);
    lv_img_set_src(img[6], &img_alarm);
    lv_img_set_src(img[7], &img_sd);
    lv_img_set_src(img[8], &img_cpu);

    lv_btn_set_action(btn[8],LV_BTN_ACTION_CLICK,btn_setting_action);
    lv_btn_set_action(btn[7],LV_BTN_ACTION_CLICK,btn_sd_action);
    lv_btn_set_action(btn[6],LV_BTN_ACTION_CLICK,btn_alarm_action);

}

static void bluetooth_list_create(void);
static lv_res_t bluetooth_list_action(lv_obj_t *obj)
{
    if(lv_sw_get_state(obj)==true)
    {
        bluetooth_list_create();
//        printf("true");
//        OUTPUT_STR("true\n");
    }
    else
    {

//        OUTPUT_STR("false\n");
    }

}

static void bluetooth_list_create(void)
{
    lv_obj_t * win = lv_win_create(lv_scr_act(),NULL);
    lv_win_add_btn(win,SYMBOL_CLOSE,lv_win_close_action_animation);
    lv_win_set_title(win,"Bluetooth");
    lv_win_set_btn_size(win,WIN_BTN_HEIGHT);

    lv_obj_t *list = lv_list_create(win,NULL);
    lv_obj_set_size(list,lv_win_get_width(win),WIN_INNER_HEIGHT);
    lv_obj_align(list,win,LV_ALIGN_CENTER,0,0);

    /* Float in the button using a built-in function
     * Delay the animation with 50 ms and float in 800 ms. NULL means no end callback*/
    lv_obj_animate(win, LV_ANIM_FLOAT_TOP | LV_ANIM_IN, 300, 50, NULL);
}


static void battery_chart_create(void);
static lv_res_t btn_battery_action(lv_obj_t *obj)
{
    battery_chart_create();
}
static void setting_create(void) {
    lv_obj_t * scr = lv_scr_act();
    //创建sys页面,使用page
//    lv_obj_t * page_sys = lv_page_create(scr,NULL);
//    lv_obj_set_size(page_sys,LV_HOR_RES,LV_VER_RES);
//    lv_page_set_sb_mode(page_sys,LV_SB_MODE_AUTO);
//    lv_obj_align(page_sys,scr,LV_ALIGN_CENTER,0,0);
//    lv_page_set_scrl_layout(page_sys, LV_LAYOUT_COL_L);     /*Arrange elements automatically*/
    //创建sys页面,使用win
    lv_obj_t * page_sys = lv_win_create(scr,NULL);
    lv_obj_set_size(page_sys,LV_HOR_RES,LV_VER_RES);
    lv_win_set_sb_mode(page_sys,LV_SB_MODE_AUTO);
    lv_obj_align(page_sys,scr,LV_ALIGN_CENTER,0,0);
    lv_win_set_layout(page_sys, LV_LAYOUT_COL_L);     /*Arrange elements automatically*/
    lv_win_set_title(page_sys,"Setting");
    lv_win_set_btn_size(page_sys,WIN_BTN_HEIGHT);
    lv_win_add_btn(page_sys,SYMBOL_CLOSE,lv_win_close_action_animation);


    lv_obj_t * cont_sysinfo = lv_cont_create(page_sys,NULL);
    lv_obj_set_width(cont_sysinfo,LV_HOR_RES-25);
    lv_cont_set_fit(cont_sysinfo,false,true);
    lv_page_glue_obj(cont_sysinfo , true);

    lv_obj_t * cont_bluetooth = lv_cont_create(page_sys,cont_sysinfo);
    lv_obj_t * cont_sdcard = lv_cont_create(page_sys,cont_sysinfo);
    lv_obj_t * cont_battery = lv_cont_create(page_sys,cont_sysinfo);
    lv_obj_t * cont_bright = lv_cont_create(page_sys,cont_sysinfo);
    lv_obj_t * cont_time = lv_cont_create(page_sys,cont_sysinfo);


    lv_obj_t * label_sysinfo = lv_label_create(cont_sysinfo,NULL);
    lv_label_set_text(label_sysinfo," #0ff000 SysInfo#");
    lv_label_set_recolor(label_sysinfo,true);
    lv_page_glue_obj(label_sysinfo , true);

    lv_obj_t * label_bluetooth = lv_label_create(cont_bluetooth,NULL);
    lv_label_set_text(label_bluetooth," #0ff000 Bluetooth#");
    lv_label_set_recolor(label_bluetooth,true);
    lv_page_glue_obj(label_bluetooth , true);

    lv_obj_t * label_sdcard= lv_label_create(cont_sdcard,NULL);
    lv_label_set_text(label_sdcard," #0ff000 SDCard#");
    lv_label_set_recolor(label_sdcard,true);
    lv_page_glue_obj(label_sdcard , true);


    lv_obj_t * label_battery= lv_label_create(cont_battery,NULL);
    lv_label_set_text(label_battery," #0ff000 Battery#");
    lv_label_set_recolor(label_battery,true);
    lv_page_glue_obj(label_battery , true);


    lv_obj_t * label_bright= lv_label_create(cont_bright,NULL);
    lv_label_set_text(label_bright," #0ff000 Bright#");
    lv_label_set_recolor(label_bright,true);
    lv_page_glue_obj(label_battery , true);


    lv_obj_t * label_time= lv_label_create(cont_time,NULL);
    lv_label_set_text(label_time," #0ff000 Time#");
    lv_label_set_recolor(label_time,true);
    lv_page_glue_obj(label_time , true);


    /*Create a style and use the new font*/
        static lv_style_t style1;
        lv_style_copy(&style1, &lv_style_pretty_color);
        style1.text.font = &lv_font_dejavu_10; /*Set the base font whcih is concatenated with the others*/
        style1.text.opa = 70;


    lv_obj_t * label_sysinfo_version = lv_label_create(cont_sysinfo,NULL);
    lv_label_set_text(label_sysinfo_version,"HW: 3.2.0\nSW: 0.0.1\nGUI: LittleVGL 5.1.0");
    lv_obj_align(label_sysinfo_version,label_sysinfo,LV_ALIGN_OUT_BOTTOM_LEFT,30,0);
    lv_label_set_style(label_sysinfo_version, &style1);

    lv_obj_t * label_bluetooth_mac = lv_label_create(cont_bluetooth,label_sysinfo_version);
    lv_label_set_text(label_bluetooth_mac,"mac:\nname:");
    lv_obj_align(label_bluetooth_mac,label_bluetooth,LV_ALIGN_OUT_BOTTOM_LEFT,30,0);

    lv_obj_t * sw_bluetooth = lv_sw_create(cont_bluetooth,NULL);
    lv_obj_align(sw_bluetooth,label_bluetooth,LV_ALIGN_OUT_LEFT_MID,190,0);
    lv_sw_on(sw_bluetooth);
    lv_page_glue_obj(sw_bluetooth , true);
    lv_sw_set_action(sw_bluetooth,bluetooth_list_action);

    //sdcard
    static uint32_t TF_TotalSize = 7000;
    static uint32_t TF_FreeSize = 6000;

    lv_obj_t *label_sdcard_cap = lv_label_create(cont_sdcard,label_sysinfo_version);
    char buffer[60];
    sprintf(buffer,"Total cap:%dMB\nFree cap :%dMB",TF_TotalSize,TF_FreeSize);
    lv_label_set_text(label_sdcard_cap,buffer);
    lv_label_set_recolor(label_sdcard_cap, true);
    lv_obj_align(label_sdcard_cap, label_sdcard, LV_ALIGN_OUT_BOTTOM_LEFT, 30, 0);


    lv_obj_t * bar_sdcard  = lv_bar_create(cont_sdcard,NULL);
    lv_bar_set_range(bar_sdcard,0,100);
    lv_obj_set_size(bar_sdcard,180,15);
    lv_bar_set_value(bar_sdcard,(TF_TotalSize-TF_FreeSize)*100/TF_TotalSize);
    lv_obj_align(bar_sdcard, label_sdcard_cap, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);


    //battery
    lv_obj_t *label_battery_percent = lv_label_create(cont_battery,label_sysinfo_version);
    lv_label_set_text(label_battery_percent,"70%");
    lv_obj_align(label_battery_percent, label_battery, LV_ALIGN_OUT_LEFT_MID,190, 0);

    static lv_style_t btn_battery_rel_style,btn_battery_pr_style;
    lv_style_copy(&btn_battery_rel_style,&lv_style_transp);
    lv_style_copy(&btn_battery_pr_style,&lv_style_transp);


    lv_obj_t *btn_battery = lv_btn_create(cont_battery,NULL);
    lv_obj_set_size(btn_battery,40,20);
    lv_obj_align(btn_battery,label_battery_percent,LV_ALIGN_CENTER,0,0);
    lv_btn_set_action(btn_battery,LV_BTN_ACTION_PR,btn_battery_action);

    lv_btn_set_style(btn_battery,LV_BTN_STYLE_REL,&btn_battery_rel_style);
    lv_btn_set_style(btn_battery,LV_BTN_STYLE_REL,&btn_battery_pr_style);


    /* Float in the button using a built-in function
     * Delay the animation with 50 ms and float in 800 ms. NULL means no end callback*/
    lv_obj_animate(page_sys, LV_ANIM_FLOAT_TOP | LV_ANIM_IN, 300, 50, NULL);


}



static void battery_chart_create(void)
{
    lv_obj_t * win = lv_win_create(lv_scr_act(),NULL);
    lv_win_add_btn(win,SYMBOL_CLOSE,lv_win_close_action_animation);
    lv_win_set_title(win,"Battery");
    lv_win_set_btn_size(win,WIN_BTN_HEIGHT);

    lv_obj_t *chart = lv_chart_create(win,NULL);
    lv_obj_set_size(chart,lv_win_get_width(win),WIN_INNER_HEIGHT);
    lv_obj_align(chart,win,LV_ALIGN_CENTER,0,0);

    lv_chart_set_point_count(chart,100);
    lv_chart_set_range(chart,1500,3300);
    lv_chart_series_t * battery_value = lv_chart_add_series(chart,LV_COLOR_GREEN);

    lv_chart_init_points(chart, battery_value, 3300);
    int16_t value[]={3209,3200,3200,3198,3150,3120,3100,3090,3080,3060,0,0,0,0,0,0,0,0,0,0,0};
    for(uint8_t i=0;i<20;i++)
    {
        lv_chart_set_next(chart,battery_value,value[i]);
    }
    /* Float in the button using a built-in function
     * Delay the animation with 50 ms and float in 800 ms. NULL means no end callback*/
    lv_obj_animate(win, LV_ANIM_FLOAT_TOP | LV_ANIM_IN, 300, 50, NULL);

}

static void sd_create(void)
{
    lv_obj_t * win = lv_win_create(lv_scr_act(),NULL);
    lv_win_add_btn(win,SYMBOL_CLOSE,lv_win_close_action_animation);
    lv_win_set_title(win,"SDCard");
    lv_win_set_btn_size(win,WIN_BTN_HEIGHT);
//    lv_obj_set_pos(win,0,LV_VER_RES);
    lv_obj_t *list = lv_list_create(win,NULL);
    lv_obj_set_size(list,lv_win_get_width(win),WIN_INNER_HEIGHT);
    lv_obj_align(list,win,LV_ALIGN_CENTER,0,0);

    lv_list_add(list,SYMBOL_FILE,"haha1.txt",NULL);
    lv_list_add(list,SYMBOL_FILE,"haha2.txt",NULL);
    lv_list_add(list,SYMBOL_FILE,"haha3.txt",NULL);
    lv_list_add(list,SYMBOL_FILE,"haha4.txt",NULL);
    lv_list_add(list,SYMBOL_FILE,"haha5.txt",NULL);
    lv_list_add(list,SYMBOL_FILE,"haha7.txt",NULL);
    lv_list_add(list,SYMBOL_FILE,"haha6.txt",NULL);
    lv_list_add(list,SYMBOL_FILE,"haha8.txt",NULL);
    lv_list_add(list,SYMBOL_FILE,"haha9.txt",NULL);
    lv_list_add(list,SYMBOL_FILE,"haha0.txt",NULL);

    /* Float in the button using a built-in function
     * Delay the animation with 50 ms and float in 800 ms. NULL means no end callback*/
    lv_obj_animate(win, LV_ANIM_FLOAT_TOP | LV_ANIM_IN, 300, 50, NULL);

}



//关闭窗口动画
static lv_res_t btn_save_action(lv_obj_t * btn)
{
//    lv_obj_t * win = lv_obj_get_parent(btn);
//    /* Float in the button using a built-in function
//     * Delay the animation with 50 ms and float in 800 ms. NULL means no end callback*/
    lv_obj_animate(btn, LV_ANIM_GROW_H , 300, 50, NULL);
}


static void alarm_create(void)
{
    lv_obj_t * win = lv_win_create(lv_scr_act(),NULL);
    lv_win_add_btn(win,SYMBOL_CLOSE,lv_win_close_action_animation);
    lv_win_set_title(win,"Alarm");
    lv_win_set_btn_size(win,WIN_BTN_HEIGHT);
//    lv_win_set_layout(win,);
//    lv_obj_set_pos(win,0,LV_VER_RES);
    //创建roller，时分

    lv_obj_t * roller[2];
    roller[0] = lv_roller_create(win,NULL);    
    
    lv_roller_set_options(roller[0]," 00 \n 01 \n 02 \n 03 \n 04 \n 05 \n 06 \n 07 \n 08 \n 09 \n 10 \n 11 \n 12 \n 13 \n 14 \n 15 \n 16 \n 17 \n 18 \n 19 \n 20 \n 21 \n 22 \n 23 ");
    lv_roller_set_visible_row_count(roller[0],5);    
    lv_roller_set_selected(roller[0],2,true);
    lv_obj_align(roller[0],win,LV_ALIGN_IN_LEFT_MID,0,0);
    
    
    roller[1] = lv_roller_create(win,NULL);
    lv_roller_set_options(roller[1]," 01 \n 02 \n 03 \n 04 \n 05 \n 06 \n 07 \n 08 \n 09 \n 10 \n 11 \n 12 ");
    lv_roller_set_visible_row_count(roller[1],5);    
    lv_obj_align(roller[1],roller[0],LV_ALIGN_OUT_RIGHT_MID,20,0);

    
    //创建 按键和标签，保存
    lv_obj_t *btn_save = lv_btn_create(win,NULL);
    lv_obj_set_size(btn_save,100,50);    
    lv_obj_align(btn_save,win,LV_ALIGN_IN_RIGHT_MID,0,0);
    lv_btn_set_action(btn_save,LV_BTN_ACTION_CLICK,btn_save_action);
    
    
    lv_obj_t *label_save = lv_label_create(win,NULL);
    lv_label_set_text(label_save,SYMBOL_SAVE"#0f00f0 save#");
    lv_label_set_recolor(label_save,true);
    lv_obj_align(label_save,btn_save,LV_ALIGN_CENTER,0,0);
}























