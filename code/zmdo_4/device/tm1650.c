/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#include "tm1650.h"
#include "delay.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#define DEFAULT_SETTING  0x48
#define NORMAL_DISPLAY   0x01
// From left to right order
#define DISPLAY_POINT    0x80
#define FIRST_POSITON    0x68
#define SECOND_POSITION  0x6A
#define THIRD_POSITION   0x6C
#define FOURTH_POSITON   0x6E

static const uint8_t show_code[15] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D,0x07, 0x7F, 0x6F, 0x76, 0x40,0x79, 0x7c,0x00};
static uint8_t show_merry[4];

static void tm1650_send(uint8_t cmd, uint8_t data) {
    #if USB_IIC
    taskENTER_CRITICAL();//进入〇接
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, cmd, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));
    /* data transmission */
    i2c_data_transmit(I2C1, data);
    /* wait until the TBE bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));
    i2c_stop_on_bus(I2C1);
    while(I2C_CTL0(I2C1)&0x0200);
    taskEXIT_CRITICAL();
    #endif
}

uint8_t tm1650_read_key(void) {
    #if USB_IIC
    uint8_t reKey;
    taskENTER_CRITICAL();//进入〇接
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C1);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C1, 0x49, I2C_RECEIVER);
      /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C1, I2C_FLAG_ADDSEND);
    
    /* While there is data to be read */
    /* Disable Acknowledgement */
    i2c_ack_config(I2C1, I2C_ACK_DISABLE);    

    /* Send STOP Condition */
    i2c_stop_on_bus(I2C1);
    /* wait until the RBNE bit is set */
    while(!i2c_flag_get(I2C1, I2C_FLAG_RBNE));
    /* Read a byte from the MPU6050 */
    reKey = i2c_data_receive(I2C1);
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);
    taskEXIT_CRITICAL();
    return(reKey);
    #endif
}

void tm1650_set_nex(uint8_t num,uint8_t cmd) {
    switch(num) {
        case 0:
        /*第一位*/
        tm1650_send(FIRST_POSITON,show_code[cmd]);
        break;
        case 1:
        /*第二位*/
        tm1650_send(SECOND_POSITION, show_code[cmd]);
        break;
    }
}


void tm1650_set_led(tm1650_led num,uint8_t var) {
        switch(num) {
            case L_COIL1:
            if(var == 0) {
                show_merry[2] &= 0xef;
            } else {
                show_merry[2] |= 0x10;
            }
            break;
            case L_COIL2:
            if(var == 0) {
                show_merry[2] &= 0xdf;
            } else {
                show_merry[2] |= 0x20;
            }
            break;
            case L_COIL3:
            if(var == 0) {
                show_merry[2] &= 0xbf;
            } else {
                show_merry[2] |= 0x40;
            }
            break;
            case L_COIL4:
            if(var == 0) {
                show_merry[2] &= 0xf7;
            } else {
                show_merry[2] |= 0x08;
            }
            break;
            case L_COIL5:
//            if(var == 0) {
//                show_merry[3] &= 0xfe;
//            } else {
//                show_merry[3] |= 0x01;
//            }
            break;
            case L_COIL6:
//            if(var == 0) {
//                show_merry[3] &= 0xfd;
//            } else {
//                show_merry[3] |= 0x02;
//            }
            break;
            case L_COIL7:
//            if(var == 0) {
//                show_merry[3] &= 0xbf;
//            } else {
//                show_merry[3] |= 0x40;
//            }
            break;
            case L_COIL8:
//            if(var == 0) {
//                show_merry[3] &= 0x7f;
//            } else {
//                show_merry[3] |= 0x80;
//            }
            break;
            case L_COIL_ALL:
            if(var == 0) {
                show_merry[2] &= 0x03;
            } else {
                show_merry[2] |= 0xfc;
            }
            /*第三位*/
            //tm1650_send(THIRD_POSITION, show_merry[2]);
            break;
            case L_RUN://运行指示灯
            if(var == 0) {
                show_merry[2] &= 0xfd;
            } else {
                show_merry[2] |= 0x02;
            }
            break;
            case L_RADIO://通讯指示灯
            if(var == 0) {
                show_merry[2] &= 0xfe;
            } else {
                show_merry[2] |= 0x01;
            }
            break;
        }
        /*第三位*/
        tm1650_send(THIRD_POSITION, show_merry[2]);
}

void tm1650_init(void) { 
    rcu_periph_clock_enable(RCU_GPIOB);

    /* enable I2C1 clock */
    rcu_periph_clock_enable(RCU_I2C1);
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_10);
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_11);

     /* I2C clock configure */
    i2c_clock_config(I2C1, 200000U, I2C_DTCY_2);
    /* I2C address configure */
    i2c_mode_addr_config(I2C1, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x82);
    /* enable I2C1 */
    i2c_enable(I2C1);
    /* enable acknowledge */
    i2c_ack_config(I2C1, I2C_ACK_ENABLE);    

    delay_ms(30);
    //tm1650_send(DEFAULT_SETTING, NORMAL_DISPLAY);
    tm1650_send(0x48, 0x21);
    tm1650_set_nex(0,0);
    tm1650_set_nex(1,0);
    tm1650_set_led((tm1650_led)8,0x00);
}
/***************************************************************END OF FILE****/
