#include "gui_demo.h"

/*********************
 *      INCLUDES
 *********************/
//lvgl
#include "../lv_hal/lv_hal_disp.h"
#include "../lv_misc/lv_mem.h"
#include "../lv_core/lv_obj.h"
#include "../lvgl.h"
#include "../lv_themes/lv_theme.h"

/*********************
 *      DEFINES
 *********************/
#define DEMO_WALLPAPER  0

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void write_create(lv_obj_t *parent);
static void list_create(lv_obj_t *parent);
static void chart_create(lv_obj_t *parent);
static lv_res_t slider_action(lv_obj_t *slider);
static lv_res_t list_btn_action(lv_obj_t *slider);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t *chart;
static lv_obj_t *ta;
#if DEMO_WALLPAPER
LV_IMG_DECLARE(img_bubble_pattern);
#endif

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a demo application
 */
void demo_create(void)
{
    lv_theme_t * th = lv_theme_zen_init(210, &lv_font_dejavu_20);     //Set a HUE value and a Font for the Night Theme
    lv_theme_set_current(th);     
#if DEMO_WALLPAPER
    lv_img_create_file("bg", img_bubble_pattern);
    lv_obj_t *wp = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_upscale(wp, true);
    lv_img_set_file(wp, "U:/bg");
    lv_obj_set_width(wp, LV_HOR_RES * 4);
    lv_obj_set_protect(wp, LV_PROTECT_POS);
#endif

    static lv_style_t style_tv_btn_bg;
    lv_style_copy(&style_tv_btn_bg, &lv_style_plain);
    style_tv_btn_bg.body.main_color = LV_COLOR_HEX(0x487fb7);
    style_tv_btn_bg.body.grad_color = LV_COLOR_HEX(0x487fb7);
    style_tv_btn_bg.body.padding.ver = 0;

    static lv_style_t style_tv_btn_rel;
    lv_style_copy(&style_tv_btn_rel, &lv_style_btn_rel);
    style_tv_btn_rel.body.empty = 1;
    style_tv_btn_rel.body.border.width = 0;

    static lv_style_t style_tv_btn_pr;
    lv_style_copy(&style_tv_btn_pr, &lv_style_btn_pr);
    style_tv_btn_pr.body.radius = 0;
    style_tv_btn_pr.body.opa = LV_OPA_50;
    style_tv_btn_pr.body.main_color = LV_COLOR_WHITE;
    style_tv_btn_pr.body.grad_color = LV_COLOR_WHITE;
    style_tv_btn_pr.body.border.width = 0;
    style_tv_btn_pr.text.color = LV_COLOR_GRAY;

    lv_obj_t *tv = lv_tabview_create(lv_scr_act(), NULL);

#if DEMO_WALLPAPER
    lv_obj_set_parent(wp, ((lv_tabview_ext_t *) tv->ext_attr)->content);
    lv_obj_set_pos(wp, 0, -5);
#endif

    lv_obj_t *tab1 = lv_tabview_add_tab(tv, "Write");
    lv_obj_t *tab2 = lv_tabview_add_tab(tv, "List");
    lv_obj_t *tab3 = lv_tabview_add_tab(tv, "Chart");

#if DEMO_WALLPAPER == 0
    /*Blue bg instead of wallpaper*/
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BG, &style_tv_btn_bg);
#endif
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_BG, &style_tv_btn_bg);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_INDIC, &lv_style_plain);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_REL, &style_tv_btn_rel);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_PR, &style_tv_btn_pr);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_TGL_REL, &style_tv_btn_rel);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_TGL_PR, &style_tv_btn_pr);

    write_create(tab1);
    list_create(tab2);
    chart_create(tab3);
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

