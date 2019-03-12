/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#include "lcd.h"
#include "delay.h"
#include "tlc7226.h"
#include "ec11.h"
#include "stdio.h"
#include "simulation.h"
#include "button.h"
#include "sram.h"
#include "multi_button.h" 
#include "read_ad.h"
#include "ina219.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "can.h"


typedef enum {
    PT100 = 0,
    PT1000 = 1,
    VOLTAGE = 2,
    ELE_CURRENT = 3,
    AM_NULL = 4
} ads_read_mode;

uint8_t bd_state = 0;

static struct button btn;

static uint8_t button_read_pin(void) 
{
    return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4); 
}

void button_callback(void *btn) {
    uint32_t btn_event_val; 
    
    btn_event_val = get_button_event((struct button *)btn); 
    
    switch(btn_event_val) {
    case PRESS_DOWN: {
        static uint8_t dr = 0;
        dr = (0 == dr) ? 1 : 0;
        simulation_set_24v(dr);
        //simulation_set_ad(dr);
    }
    break; 

    case PRESS_UP: 
        
    break; 

    case PRESS_REPEAT: 
        
    break; 

    case SINGLE_CLICK: 
        
    break; 

    case DOUBLE_CLICK: 
        
    break; 

    case LONG_RRESS_START: 
        
    break; 

    case LONG_PRESS_HOLD: 
        
    break; 
    }
}



static struct button btn_ec;

static uint8_t button_ec_read_pin(void) 
{
    return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0); 
}

void button_ec_callback(void *btn) {
    uint32_t btn_event_val; 
    btn_event_val = get_button_event((struct button *)btn); 
    switch(btn_event_val) {
    case PRESS_DOWN: {
        if( 0 == bd_state ) {
            lcd_clolor_box(0,0,320,240,Black);
            bd_state = 1;
        }
    }
    break; 
    }
}


void multi_button_test(void)
{
    button_init  (&btn, button_read_pin, 0);
    button_attach(&btn, PRESS_DOWN,       button_callback);
//    button_attach(&btn, PRESS_UP,         button_callback);
//    button_attach(&btn, PRESS_REPEAT,     button_callback);
//    button_attach(&btn, SINGLE_CLICK,     button_callback);
//    button_attach(&btn, DOUBLE_CLICK,     button_callback);
//    button_attach(&btn, LONG_RRESS_START, button_callback);
//    button_attach(&btn, LONG_PRESS_HOLD,  button_callback);
    button_start (&btn);
    
    button_init  (&btn_ec, button_ec_read_pin, 0);
    button_attach(&btn_ec, PRESS_DOWN,       button_ec_callback);
    button_start (&btn_ec);
}


void all_close(void) {
    simulation_set_enab(0);
    simulation_set_venab(0);
    simulation_set_ad(0);
    simulation_set_pt100(1);
    simulation_set_pt1000(1);
    simulation_set_ntc10k(1);
    tlc7226_set(1,0,0);
    tlc7226_set(1,1,0);
    tlc7226_set(1,2,0);
    tlc7226_set(1,3,0);
    
    tlc7226_set(2,0,0);
    tlc7226_set(2,1,0);
    tlc7226_set(2,2,0);
    tlc7226_set(2,3,0);
}
void bd_cur_10ma(void) {
    simulation_set_enab(0);
    simulation_set_venab(0);
    tlc7226_set(1,0,0);
    tlc7226_set(1,1,0);
    tlc7226_set(1,2,0);
    tlc7226_set(1,3,0);
    
    tlc7226_set(2,0,0);
    tlc7226_set(2,1,0);
    tlc7226_set(2,2,0);
    tlc7226_set(2,3,0);
    simulation_set_pt100(1);
    simulation_set_pt1000(0);
    simulation_set_ntc10k(0);
}

void bd_cur_1ma(void) {
    simulation_set_enab(0);
    simulation_set_venab(0);
    tlc7226_set(1,0,0);
    tlc7226_set(1,1,0);
    tlc7226_set(1,2,0);
    tlc7226_set(1,3,0);
    
    tlc7226_set(2,0,0);
    tlc7226_set(2,1,0);
    tlc7226_set(2,2,0);
    tlc7226_set(2,3,0);
    simulation_set_pt100(0);
    simulation_set_pt1000(1);
    simulation_set_ntc10k(0);
}

