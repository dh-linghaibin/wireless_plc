/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "gd32f10x.h"

#include "tm1650.h"
#include "sign_led.h"
#include "sign_configure.h"
#include "delay.h"
#include "sm16703p.h"
#include "wireless.h"
#include "si446x.h"

#include "can.h"
#include "multi_timer.h"
#include "multi_button.h" 
#include "timer.h"
#include "app_can.h"

struct Timer t_led;
struct Timer t_ai_led;
struct Timer t_key;

struct button b_key;

void timeout_cb(void);
void ai_timeout_cb(void);
void timeout_cb_key( void );

uint8_t pin_level(void);
void key_callback(void * p);

int main(void) {
    delay_init();
    tm1650_init();		/* 显示初始化 */
    sign_led();
    sign_configure();
    timer_init_l();
//    wireless_init();
//    si446x_init();
    bxcan_init(2);
    
    {
        sign_write(L_AI1,50);
        sign_write(L_AI2,30);
        sign_write(L_AI3,10);
        sign_write(L_AI4,0);
        sign_write(L_AO1,0);
        sign_write(L_AO2,0);
        
        tm1650_set_led(L_DI1,LV_CLOSE);
        tm1650_set_led(L_DI2,LV_CLOSE);
        tm1650_set_led(L_DI3,LV_CLOSE);
        tm1650_set_led(L_DI4,LV_CLOSE);
        tm1650_set_led(L_DO1,LV_CLOSE);
    }
    
    mtimer_init(&t_led,timeout_cb,1000, 1000);
    mtimer_start(&t_led);
    
    mtimer_init(&t_ai_led,ai_timeout_cb,200, 200);
    mtimer_start(&t_ai_led);
    
    mtimer_init(&t_key,timeout_cb_key,5, 5);
    mtimer_start(&t_key);
    
    button_init(&b_key,pin_level,1);
    button_attach(&b_key,PRESS_DOWN,key_callback);
    button_start(&b_key);
    while (1) {
        mtimer_loop();
    }
}

void timeout_cb(void) {
    static led_val vr = LV_CLOSE;
    tm1650_set_led(L_RUN,vr);
    tm1650_set_led(L_ERROR,vr);
    vr = vr == LV_OPEN ? LV_CLOSE : LV_OPEN;
    //mtimer_stop(&t_led);
}

void ai_timeout_cb(void) {
    sign_write( L_AI1 , (sign_read(S_AI1)/10)*10 ); /* /10 * 10 是为了去掉精度 */
    sign_write( L_AI2 , (sign_read(S_AI2)/10)*10 );
    sign_write( L_AI3 , (sign_read(S_AI3)/10)*10 );
    sign_write( L_AI4 , (sign_read(S_AI4)/10)*10 );
}

uint8_t but_key;

uint8_t pin_level(void) {
    if(but_key == 0x4f)  return 1; else return 0;
}

void timeout_cb_key( void ) {
    but_key = tm1650_read_key();
    button_ticks();
}

void key_callback(void * p) {
    uint32_t btn_event_val; 
    
    btn_event_val = get_button_event((struct button *)p); 
    
    static uint8_t add = 0;
    
    switch(btn_event_val)
    {
        case PRESS_DOWN:
            add++;
            tm1650_set_nex(0,add/100);/* 显示设备地址 */
            tm1650_set_nex(1,add%100/10);/* 显示设备地址 */
            tm1650_set_nex(2,add%10);
            
            can_trasnmit_message_struct send_msg;
            send_msg.tx_sfid = bxcan_get_id();
            send_msg.tx_dlen = 3;
            send_msg.tx_data[0] = DI_4; /* 设备类型 */
            send_msg.tx_data[1] = 0xf0; /* 命令 */
            send_msg.tx_data[2] = 11; /* 继电器状态 */
            bxcan_send(send_msg);
        
        break; 
    }
}