static void write_create(lv_obj_t *parent)
{
//    lv_page_set_sb_mode(parent, LV_SB_MODE_OFF);
    lv_page_set_style(parent, LV_PAGE_STYLE_BG, &lv_style_transp_fit);
    lv_page_set_style(parent, LV_PAGE_STYLE_SCRL, &lv_style_transp_fit);

    static lv_style_t style_ta;
    lv_style_copy(&style_ta, &lv_style_pretty);
    style_ta.body.opa = LV_OPA_30;
    style_ta.body.radius = 0;
    style_ta.text.color = LV_COLOR_WHITE;

    static lv_style_t style_kb;
    lv_style_copy(&style_kb, &lv_style_plain);
    style_kb.body.opa = LV_OPA_70;
    style_kb.body.main_color = LV_COLOR_HEX3(0x333);
    style_kb.body.grad_color = LV_COLOR_HEX3(0x333);
    style_kb.body.padding.hor = 0;
    style_kb.body.padding.ver = 0;
    style_kb.body.padding.inner = 0;

    static lv_style_t style_kb_rel;
    lv_style_copy(&style_kb_rel, &lv_style_plain);
    style_kb_rel.body.empty = 1;
    style_kb_rel.body.radius = 0;
    style_kb_rel.body.border.width = 1;
    style_kb_rel.body.border.color = LV_COLOR_SILVER;
    style_kb_rel.body.border.opa = LV_OPA_50;
    style_kb_rel.text.color = LV_COLOR_WHITE;

    static lv_style_t style_kb_pr;
    lv_style_copy(&style_kb_pr, &lv_style_plain);
    style_kb_pr.body.radius = 0;
    style_kb_pr.body.opa = LV_OPA_50;
    style_kb_pr.body.main_color = LV_COLOR_WHITE;
    style_kb_pr.body.grad_color = LV_COLOR_WHITE;
    style_kb_pr.body.border.width = 1;
    style_kb_pr.body.border.color = LV_COLOR_SILVER;

    ta = lv_ta_create(parent, NULL);
    lv_obj_set_size(ta, lv_page_get_scrl_width(parent), lv_obj_get_height(parent) / 2);
    lv_ta_set_style(ta, LV_TA_STYLE_BG, &style_ta);
    lv_ta_set_text(ta, "");

    lv_obj_t *kb = lv_kb_create(parent, NULL);
    lv_obj_set_size(kb, lv_page_get_scrl_width(parent), lv_obj_get_height(parent) / 2);
    lv_obj_align(kb, ta, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_kb_set_ta(kb, ta);
    lv_kb_set_style(kb, LV_KB_STYLE_BG, &style_kb);
    lv_kb_set_style(kb, LV_KB_STYLE_BTN_REL, &style_kb_rel);
    lv_kb_set_style(kb, LV_KB_STYLE_BTN_PR, &style_kb_pr);
}

static void list_create(lv_obj_t *parent)
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
    lv_obj_t * slider1;
    lv_obj_t * gauge1;
    /*Create a gauge*/
    gauge1 = lv_gauge_create(parent, NULL);
    lv_gauge_set_style(gauge1, &style);
    lv_gauge_set_needle_count(gauge1, 3, needle_colors);
    lv_obj_align(gauge1, NULL, LV_ALIGN_CENTER, 0, 20);

    /*Set the values*/
    lv_gauge_set_value(gauge1, 0, 10);
    lv_gauge_set_value(gauge1, 1, 20);
    lv_gauge_set_value(gauge1, 2, 30);
    
    
    /*Create a default slider*/
    slider1 = lv_slider_create(parent, NULL);
    lv_obj_set_size(slider1, 160, 30);
    lv_obj_align(slider1, NULL, LV_ALIGN_IN_TOP_RIGHT, -30, 30);
    lv_slider_set_action(slider1, slider_action);
    lv_bar_set_value(slider1, 70);
}

