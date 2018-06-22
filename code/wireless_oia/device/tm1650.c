/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "tm1650.h"
#include "multi_button.h" 

static const uint8_t show_code[14] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D,0x07, 0x7F, 0x6F, 0x76, 0x40,0x79, 0x00}; 
static uint8_t show_led_code = 0;

#define DEFAULT_SETTING 0x48
#define NORMAL_DISPLAY    0x01
// From left to right order
#define DISPLAY_POINT    0x80
#define FIRST_POSITON    0x68
#define SECOND_POSITION 0x6A
#define THIRD_POSITION  0x6C
#define FOURTH_POSITON  0x6E

static void iic_send(uint8_t cmd, uint8_t data) {
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, cmd, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while(!i2c_flag_get(I2C0, I2C_FLAG_TBE));
    /* data transmission */
    i2c_data_transmit(I2C0, data);
    /* wait until the TBE bit is set */
    int time_out = 0;
    while( (!i2c_flag_get(I2C0, I2C_FLAG_TBE)) && (time_out < 60000) ) {
        time_out++;
    }
    i2c_stop_on_bus(I2C0);
    while(I2C_CTL0(I2C0)&0x0200);
}

static struct button btn;

static uint8_t button_read_pin(void) 
{
    return 1;//rt_pin_read(BUTTON_PIN); 
}

void tm1650_button_init(BtnCallback cb) {
     /* low level drive */
    button_init  (&btn, button_read_pin, 0);
    button_attach(&btn, PRESS_DOWN,       cb);
    button_attach(&btn, PRESS_UP,         cb);
    button_attach(&btn, PRESS_REPEAT,     cb);
    button_attach(&btn, SINGLE_CLICK,     cb);
    button_attach(&btn, DOUBLE_CLICK,     cb);
    button_attach(&btn, LONG_RRESS_START, cb);
    button_attach(&btn, LONG_PRESS_HOLD,  cb);
    button_start (&btn);
}

void tm1650_init(void) {
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable I2C0 clock */
    rcu_periph_clock_enable(RCU_I2C0);
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_7);

     /* I2C clock configure */
    i2c_clock_config(I2C0, 400000U, I2C_DTCY_2);
    /* I2C address configure */
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x82);
    /* enable I2C0 */
    i2c_enable(I2C0);
    /* enable acknowledge */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);    

    iic_send(DEFAULT_SETTING, NORMAL_DISPLAY); /* ÉèÖÃÁÁ¶È */
}

void tm1650_set_nex(uint8_t num,uint8_t cmd) {
    switch(num) {
        case 0: {
            iic_send(FIRST_POSITON,show_code[cmd]);    
        } break;
        case 1: {
            iic_send(SECOND_POSITION,show_code[cmd]);    
        } break;
        case 2: {
            iic_send(THIRD_POSITION,show_code[cmd]);    
        } break;
        default: {

        } break;
    }
}

void tm1650_set_led(led_code code, led_val val) {
    if(val == 0) {
        show_led_code &= ~(1 << code);
    } else {
        show_led_code |= (1 << code);
    }
    iic_send(FOURTH_POSITON,show_led_code);    
}

uint8_t tm1650_read_key(void) {
    uint8_t reKey;
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C0, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(I2C0);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(I2C0, 0x49, I2C_RECEIVER);
      /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
    
    /* While there is data to be read */
    /* Disable Acknowledgement */
    i2c_ack_config(I2C0, I2C_ACK_DISABLE);    

    /* Send STOP Condition */
    i2c_stop_on_bus(I2C0);
    /* wait until the RBNE bit is set */
    while(!i2c_flag_get(I2C0, I2C_FLAG_RBNE));
    /* Read a byte from the MPU6050 */
    reKey = i2c_data_receive(I2C0);
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
    return(reKey);
}



