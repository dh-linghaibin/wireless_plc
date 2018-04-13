/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "task_set.h"
#include "digital_tube.h"
#include "can.h"
#include "wdog.h"
#include "flash.h"
#include "button.h"
#include "time.h"

typedef struct {
    uint8_t device_id;
    uint8_t device_br;
    uint8_t flag;
    uint8_t show_time;
    uint8_t page; /* 当前菜单页 */
    uint8_t line; /* 当前行 */
    uint8_t show_num[2]; /* 当前显示内容 */
} menu_v;

static menu_v menu_t;

static TimerHandle_t xtime_show; /* 定义句柄 */
static TimerHandle_t xtime_tic; /* 定义句柄 */
static menu_v menu_t;

static void task_set(void *pvParameters);

void task_set_init(void) {
    menu_t.show_time = 0;
    menu_t.page = 0;
    menu_t.flag = 0;
    menu_t.device_br = 2;
    menu_t.device_id = 5;
    
    digital_tube_init();
    button_init();
    bxcan_init();
    
    digital_tube_set_num(0,1);
    digital_tube_set_num(1,2);
    
    uint32_t addr = 0;/* 读取地址 */
//    flash_read(C_FLAG,&addr);
//    if(addr != 0x5555) {/* 判断是否第一次上电 */
//        addr = 0x5555; 
//        flash_write(C_FLAG,addr);
//        addr = 1;
//        flash_write(C_ADDR,addr);
//        addr = 0;
//        flash_write(C_DEVICE_VAL,addr);
//        addr = 2;
//        flash_write(C_BR,addr);
//    }
//    flash_read(C_ADDR,&addr);
//    if(addr > 100) {/* 保护地址合法范围 */
//        addr = 1;
//    }
//    menu_t.device_id = addr;
//    digital_tube_set_num(0,menu_t.device_id/10);
//    digital_tube_set_num(1,menu_t.device_id%10);
//    bxcan_set_id(menu_t.device_id);
//    flash_read(C_BR,&addr);
//    if(addr > 7) {/* 保护地址合法范围 */
//        addr = 2;
//    }
//    menu_t.device_br = addr;
//    bxcan_init(menu_t.device_br);
}

void task_set_create(void) {
    xTaskCreate( task_set,"task_set", 400, NULL, tskIDLE_PRIORITY+3, NULL );
}

void task_set_br(uint8_t br) {
//    if(br > 7) {
//        br = 2;
//    }
//    menu_t.page = M_PAGE1_SHOW;
//    //xTimerStop(xtime_show,0);/*关闭定时器*/
//    vTaskDelay(50 / portTICK_RATE_MS);
//    menu_t.device_br = br;
//    bxcan_set_br(menu_t.device_br);
//    taskENTER_CRITICAL();//进入〇接
//    flash_write(C_BR,menu_t.device_br);
//    taskEXIT_CRITICAL();
//    digital_tube_set_num(0,13);
//    digital_tube_set_num(1,menu_t.device_br);
}

void task_set_address(uint8_t address) {
//    if(address > 99) {
//        address = 2;
//    }
//    //xTimerStop(xtime_show,0);/*关闭定时器*/
//    menu_t.page = M_PAGE1_SHOW;
//    vTaskDelay(50 / portTICK_RATE_MS);
//    menu_t.device_id = address;
//    bxcan_set_id(menu_t.device_id);
//    taskENTER_CRITICAL();//进入〇接
//    flash_write(C_ADDR,menu_t.device_id);
//    taskEXIT_CRITICAL();
//    digital_tube_set_num(0,menu_t.device_id/10);
//    digital_tube_set_num(1,menu_t.device_id%10);
}

static void vtimer_show( TimerHandle_t xtimer ) {
    configASSERT(xtimer);
    digital_tube_refresh();
}