static void chart_create(lv_obj_t *parent)
{
    lv_page_set_style(parent, LV_PAGE_STYLE_BG, &lv_style_transp_fit);
    lv_page_set_style(parent, LV_PAGE_STYLE_SCRL, &lv_style_transp_fit);

    lv_page_set_scrl_fit(parent, false, false);
    lv_page_set_scrl_height(parent, lv_obj_get_height(parent));
    lv_page_set_sb_mode(parent, LV_SB_MODE_OFF);

    static lv_style_t style_chart;
    lv_style_copy(&style_chart, &lv_style_pretty);
    style_chart.body.opa = LV_OPA_60;
    style_chart.body.radius = 0;
    style_chart.line.color = LV_COLOR_GRAY;

    chart = lv_chart_create(parent, NULL);
    lv_obj_set_size(chart, 2 * lv_obj_get_width(parent) / 3, lv_obj_get_height(parent) / 2);
    lv_obj_align(chart, NULL,  LV_ALIGN_IN_TOP_MID, 0, LV_DPI / 4);
    lv_chart_set_type(chart, LV_CHART_TYPE_COLUMN);
    lv_chart_set_style(chart, &style_chart);
    lv_chart_set_series_opa(chart, LV_OPA_70);
    lv_chart_series_t *ser1;
    ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
    lv_chart_set_next(chart, ser1, 40);
    lv_chart_set_next(chart, ser1, 30);
    lv_chart_set_next(chart, ser1, 47);
    lv_chart_set_next(chart, ser1, 59);
    lv_chart_set_next(chart, ser1, 59);
    lv_chart_set_next(chart, ser1, 31);
    lv_chart_set_next(chart, ser1, 55);
    lv_chart_set_next(chart, ser1, 70);
    lv_chart_set_next(chart, ser1, 82);

    /*Create a bar, an indicator and a knob style*/
    static lv_style_t style_bar;
    static lv_style_t style_indic;
    static lv_style_t style_knob;

    lv_style_copy(&style_bar, &lv_style_pretty);
    style_bar.body.main_color =  LV_COLOR_BLACK;
    style_bar.body.grad_color =  LV_COLOR_GRAY;
    style_bar.body.radius = LV_RADIUS_CIRCLE;
    style_bar.body.border.color = LV_COLOR_WHITE;
    style_bar.body.opa = LV_OPA_60;
    style_bar.body.padding.hor = 0;
    style_bar.body.padding.ver = LV_DPI / 6;

    lv_style_copy(&style_indic, &lv_style_pretty);
    style_indic.body.grad_color =  LV_COLOR_MARRON;
    style_indic.body.main_color =  LV_COLOR_RED;
    style_indic.body.radius = LV_RADIUS_CIRCLE;
    style_indic.body.shadow.width = LV_DPI / 10;
    style_indic.body.shadow.color = LV_COLOR_RED;
    style_indic.body.padding.hor = LV_DPI / 30;
    style_indic.body.padding.ver = LV_DPI / 30;

    lv_style_copy(&style_knob, &lv_style_pretty);
    style_knob.body.radius = LV_RADIUS_CIRCLE;
    style_knob.body.opa = LV_OPA_70;

    /*Create a second slider*/
    lv_obj_t *slider = lv_slider_create(parent, NULL);
    lv_slider_set_style(slider, LV_SLIDER_STYLE_BG, &style_bar);
    lv_slider_set_style(slider, LV_SLIDER_STYLE_INDIC, &style_indic);
    lv_slider_set_style(slider, LV_SLIDER_STYLE_KNOB, &style_knob);
    lv_obj_set_size(slider, lv_obj_get_width(chart), LV_DPI / 2);
    lv_obj_align(slider, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, - LV_DPI / 4); /*Align below the slider*/
    lv_slider_set_action(slider, slider_action);
    lv_slider_set_range(slider, 10, 1000);
    lv_slider_set_value(slider, 700);
    slider_action(slider);          /*Simulate a user value set the refresh the chart*/
}

/**
 * Called when a new value on the slider on the Chart tab is set
 * @param slider pointer to the slider
 * @return LV_RES_OK because the slider is not deleted in the function
 */
static lv_res_t slider_action(lv_obj_t *slider)
{
    int16_t v = lv_slider_get_value(slider);
    v = 1000 * 100 / v; /*Convert to range modify values linearly*/
    lv_chart_set_range(chart, 0, v);

    return LV_RES_OK;
}

/**
 * Called when a a list button is clicked on the List tab
 * @param btn pointer to a list button
 * @return LV_RES_OK because the button is not deleted in the function
 */
static lv_res_t list_btn_action(lv_obj_t *btn)
{
    lv_ta_add_char(ta, '\n');
    lv_ta_add_text(ta, lv_list_get_btn_text(btn));

    return LV_RES_OK;
}


//    lv_vdb_init();
//    lv_init();
//    lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
//    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/
//    /*Set up the functions to access to your display*/
//    disp_drv.disp_flush = monitor_flush;            /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/
//    //disp_drv.disp_fill = tft_fill;              /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
//    //disp_drv.disp_map = tft_fill;                /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/