void bd_val_0v(void) {
    simulation_set_pt100(1);
    simulation_set_pt1000(0);
    simulation_set_ntc10k(0);
    tlc7226_set(3,0,250);
    delay_ms(10);
    tlc7226_set(3,1,250);
    delay_ms(10);
    tlc7226_set(3,2,250);
    delay_ms(10);
    tlc7226_set(3,3,250);
    simulation_set_enab(0);
    simulation_set_venab(0);
}

void bd_val(void) {
    simulation_set_pt100(0);
    simulation_set_pt1000(0);
    simulation_set_ntc10k(0);
    tlc7226_set(3,0,250);
    delay_ms(10);
    tlc7226_set(3,1,250);
    delay_ms(10);
    tlc7226_set(3,2,250);
    delay_ms(10);
    tlc7226_set(3,3,250);
    simulation_set_enab(1);
    simulation_set_venab(1);
}

void bd_cur_0ma(void) {
    simulation_set_pt100(1);
    simulation_set_pt1000(0);
    simulation_set_ntc10k(0);
    tlc7226_set(3,0,250);
    delay_ms(10);
    tlc7226_set(3,1,250);
    delay_ms(10);
    tlc7226_set(3,2,250);
    delay_ms(10);
    tlc7226_set(3,3,250);
    simulation_set_enab(0);
    simulation_set_venab(0);
}

void bd_cur_20ma(void) {
    simulation_set_pt100(0);
    simulation_set_pt1000(0);
    simulation_set_ntc10k(0);
    tlc7226_set(3,0,250);
    delay_ms(10);
    tlc7226_set(3,1,250);
    delay_ms(10);
    tlc7226_set(3,2,250);
    delay_ms(10);
    tlc7226_set(3,3,250);
    simulation_set_enab(1);
    simulation_set_venab(0);
}

static int v1ma_bc = 0;
static int v10ma_bc = 0;
static int vval_bc[16];
static int vcur_bc[16];
static int vpt100_bc = 0;
static int vpt1000_bc = 0;
static uint8_t val_count = 0;
static uint8_t val_bd_arrow = 0;