static void vtimer_tic( TimerHandle_t xtimer ) {
    configASSERT(xtimer);
    static uint8_t dr = 0;
    static uint8_t hz = 0;
    
    static uint8_t run_dr = 1;
    static uint8_t run_hz = 0;
    if(run_hz < 10) {
        run_hz ++;
    } else {
        run_hz = 0;
        run_dr = run_dr==0?1:0;
        igital_tube_toggle_point(0,run_dr);
       // wdog_reload();/* 喂狗 */
    }
    switch(menu_t.page) {
        case M_PAGE1_SHOW: {
            if(menu_t.show_time < 200) {
                menu_t.show_time++;
            } else if(menu_t.show_time == 200) {
                menu_t.show_time = 201;
                digital_tube_set_num(0,11);
                digital_tube_set_num(1,11);
                vTaskDelay(50 / portTICK_RATE_MS);
            }
        } break;
        case M_PAGE2_SET_MENU: {
            if(hz < 5) {
                hz++;
            } else {
                hz = 0;
                if(dr == 0) {
                    dr = 1;
                    digital_tube_set_num(0,menu_t.show_num[0]);
                    digital_tube_set_num(1,menu_t.show_num[1]);
                } else {
                    dr = 0;
                    digital_tube_set_num(0,11);
                    digital_tube_set_num(1,11);
                }
            }
        } break;
        case M_PAGE2_SET_ADDR: {
            if(menu_t.flag == 1) { /* 自动加 */
                if(menu_t.line < 99) {
                    menu_t.line++;
                } else {
                    menu_t.line = 0;
                }
                menu_t.show_num[0] = menu_t.line/10;
                menu_t.show_num[1] = menu_t.line%10;
                digital_tube_set_num(0,menu_t.show_num[0]);
                digital_tube_set_num(1,menu_t.show_num[1]);
            } else {
                if(hz < 5) {
                    hz++;
                } else {
                    hz = 0;
                    if(dr == 0) {
                        dr = 1;
                        digital_tube_set_num(0,menu_t.show_num[0]);
                        digital_tube_set_num(1,menu_t.show_num[1]);
                    } else {
                        dr = 0;
                        digital_tube_set_num(0,11);
                        digital_tube_set_num(1,11);
                    }
                }
            }
        } break;
        case M_PAGE3_SET_BR: {
            if(hz < 5) {
                hz++;
            } else {
                hz = 0;
                if(dr == 0) {
                    dr = 1;
                    digital_tube_set_num(0,menu_t.show_num[0]);
                    digital_tube_set_num(1,menu_t.show_num[1]);
                } else {
                    dr = 0;
                    digital_tube_set_num(0,11);
                    digital_tube_set_num(1,11);
                }
            }
        } break;
    }
    if(menu_t.page != M_PAGE1_SHOW) { /* 超时退出 */
        if(menu_t.show_time < 200) {
            menu_t.show_time++;
        } else if(menu_t.show_time == 200) {
            menu_t.page = M_PAGE1_SHOW;
            menu_t.show_time = 0;
            digital_tube_set_num(0,menu_t.device_id/10);
            digital_tube_set_num(1,menu_t.device_id%10);
            menu_t.line = M_LENE_ADDR;
            vTaskDelay(50 / portTICK_RATE_MS);
        }
    }
}