//    lv_indev_drv_t indev_drv;
//    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
//    indev_drv.type = LV_INDEV_TYPE_POINTER;
//    indev_drv.read = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
//    lv_indev_drv_register(&indev_drv);

//    lv_disp_drv_register(&disp_drv);
//    /*Create a simple base object*/
//    //  lv_test_object_1();
//    demo_create();
//    //vTaskStartScheduler();
// lv_task_handler();
// lv_tick_inc(1);




//    FATFS fs;
//    u8 res=f_mount(&fs,"1:",1);                 //挂载FLASH.    
//    if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
//    {
//        printf("Flash Disk Formatting...\n");    //格式化FLASH
//        res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
//        if(res==0)
//        {
//            f_setlabel((const TCHAR *)"1:ALIENTEK");    //设置Flash磁盘的名字为：ALIENTEK
//            printf("Flash Disk Format Finish\n");    //格式化完成
//        } else { 
//            printf("Flash Disk Format Error \n");    //格式化失败
//        }
//        delay_ms(1000);
//    } else {
//        printf("Flash Disk ok \n");
//    }
//static int Delay_ms(lua_State *L)  
//{  
//    delay_ms(luaL_checkinteger(L, -1));  
//    return 0;  
//}  
//static int print(lua_State *L)  
//{  
//    int n=lua_gettop(L);  
//    int i;  
//    for (i=1; i<=n; i++)  
//    {  
//        if (lua_isstring(L,i))  
//            printf("%s",lua_tostring(L,i));  
//        else if (lua_isnil(L,i))  
//            printf("%s","nil");  
//        else if (lua_isboolean(L,i))  
//            printf("%s",lua_toboolean(L,i) ? "true" : "false");  
//        else  
//            printf("%s:%p",luaL_typename(L,i),lua_topointer(L,i));  
//     }  
//     return 0;  
//}  
//static luaL_Reg LuaLib1_0[] =   
//{   
//    //c接口函数都可以放在这里在lua中声明  
//    {"Delay_ms", Delay_ms},  
//    {"print", print},  
//  
//    {NULL, NULL}   
//};  


///* 测试的Lua代码字符串 */  
//const char lua_test[] = {   
//    "print(\"Hello,I am lua!\\n--this is newline printf\")\n"  
//    "function foo()\n"  
//    "  local i = 0\n"  
//    "  local sum = 1\n"  
//    "  while i <= 10 do\n"  
//    "    sum = sum * 2\n"  
//    "    i = i + 1\n"  
//    "  end\n"  
//    "return sum\n"  
//    "end\n"  
//    "print(\"sum =\", foo())\n"  
//    "print(\"and sum = 2^11 =\", 2 ^ 11)\n"  
//    "print(\"exp(200) =\", math.exp(200))\n"  
//};  

