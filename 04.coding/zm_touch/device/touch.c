/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
#include "touch.h"
#include "delay.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"


#define SDA_0 gpio_bit_write(GPIOB,GPIO_PIN_7,RESET)
#define SDA_1 gpio_bit_write(GPIOB,GPIO_PIN_7,SET)
#define SCL_0 gpio_bit_write(GPIOB,GPIO_PIN_6,RESET)
#define SCL_1 gpio_bit_write(GPIOB,GPIO_PIN_6,SET)

#define SDA_IN  gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,GPIO_PIN_7)
#define SDA_OUT gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7)
#define SCL_IN  gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,GPIO_PIN_6)
#define SCL_OUT gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6)

#define I2C_SDA_READ gpio_input_bit_get(GPIOB,GPIO_PIN_7)

static void iic0_conguration(void) {
    rcu_periph_clock_enable(RCU_GPIOB);
    
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
    
    SDA_0;
    SCL_0;
    delay_us(50);
    SCL_1;
    SDA_1;
    delay_us(50);
}

void soft_iic_send_noack(void){
    SDA_OUT;
    SDA_1;
    vTaskDelay(1 / portTICK_RATE_MS);
    SCL_1;
    vTaskDelay(1 / portTICK_RATE_MS);
    SCL_0;
}

void soft_iic_send_ack(void) {
    SDA_OUT;
    SDA_0;
    vTaskDelay(1 / portTICK_RATE_MS);
    SCL_1;
    vTaskDelay(1 / portTICK_RATE_MS);
    SCL_0;
}

void soft_iic_stop(void){
    SDA_OUT;
    SCL_0;
    vTaskDelay(1 / portTICK_RATE_MS);
    SCL_1;
    vTaskDelay(1 / portTICK_RATE_MS);
    SDA_1;
    SDA_IN;
    SCL_IN;
}

void soft_iic_start(void){
    SDA_OUT;
    SCL_OUT;
    SDA_1;
    SCL_1;
    vTaskDelay(1 / portTICK_RATE_MS);
    SDA_0;
    vTaskDelay(1 / portTICK_RATE_MS);
    SCL_0;
}

void iic_write(uint8_t datIn) {
    uint8_t dat, j;
    dat = datIn;
    SDA_OUT;
    SCL_0;
    for (j = 0; j < 8; j++) {
        if((dat & 0x80)) SDA_1;
        else SDA_0;
        vTaskDelay(1 / portTICK_RATE_MS);
        SCL_1;
        vTaskDelay(1 / portTICK_RATE_MS);
        dat <<= 1;
        SCL_0;
        vTaskDelay(1 / portTICK_RATE_MS);
    }
}

uint8_t check_ack(void) {
    uint8_t ack_flag;
    
    SDA_IN;
    vTaskDelay(1 / portTICK_RATE_MS);
    SCL_1;
    vTaskDelay(1 / portTICK_RATE_MS);
    
    if(I2C_SDA_READ == 1){
        ack_flag = 0;
    }else{
        ack_flag = 1;
    }
    SCL_0;
    return ack_flag;
}

uint8_t iic_read(void) {
    uint8_t j, dat = 0;
    SDA_IN;
    vTaskDelay(1 / portTICK_RATE_MS);
    for (j = 0; j < 8; j++) {
        SCL_1;
        dat <<= 1;
        vTaskDelay(1 / portTICK_RATE_MS);
        if(I2C_SDA_READ == 1){
            dat |= 0x01;
        }
        SCL_0;
        vTaskDelay(1 / portTICK_RATE_MS);
    }
    return dat;
}

uint16_t iic0_read(void) {
    uint16_t val;
    soft_iic_start();
    iic_write(0x81);
    if (check_ack()==0) {
        
    }
    val = iic_read();
    val |= iic_read() << 8;
    soft_iic_send_noack();
    soft_iic_stop();
    return val;
}

static uint16_t touch_val = 0;
static uint8_t touch_mode = 1;

void touch_init(void) { 
    iic0_conguration();
    touch_mode = 0;
    touch_val = 0;
}

uint16_t touch_get_val(void) {
    return touch_val;
}

void touch_get_val_set(uint16_t val) {
    touch_val = val;
}

static uint16_t flag[9] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};

touch_key_e touch_read(void) {
    uint16_t val = iic0_read();
    if( (val&0x8000) == 0x8000) {
        flag[0] = 0;
    } else {
        if(flag[0] == 0) {
            flag[0] = 1;
            if(touch_mode == 0) {
                touch_val = 0x0001;
            } else if(touch_mode == 1) {
                touch_val = 1;
            }
            return KEY1;
        }
    }
    
    if( (val&0x4000) == 0x4000) {
        flag[1] = 0;
    } else {
        if(flag[1] == 0) {
            flag[1] = 1;
            if(touch_mode == 0) {
                touch_val = 0x0002;
            } else if(touch_mode == 1) {
                touch_val = 2;
            }
            return KEY2;
        }
    }
    
    if( (val&0x2000) == 0x2000) {
        flag[2] = 0;
    } else {
        if(flag[2] == 0) {
            flag[2] = 1;
            if(touch_mode == 0) {
                touch_val = 0x0004;
            } else if(touch_mode == 1) {
                touch_val = 3;
            }
            return KEY3;
        }
    }
    
    if( (val&0x0001) == 0x0001) {
        flag[3] = 0;
    } else {
        if(flag[3] == 0) {
            flag[3] = 1;
            if(touch_mode == 0) {
                touch_val = 0x0008;
            } else if(touch_mode == 1) {
                touch_val = 4;
            }
            return KEY4;
        }
    }
    
    if( (val&0x0002) == 0x0002) {
        flag[4] = 0;
    } else {
        if(flag[4] == 0) {
            flag[4] = 1;
            if(touch_mode == 0) {
                touch_val = 0x0010;
            } else if(touch_mode == 1) {
                touch_val = 5;
            }
            return KEY5;
        }
    }
    
    if( (val&0x1000) == 0x1000) {
        flag[5] = 0;
    } else {
        if(flag[5] == 0) {
            flag[5] = 1;
            if(touch_mode == 0) {
                touch_val = 0x0020;
            } else if(touch_mode == 1) {
                touch_val = 6;
            }
            return KEY6;
        }
    }
    
    if( (val&0x0004) == 0x0004) {
        flag[6] = 0;
    } else {
        if(flag[6] == 0) {
            flag[6] = 1;
            if(touch_mode == 0) {
                touch_val = 0x0040;
            } else if(touch_mode == 1) {
                touch_val = 7;
            }
            return KEY7;
        }
    }
    
    if( (val&0x0008) == 0x0008) {
        flag[7] = 0;
    } else {
        if(flag[7] == 0) {
            flag[7] = 1;
            if(touch_mode == 0) {
                touch_val = 0x0080;
            } else if(touch_mode == 1) {
                touch_val = 8;
            }
            return KEY8;
        }
    }
    
    if( (val&0x0010) == 0x0010) {
        flag[8] = 0;
    } else {
        if(flag[8] == 0) {
            flag[8] = 1;
            if(touch_mode == 0) {
                touch_val = 0x0100;
            } else if(touch_mode == 1) {
                touch_val = 9;
            }
            return KEY9;
        }
    }
    
    return KEY_NULL;
}

/***************************************************************END OF FILE****/

 