void get_data(uint8_t * cmd) {
    switch(cmd[0]) {
        case 0x00: { /* 设置通道模式 */
        
        } break;
        case 0x01: { /* 设置电流通道 */
          
        } break;
        case 0x02: { /* 获取值*/
            switch(cmd[1]) {
                case 0: { //1ma
                    
                } break;
                case 1: { // 10ma
                    
                } break;
                case 2: { // 电压
                    char  buf[20];
                    uint16_t val = cmd[2];
                    val |= cmd[3] << 8;
                    if( (val_bd_arrow%2) == 0 ) {
                        sprintf(buf,"val-0  %d",val);
                        vval_bc[val_bd_arrow] = 0 - val;
                    } else {
                        sprintf(buf,"val-10 %d",val);
                        vval_bc[val_bd_arrow] = val + vval_bc[val_bd_arrow-1];
                    }
                    lcd_string(F_7X8,5,60+(val_bd_arrow*10),buf,Red,Black);
                    
                    uint8_t s_d[4];
                    s_d[0] = vval_bc[val_bd_arrow];
                    s_d[1] = vval_bc[val_bd_arrow] >> 8;
                    s_d[2] = vval_bc[val_bd_arrow] >> 16;
                    s_d[3] = vval_bc[val_bd_arrow] >> 24;
                    CanTxMsg tx_msg;
                    tx_msg.StdId = 0x0155;
                    tx_msg.DLC = 7;
                    tx_msg.Data[0] = 3; 
                    tx_msg.Data[1] = 2; /* 电压 */
                    tx_msg.Data[2] = val_bd_arrow; /*第几组*/
                    tx_msg.Data[3] = s_d[0];
                    tx_msg.Data[4] = s_d[1];
                    tx_msg.Data[5] = s_d[2];
                    tx_msg.Data[6] = s_d[3];
                    can_send_msg(tx_msg);
                    delay_ms(10);
                    if(val_bd_arrow == 14) {
                        val_bd_arrow = 1;
                        bd_state++;
                    } else if(val_bd_arrow == 15) {
                        bd_state++;
                    } else {
                        val_bd_arrow += 2;
                        bd_state--;
                    }
                   
                } break;
                case 3: { // 电流
                    char  buf[20];
                    uint16_t val = cmd[2];
                    val |= cmd[3] << 8;
                    if( (val_bd_arrow%2) == 0 ) {
                        sprintf(buf,"cur-0  %d",val);
                        vcur_bc[val_bd_arrow] = 0 - val;
                    } else {
                        sprintf(buf,"cur-20 %d",val);
                        vcur_bc[val_bd_arrow] = val + vcur_bc[val_bd_arrow-1];
                    }
                    lcd_string(F_7X8,105,60+(val_bd_arrow*10),buf,Green,Black);
                    
                    uint8_t s_d[4];
                    s_d[0] = vcur_bc[val_bd_arrow];
                    s_d[1] = vcur_bc[val_bd_arrow] >> 8;
                    s_d[2] = vcur_bc[val_bd_arrow] >> 16;
                    s_d[3] = vcur_bc[val_bd_arrow] >> 24;
                    CanTxMsg tx_msg;
                    tx_msg.StdId = 0x0155;
                    tx_msg.DLC = 7;
                    tx_msg.Data[0] = 3; 
                    tx_msg.Data[1] = 3; /* 电流 */
                    tx_msg.Data[2] = val_bd_arrow; /*第几组*/
                    tx_msg.Data[3] = s_d[0];
                    tx_msg.Data[4] = s_d[1];
                    tx_msg.Data[5] = s_d[2];
                    tx_msg.Data[6] = s_d[3];
                    can_send_msg(tx_msg);
                    delay_ms(10);
//                    val_bd_arrow++;
//                    bd_state++;
                    if(val_bd_arrow == 14) {
                        val_bd_arrow = 1;
                        bd_state++;
                    } else if(val_bd_arrow == 15) {
                        bd_state++;
                    } else {
                        val_bd_arrow += 2;
                        bd_state--;
                    }
                } break;
                case 4: { //pt100 
                    char  buf[20];
                    uint16_t val = cmd[2];
                    val |= cmd[3] << 8;
                    sprintf(buf,"pt1000 %d",val);
                    lcd_string(F_7X8,5,40,buf,White,Black);
                    vpt100_bc = 0 - val;
                    
                    uint8_t s_d[4];
                    s_d[0] = vpt100_bc;
                    s_d[1] = vpt100_bc >> 8;
                    s_d[2] = vpt100_bc >> 16;
                    s_d[3] = vpt100_bc >> 24;
                    CanTxMsg tx_msg;
                    tx_msg.StdId = 0x0155;
                    tx_msg.DLC = 6;
                    tx_msg.Data[0] = 3; /* 设置PT100 */
                    tx_msg.Data[1] = 4; /* PT100 */
                    tx_msg.Data[2] = s_d[0];
                    tx_msg.Data[3] = s_d[1];
                    tx_msg.Data[4] = s_d[2];
                    tx_msg.Data[5] = s_d[3];
                    can_send_msg(tx_msg);
                    delay_ms(10);
                    bd_state++;
                } break;
                case 5: { //pt1000 
                    char  buf[20];
                    uint16_t val = cmd[2];
                    val |= cmd[3] << 8;
                    sprintf(buf,"pt100  %d",val);
                    lcd_string(F_7X8,5,50,buf,Yellow,Black);
                    vpt1000_bc = 0 - val;
                    
                    uint8_t s_d[4];
                    s_d[0] = vpt1000_bc;
                    s_d[1] = vpt1000_bc >> 8;
                    s_d[2] = vpt1000_bc >> 16;
                    s_d[3] = vpt1000_bc >> 24;
                    CanTxMsg tx_msg;
                    tx_msg.StdId = 0x0155;
                    tx_msg.DLC = 6;
                    tx_msg.Data[0] = 3; /* 设置PT1000 */
                    tx_msg.Data[1] = 5; /* PT1000 */
                    tx_msg.Data[2] = s_d[0];
                    tx_msg.Data[3] = s_d[1];
                    tx_msg.Data[4] = s_d[2];
                    tx_msg.Data[5] = s_d[3];
                    can_send_msg(tx_msg);
                    delay_ms(10);
                    bd_state++;
                } break;
            }
        } break;
    }
}

uint16_t ad_val = 0;//100
float dy = 0;