//uint8_t fatfs_test_mkdir(void) {
//    FIL fnew; /* 文件对象 */
//    FRESULT res_sd; /* 文件操作结果 */
//    UINT fnum; /* 文件成功读写数量 */
//    BYTE ReadBuffer[1024]= {0}; /* 读缓冲区 */
//    BYTE WriteBuffer[1024] = {0,};//
//    BYTE work[100]; /* Work area (larger is better for processing time) */
//    /*--------------------- 文件系统测试：写测试 -----------------------*/
//    /* 打开文件，如果文件不存在则创建它 */
////    printf("\r\n****** 即将进行文件写入测试... ******\r\n");
////    res_sd=f_open(&fnew,"1:test.lua",FA_CREATE_ALWAYS|FA_WRITE);
////    if ( res_sd == FR_OK ) {
////        printf("open ok write \r\n");
////        /* 将指定存储区内容写入到文件内 */
////        res_sd=f_write(&fnew,lua_test,sizeof(lua_test),&fnum);
////        if (res_sd==FR_OK) {
////            printf("ok %d\n",fnum);
////        } else {
////            printf("fale\n");
////        }
////        /* 不再读写，关闭文件 */
////        f_close(&fnew);
////    } else {
////        printf("open file\r\n");
////    }
//    /*------------------ 文件系统测试：读测试 --------------------------*/
//   // printf("file read\r\n");
//    res_sd=f_open(&fnew,"1:test.lua",FA_OPEN_EXISTING|FA_READ);
//    if (res_sd == FR_OK) {
//       // printf("open ok\r\n");
//        res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
//        if (res_sd==FR_OK) {
//            //printf("read: %d\r\n",fnum);
//            //printf("read-a: \r\n%s \r\n", ReadBuffer);
//        } else {
//           // printf("file (%d)\n",res_sd);
//        }
//    } else {
//        printf("file\r\n");
//    }
//    /* 不再读写，关闭文件 */
//    f_close(&fnew);
//    /* 不再使用文件系统，取消挂载文件系统 */
//    //f_mount(0,"0:",0);
//    
//     lua_State* L;  
//    L = luaL_newstate();  
//    luaL_openlibs(L);  
//  
//    luaL_newlibtable(L, LuaLib1_0);  
//    luaL_setfuncs(L, LuaLib1_0, 0);  
//    //这里定义全局变量把栈顶的table赋值给LuaLib1_0，这个方式使用模块是因为无法将c模块生成动态链接库.so或.dll给LUA_CPATH_DEFAULT加载  
//    lua_setglobal(L, "lhb");  
//    //dostring(L,"print('hello my name is a linghaibin haha ')","Test_lua");
//    luaL_dostring(L, ReadBuffer); /* 运行Lua脚本 */  
//    lua_close(L);
//}


//void Lua_task(void)  
//{   
//    lua_State* L;  
//    L = luaL_newstate();  
//    luaL_openlibs(L);  
//  
//    luaL_newlibtable(L, LuaLib1_0);  
//    luaL_setfuncs(L, LuaLib1_0, 0);  
//    //这里定义全局变量把栈顶的table赋值给LuaLib1_0，这个方式使用模块是因为无法将c模块生成动态链接库.so或.dll给LUA_CPATH_DEFAULT加载  
//    lua_setglobal(L, "lhb");  
//    //dostring(L,"print('hello my name is a linghaibin haha ')","Test_lua");
//    luaL_dostring(L, lua_test); /* 运行Lua脚本 */  
//    lua_close(L);
//}  

//void lv_test_roller_1(void)
//{
//    /* Default object*/
//    lv_obj_t *roller1 = lv_roller_create(lv_scr_act(), NULL);
//    lv_obj_set_pos(roller1, 10, 10);


//    static lv_style_t bg;
//    lv_style_copy(&bg, &lv_style_pretty);
//    bg.body.main_color = LV_COLOR_GRAY;
//    bg.body.grad_color = LV_COLOR_WHITE;
//    bg.body.shadow.width = 5;
//    bg.text.line_space = 10;
//    bg.text.opa = LV_OPA_60;
//    bg.text.color = LV_COLOR_GRAY;

//    lv_obj_t *roller2 = lv_roller_create(lv_scr_act(), NULL);
//    lv_obj_set_size(roller2, 80, 120);
//    lv_roller_set_options(roller2, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9");
//    lv_obj_align(roller2, roller1, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
//    lv_roller_set_anim_time(roller2, 500);
//    lv_roller_set_style(roller2, LV_ROLLER_STYLE_BG, &bg);
//    lv_roller_set_style(roller2, LV_ROLLER_STYLE_SEL, &lv_style_plain);
//    lv_roller_set_selected(roller2, 4, true);

//    lv_obj_t *roller3 = lv_roller_create(lv_scr_act(), roller2);
//    lv_obj_align(roller3, roller2, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
//    lv_roller_set_hor_fit(roller3, false);
//    lv_obj_set_width(roller3, LV_DPI);

//}

//void lcd_test(void) {
//    int c = 100;
//    for(int i = 0;i < 500;i++) {
//        tft_fill(0,0,100,300,c);
//        c+= 10;
//        xpt2046_loop();
//    }
//}

//const char logo[] = "\
///*\n\
// * This file is part of the \n\
// *\n\
// * Copyright (c) 2017-2018 linghaibin\n\
// *\n\
// */\n\
// Welcome to use device!\n";