static void task_set(void *pvParameters) {
    uint8_t flag = 0;
    uint8_t long_down  = 0;
    xtime_show = xTimerCreate("set_show",         /* 定时器名字 */
                              8,             /* 定时器周期,单位时钟节拍 */
                              pdTRUE,           /* 周期性 */
                              (void *) 0,       /* 定时器ID */
                              vtimer_show); /* 定时器回调函数 */
    if(xtime_show == NULL) {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    } else {
        /* 启动定时器，系统启动后才开始工作  0 表示不阻塞*/
        if(xTimerStart(xtime_show, 0) != pdPASS) {
            /* 定时器还没有进入激活状态 */
        }
    }
    
    xtime_tic = xTimerCreate("set_tic",         /* 定时器名字 */
                          50,             /* 定时器周期,单位时钟节拍 */
                          pdTRUE,           /* 周期性 */
                          (void *) 0,       /* 定时器ID */
                          vtimer_tic); /* 定时器回调函数 */
    if(xtime_tic == NULL) {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    } else {
        /* 启动定时器，系统启动后才开始工作  0 表示不阻塞*/
        if(xTimerStart(xtime_tic, 0) != pdPASS) {
            /* 定时器还没有进入激活状态 */
        }
    }
    
    for(;;) {
        vTaskDelay(20 / portTICK_RATE_MS);
        uint8_t key = button_read();
        switch(key) {
            case B_ADD: {
                flag = key;
                if(long_down < 100) {
                    long_down++;
                } else {
                    if(long_down == 100) {
                        long_down = 200;
                        menu_t.show_time = 0; /*开显示*/
                        switch(menu_t.page) {
                            case M_PAGE1_SHOW: {
                                menu_t.show_num[0] = 12; /* H1 */
                                menu_t.show_num[1] = 0;
                                menu_t.line = M_LENE_ADDR;
                                menu_t.page = M_PAGE2_SET_MENU;
                            } break;
                            case M_PAGE2_SET_MENU: {
                                switch(menu_t.line) {
                                    case M_LENE_ADDR: {
                                        menu_t.flag = 0;
                                        menu_t.line = menu_t.device_id;
                                        menu_t.show_num[0] = menu_t.line/10;
                                        menu_t.show_num[1] = menu_t.line%10;
                                        menu_t.page = M_PAGE2_SET_ADDR;
                                    } break;
                                    case M_LINE_BR: {
                                        menu_t.flag = 0;
                                        menu_t.line = menu_t.device_br;
                                        menu_t.show_num[0] = 10;
                                        menu_t.show_num[1] = menu_t.line;
                                        menu_t.page = M_PAGE3_SET_BR;
                                    } break;
                                }
                            } break;
                            case M_PAGE2_SET_ADDR: { /* 长按 加 */
                                menu_t.flag = 1;
                            } break;
                            case M_PAGE3_SET_BR: {
                                
                            } break;
                        }
                    }
                }
            } break;
            case B_SUB: {
                flag = key;
                if(long_down < 100) {
                    long_down++;
                } else {
                    if(long_down == 100) {
                        long_down = 200;
                        menu_t.show_time = 0; /*开显示*/
                        switch(menu_t.page) {
                            case M_PAGE1_SHOW: {
                                
                            } break;
                            case M_PAGE2_SET_MENU: {
                                
                            } break;
                            case M_PAGE2_SET_ADDR: { /* 长按保存地址 */
                                //xTimerStop(xtime_show,0);/*关闭定时器*/
                                menu_t.page = M_PAGE1_SHOW;
                                vTaskDelay(50 / portTICK_RATE_MS);
                                menu_t.device_id = menu_t.line;
                                bxcan_set_id(menu_t.device_id);
                                taskENTER_CRITICAL();//进入〇接
                                flash_write(C_ADDR,menu_t.device_id);
                                taskEXIT_CRITICAL();
                                digital_tube_set_num(0,menu_t.device_id/10);
                                digital_tube_set_num(1,menu_t.device_id%10);
                            } break;
                            case M_PAGE3_SET_BR: {
                                menu_t.page = M_PAGE1_SHOW;
                                //xTimerStop(xtime_show,0);/*关闭定时器*/
                                vTaskDelay(50 / portTICK_RATE_MS);
                                menu_t.device_br = menu_t.line;
                                bxcan_set_br(menu_t.device_br);
                                taskENTER_CRITICAL();//进入〇接
                                flash_write(C_BR,menu_t.device_br);
                                taskEXIT_CRITICAL();
                                digital_tube_set_num(0,10);
                                digital_tube_set_num(1,menu_t.device_br);
                            } break;
                        }
                    }
                }
            } break;
            default: {
                switch( flag ) {
                    case B_ADD: {
                        if( (long_down > 1)  && (long_down < 180) ){
                            menu_t.show_time = 0; /*开显示*/
                            switch(menu_t.page) {
                                case M_PAGE1_SHOW: {
                                    switch(menu_t.line) {
                                        case M_LENE_ADDR: {
                                            digital_tube_set_num(0,10);
                                            digital_tube_set_num(1,menu_t.device_br%10);
                                            menu_t.line = M_LINE_BR;
                                        } break;
                                        case M_LINE_BR: {
                                            digital_tube_set_num(0,menu_t.device_id/10);
                                            digital_tube_set_num(1,menu_t.device_id%10);
                                            
                                            menu_t.line = M_LENE_ADDR;
                                        } break;
                                        default : {
                                            digital_tube_set_num(0,menu_t.device_id/10);
                                            digital_tube_set_num(1,menu_t.device_id%10);
                                            menu_t.line = M_LENE_ADDR;
                                        } break;
                                    }
                                    vTaskDelay(50 / portTICK_RATE_MS);
                                } break;
                                case M_PAGE2_SET_MENU: {
                                    switch(menu_t.line) {
                                        case M_LENE_ADDR: {
                                            menu_t.show_num[0] = 12; /* H1 */
                                            menu_t.show_num[1] = 1;
                                            menu_t.line = M_LINE_BR;
                                        } break;
                                        case M_LINE_BR: {
                                            menu_t.show_num[0] = 12; /* H1 */
                                            menu_t.show_num[1] = 0;
                                            menu_t.line = M_LENE_ADDR;
                                        } break;
                                    }
                                } break;
                                case M_PAGE2_SET_ADDR: {
                                    if(menu_t.line < 99) {
                                        menu_t.line++;
                                    } else {
                                        menu_t.line = 0;
                                    }
                                    menu_t.show_num[0] = menu_t.line/10;
                                    menu_t.show_num[1] = menu_t.line%10;
                                } break;
                                case M_PAGE3_SET_BR: {
                                    if(menu_t.line < 8) {
                                        menu_t.line++;
                                    } else {
                                        menu_t.line = 0;
                                    }
                                    menu_t.show_num[1] = menu_t.line;
                                } break;
                            }
                        } else {
                            switch(menu_t.page) {
                                case M_PAGE1_SHOW: {
                                    
                                } break;
                                case M_PAGE2_SET_MENU: {
                                
                                } break;
                                case M_PAGE2_SET_ADDR: {
                                    menu_t.flag = 0;
                                } break;
                                case M_PAGE3_SET_BR: {
                                
                                } break;
                            }
                            long_down = 0;
                        }
                    } break;
                    case B_SUB: {
                        if( (long_down > 1)  && (long_down < 180) ){
                            menu_t.show_time = 0; /*开显示*/
                            switch(menu_t.page) {
                                case M_PAGE1_SHOW: {
                                    switch(menu_t.line) {
                                        case M_LENE_ADDR: {
                                            digital_tube_set_num(0,10);
                                            digital_tube_set_num(1,menu_t.device_br%10);
                                            menu_t.line = M_LINE_BR;
                                        } break;
                                        case M_LINE_BR: {
                                            digital_tube_set_num(0,menu_t.device_id/10);
                                            digital_tube_set_num(1,menu_t.device_id%10);
                                            menu_t.line = M_LENE_ADDR;
                                        } break;
                                        default : {
                                            digital_tube_set_num(0,menu_t.device_id/10);
                                            digital_tube_set_num(1,menu_t.device_id%10);
                                            menu_t.line = M_LENE_ADDR;
                                        } break;
                                    }
                                    vTaskDelay(50 / portTICK_RATE_MS);
                                } break;
                                case M_PAGE2_SET_MENU: {
                                    switch(menu_t.line) {
                                        case M_LENE_ADDR: {
                                            menu_t.show_num[0] = 12; /* H1 */
                                            menu_t.show_num[1] = 1;
                                            menu_t.line = M_LINE_BR;
                                        } break;
                                        case M_LINE_BR: {
                                            menu_t.show_num[0] = 12; /* H1 */
                                            menu_t.show_num[1] = 0;
                                            menu_t.line = M_LENE_ADDR;
                                        } break;
                                    }
                                } break;
                                case M_PAGE2_SET_ADDR: {
                                    if(menu_t.line > 0) {
                                        menu_t.line--;
                                    } else {
                                        menu_t.line = 99;
                                    }
                                    menu_t.show_num[0] = menu_t.line/10;
                                    menu_t.show_num[1] = menu_t.line%10;
                                } break;
                                case M_PAGE3_SET_BR: {
                                    if(menu_t.line > 0) {
                                        menu_t.line--;
                                    } else {
                                        menu_t.line = 7;
                                    }
                                    menu_t.show_num[1] = menu_t.line;
                                } break;
                            }
                        } else {
                            switch(menu_t.page) {
                                case M_PAGE1_SHOW: {
                                    
                                } break;
                                case M_PAGE2_SET_MENU: {
                                    
                                } break;
                                case M_PAGE2_SET_ADDR: {
                                    
                                } break;
                                case M_PAGE3_SET_BR: {
                                
                                } break;
                            }
                            long_down = 0;
                        }
                    } break;
                }
                long_down = 0;
                flag = 0x00;
            } break;
        }
    }
}