#define SysNoiseCoVar         (0.0023)
#define MeasNoiseCoVar        (56.7415926)           //系统噪声协方差，测量噪声协方差
//SysNoiseCoVar越大，精度越低，速度越快MeasNoiseCovar越大，精度越高，速度越慢
volatile u32 AdjustVar1=0;//调整值
volatile u32 AdjustVar2=0;
volatile u32 AdjustVar3=0;
uint16_t MeasMatrix[3];	//测量值矩阵
/*卡尔曼滤波需要设置4个值，其余均可自动调整*/
uint16_t KalmanFitterAD1(uint16_t MeasVar) {
    static double PreOptimalVar=1;                       //先前最优值
    static double CurMeasVar=0,CurEstimateVar=0;        //当前测量值，当前预测值
    static double CurOptimalVar=0;                      //当前最优值
    static double CurSysCoVar=0,PreSysCoVar=30;         //当前系统协方差，先前系统协方差
    static double KalmanGain=0;                         //卡尔曼增益
    CurMeasVar=MeasVar;                         //当前系统测量值
    //先前系统最优值和先前系统协方差需要设置非零初始值；；；；
    /*当前估计值=先前最优值+调整值*/
    CurEstimateVar=PreOptimalVar+AdjustVar1;
    AdjustVar1=0;                                //设定调整值的原因是加快调整速度
    /*当前系统协方差=先前系统协方差+系统噪声协方差*/
    CurSysCoVar=PreSysCoVar+SysNoiseCoVar;
    /*卡尔曼增益=当前系统协方差/(当前系统协方差+测量噪声协方差)*/
    KalmanGain=CurSysCoVar/(CurSysCoVar+MeasNoiseCoVar);
    /*当前系统最优值=当前系统估计值+卡尔曼增益*（测量值-当前系统估计值）*/
    CurOptimalVar=CurEstimateVar+KalmanGain*(CurMeasVar-CurEstimateVar);
    /*先前系统噪声协方差=（1-卡尔曼增益）x当前系统协方差*/
    PreSysCoVar =(1-KalmanGain)*CurSysCoVar;
    PreOptimalVar=CurOptimalVar;		//递归处理当前系统最优值
    /*返回数据*/
    return (uint16_t)CurOptimalVar;
}

unsigned int voltage_mv,Current_ma,Power_mw,R_mv;

float voltage_mv_float,
Current_ma_float,
Power_mw_float,
R_mv_float;

int main(void) {
    delay_init(168);
    simulation_init();
    tlc7226_init();
    read_ad_init();
    ec11_init();
    lbutton_init();
    multi_button_test();
    lcd_init();
    all_close();
    can_init(BAUD_RATE_250K,CAN_Mode_Normal); 
    ina219_init();
//    lcd_clolor_box(0,0,110,46,alpha_blend(Green,Black,40));
//    lcd_string(F_7X8B,5,5,"Val 24.000V",Black,alpha_blend(Green,Black,40));
//    lcd_string(F_7X8B,5,23,"Cul 0.0000A",Black,alpha_blend(Green,Black,40));
//    lcd_string(F_7X8B,200,20,"ing...  5%",White,Black);
    while(1) {
        static uint16_t count_jc = 0;
        if(count_jc < 6000) {
            count_jc++;
        } else {
            count_jc = 0;
            voltage_mv=ina219_get_voltage_mv();	//得到电压（mV）
            voltage_mv_float=(float)voltage_mv/1000;	//转换为浮点型电压（V）
            Current_ma=ina219_get_current_ma();	//得到电流（mA）
            Current_ma_float=(float)Current_ma;	////得到浮点型电流（V）

            Power_mw=ina219_get_power_mv();//得到功率（mW）
            Power_mw_float=(float)Power_mw;//得到浮点型功率（mW）
            
            char buf[20];
            sprintf(buf,"VAL:%05.4f",voltage_mv_float);
            lcd_string(F_7X8,210,10,buf,White,Black);
            
            sprintf(buf,"CUR:%d",Current_ma);
            if(Current_ma > 60) {
                lcd_string(F_7X8,210,20,buf,Red,Black);
            } else {
                lcd_string(F_7X8,210,20,buf,White,Black);
            }
            
            delay_ms(5);
            button_ticks();
        }
//        switch((uint8_t)ec11_get()) {
//            case E_RIGHT: {
//              
//            } break;
//            case E_LEFT: {
//              
//            } break;
//        }
        switch(bd_state) {
            case 0: { /* 还没开始标定 */

            } break;
            case 1: { /*10ma 校准*/
                CanTxMsg tx_msg;
                tx_msg.StdId = 0x0155;
                tx_msg.DLC = 3;
                tx_msg.Data[0] = 0;
                tx_msg.Data[1] = 8;
                tx_msg.Data[2] = PT100;
                can_send_msg(tx_msg);
                delay_ms(50);
                bd_cur_10ma();//100
                delay_ms(100);
                bd_state = 2;
            } break;
            case 2: {
                CanTxMsg tx_msg;
                tx_msg.StdId = 0x0155;
                tx_msg.DLC = 3;
                tx_msg.Data[0] = 1; /* 设置电流通道 */
                tx_msg.Data[1] = 0; /* 10ma */
                tx_msg.Data[2] = 4; /* 通道5 */
                can_send_msg(tx_msg);
                delay_ms(50);
                simulation_set_ad(1);
                bd_state = 3;
                for(uint8_t i = 0;i < 50;i++) {//150
                     KalmanFitterAD1(read_ad_get());
                     delay_ms(1);
                }
            } break;
            case 3: {
                ad_val = KalmanFitterAD1(read_ad_get());//filter();
                if(abs(ad_val - 1253) < 1) {
                    if(val_count > 5) {
                        bd_state = 4;
                        simulation_set_ad(0);
                        lcd_string(F_7X8B,100,0,"10ma OK",White,Black);
                    } else {
                        val_count++;
                    }
                } else {
                    val_count = 0;
                    if(ad_val > 1253) {
                        //减
                        v10ma_bc--;
                    } else {
                        //加
                        v10ma_bc++;
                    }
                    uint8_t s_d[4];
                    s_d[0] = v10ma_bc;
                    s_d[1] = v10ma_bc >> 8;
                    s_d[2] = v10ma_bc >> 16;
                    s_d[3] = v10ma_bc >> 24;
                    CanTxMsg tx_msg;
                    tx_msg.StdId = 0x0155;
                    tx_msg.DLC = 6;
                    tx_msg.Data[0] = 3; /* 设置电流通道 */
                    tx_msg.Data[1] = 1; /* 10ma */
                    tx_msg.Data[2] = s_d[0];
                    tx_msg.Data[3] = s_d[1];
                    tx_msg.Data[4] = s_d[2];
                    tx_msg.Data[5] = s_d[3];
                    can_send_msg(tx_msg);
                    delay_ms(5);
                    bd_state = 2;
                }
            } break;
            case 4: { /* 校准0度 */
                CanTxMsg tx_msg; /* 获取0设置度 */
                tx_msg.StdId = 0x0155;
                tx_msg.DLC = 2;
                tx_msg.Data[0] = 2;
                tx_msg.Data[1] = 4;
                can_send_msg(tx_msg);
                bd_state++;
            }
            case 5: {
            
            } break;
            case 6: { /* 标定1ma */
                CanTxMsg tx_msg;
                tx_msg.StdId = 0x0155;
                tx_msg.DLC = 3;
                tx_msg.Data[0] = 0;
                tx_msg.Data[1] = 8;
                tx_msg.Data[2] = PT1000;
                can_send_msg(tx_msg);
                delay_ms(50);
                bd_cur_1ma();//100
                delay_ms(100);
                bd_state++;
            } break;
            case 7: {
                CanTxMsg tx_msg;
                tx_msg.StdId = 0x0155;
                tx_msg.DLC = 3;
                tx_msg.Data[0] = 1; /* 设置电流通道 */
                tx_msg.Data[1] = 1; /* 1ma */
                tx_msg.Data[2] = 4; /* 通道5 */
                can_send_msg(tx_msg);
                delay_ms(50);
                simulation_set_ad(1);
                bd_state++;
                for(uint8_t i = 0;i < 50;i++) {//150
                     KalmanFitterAD1(read_ad_get());
                     delay_ms(2);
                }
            } break;
            case 8: {
                ad_val = KalmanFitterAD1(read_ad_get());//filter();
                if(abs(ad_val - 1253) < 1) {
                    if(val_count > 5) {
                        bd_state++;
                        simulation_set_ad(0);
                        lcd_string(F_7X8B,100,10,"1ma OK",White,Black);
                    } else {
                        val_count++;
                    }
                } else {
                    val_count = 0;
                    if(ad_val > 1253) {
                        //减
                        v1ma_bc--;
                    } else {
                        //加
                        v1ma_bc++;
                    }
                    uint8_t s_d[4];
                    s_d[0] = v1ma_bc;
                    s_d[1] = v1ma_bc >> 8;
                    s_d[2] = v1ma_bc >> 16;
                    s_d[3] = v1ma_bc >> 24;
                    CanTxMsg tx_msg;
                    tx_msg.StdId = 0x0155;
                    tx_msg.DLC = 6;
                    tx_msg.Data[0] = 3; /* 设置电流通道 */
                    tx_msg.Data[1] = 0; /* 10ma */
                    tx_msg.Data[2] = s_d[0];
                    tx_msg.Data[3] = s_d[1];
                    tx_msg.Data[4] = s_d[2];
                    tx_msg.Data[5] = s_d[3];
                    can_send_msg(tx_msg);
                    delay_ms(5);
                    bd_state--;
                }
            } break;
            case 9: { /* 标定0度 */
                CanTxMsg tx_msg; /* 获取0设置度 */
                tx_msg.StdId = 0x0155;
                tx_msg.DLC = 2;
                tx_msg.Data[0] = 2;
                tx_msg.Data[1] = 5; /* PT100 */
                can_send_msg(tx_msg);
                bd_state++;
            } break;
            case 10: {
                
            } break;
            case 11: { /* 标定电压 */
                if(val_bd_arrow == 0) {
                    all_close(); /* 先关闭全部 */
                }
                CanTxMsg tx_msg;
                tx_msg.StdId = 0x0155;
                tx_msg.DLC = 3;
                tx_msg.Data[0] = 0;
                tx_msg.Data[1] = 8;
                tx_msg.Data[2] = VOLTAGE;
                can_send_msg(tx_msg);
                delay_ms(15);
                bd_val_0v();
                delay_ms(15);
                tx_msg.StdId = 0x0155; /* 获取电压 */
                tx_msg.DLC = 3;
                tx_msg.Data[0] = 2;
                tx_msg.Data[1] = 2;
                tx_msg.Data[2] = val_bd_arrow/2; /* 通道0 */
                can_send_msg(tx_msg);
                bd_state++;
            } break;
            case 12: {
                /* 等待返回 电压 */
            } break;
            case 13: {
                bd_val();
                delay_ms(15);
                CanTxMsg tx_msg;
                tx_msg.StdId = 0x0155; /* 获取电压 */
                tx_msg.DLC = 3;
                tx_msg.Data[0] = 2;
                tx_msg.Data[1] = 2;
                tx_msg.Data[2] = val_bd_arrow/2; /* 通道0 */
                can_send_msg(tx_msg);
                bd_state++;
            } break;
            case 14: {
                /* 等待返回 电压 */
            } break;
            case 15: {
                val_bd_arrow = 0; /* 给电流标定使用 */
                all_close(); /* 先关闭全部 */
                bd_state++;
            } break;
            case 16: {
                if(val_bd_arrow == 0) {
                    all_close(); /* 先关闭全部 */
                }
                CanTxMsg tx_msg;
                tx_msg.StdId = 0x0155;
                tx_msg.DLC = 3;
                tx_msg.Data[0] = 0;
                tx_msg.Data[1] = 8;
                tx_msg.Data[2] = ELE_CURRENT;
                can_send_msg(tx_msg);
                delay_ms(15);
                bd_cur_0ma();
                delay_ms(15);
                tx_msg.StdId = 0x0155; /* 获取电流 */
                tx_msg.DLC = 3;
                tx_msg.Data[0] = 2;
                tx_msg.Data[1] = 3; /* 电流 */
                tx_msg.Data[2] = val_bd_arrow/2; /* 通道0 */
                can_send_msg(tx_msg);
                bd_state++;
            } break;
            case 17: {
                /* 等待返回 电流 */
            } break;
            case 18: {
                bd_cur_20ma();
                delay_ms(15);
                CanTxMsg tx_msg;
                tx_msg.StdId = 0x0155; /* 获取电流 */
                tx_msg.DLC = 3;
                tx_msg.Data[0] = 2;
                tx_msg.Data[1] = 3; /* 电流 */
                tx_msg.Data[2] = val_bd_arrow/2; /* 通道0 */
                can_send_msg(tx_msg);
                bd_state++;
            } break;
            case 19: {
                /* 等待返回 电压 */
            } break;
            case 20: {
                val_bd_arrow = 0; /* 给电流标定使用 */
                all_close(); /* 先关闭全部 */
                
                CanTxMsg tx_msg; /* 保存配置值 */
                tx_msg.StdId = 0x0155;
                tx_msg.DLC = 1;
                tx_msg.Data[0] = 4;
                can_send_msg(tx_msg);
                
                bd_state = 0;
            } break;
        }
    }
}

