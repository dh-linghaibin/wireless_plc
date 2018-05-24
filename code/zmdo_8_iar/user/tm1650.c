/* 引用头文件 *****************************************************************/
#include "tm1650.h"
/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
/* 私有动作宏 *****************************************************************/
#define DEFAULT_SETTING 0x48
#define NORMAL_DISPLAY	0x01
// From left to right order
#define DISPLAY_POINT	0x80
#define FIRST_POSITON	0x68
#define SECOND_POSITION 0x6A
#define THIRD_POSITION  0x6C
#define FOURTH_POSITON  0x6E
/* 私有变量 *******************************************************************/
/* 全局变量 */
/* 私有函数声明 ***************************************************************/
/* 私有函数  ******************************************************************/
static void Tm1650Clock(GPIO_TypeDef* port){
    if (port == GPIOA)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    else if(port == GPIOB)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    else if(port == GPIOC)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    else if(port == GPIOD)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
    else if(port == GPIOE)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
    else if(port == GPIOF)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
}

static void Tm1650DelayMs(uint16_t ms) {						
    uint16_t i;
    while(ms--) {
        for(i=0;i<1125;i++);//2M crystal cycle 1us, i = 140; just 1ms, when 16M, i = 1125
    }
}

static void TM1650Start(Stdtm1650_n * tm1650n)
{
    GPIO_WriteBit(tm1650n->scl.port,tm1650n->scl.pin,(BitAction)(1));
    GPIO_WriteBit(tm1650n->sda.port,tm1650n->sda.pin,(BitAction)(1));
    Tm1650DelayMs(1);
    GPIO_WriteBit(tm1650n->sda.port,tm1650n->sda.pin,(BitAction)(0));
    Tm1650DelayMs(1);
    GPIO_WriteBit(tm1650n->scl.port,tm1650n->scl.pin,(BitAction)(0));
    Tm1650DelayMs(1);
}

static void TM1650Stop(Stdtm1650_n * tm1650n)
{
    GPIO_WriteBit(tm1650n->scl.port,tm1650n->scl.pin,(BitAction)(0));
    GPIO_WriteBit(tm1650n->sda.port,tm1650n->sda.pin,(BitAction)(0));
    Tm1650DelayMs(1);
    GPIO_WriteBit(tm1650n->scl.port,tm1650n->scl.pin,(BitAction)(1));
    Tm1650DelayMs(1);
    GPIO_WriteBit(tm1650n->sda.port,tm1650n->sda.pin,(BitAction)(1));
    Tm1650DelayMs(1);
}
/*---------------------------------------------------------------*-
* TM1650_ACK()
* 
* Make sure the slave has received the data
-*---------------------------------------------------------------*/
static void TM1650ACK(Stdtm1650_n * tm1650n) {
    GPIO_WriteBit(tm1650n->sda.port,tm1650n->sda.pin,(BitAction)(0));
    GPIO_WriteBit(tm1650n->scl.port,tm1650n->scl.pin,(BitAction)(1));
    Tm1650DelayMs(1);
    GPIO_WriteBit(tm1650n->scl.port,tm1650n->scl.pin,(BitAction)(0));
    GPIO_WriteBit(tm1650n->sda.port,tm1650n->sda.pin,(BitAction)(1));
}
/*---------------------------------------------------------------*-
* TM1650_Write_Byte()
* 
* Send one byte data to slave
-*---------------------------------------------------------------*/
static void TM1650WriteByte(Stdtm1650_n * tm1650n,uint8_t data)
{
    uint8_t i;
    for (i = 0; i < 8; i++) {
        if ((data & 0x80) == 0)
            GPIO_WriteBit(tm1650n->sda.port,tm1650n->sda.pin,(BitAction)(0));
        else
            GPIO_WriteBit(tm1650n->sda.port,tm1650n->sda.pin,(BitAction)(1));
        data <<= 1;
        GPIO_WriteBit(tm1650n->scl.port,tm1650n->scl.pin,(BitAction)(0));
        Tm1650DelayMs(1);
        GPIO_WriteBit(tm1650n->scl.port,tm1650n->scl.pin,(BitAction)(1));
        Tm1650DelayMs(1);
        GPIO_WriteBit(tm1650n->scl.port,tm1650n->scl.pin,(BitAction)(0));
        Tm1650DelayMs(1);
    }
}

static void TM1650Send(Stdtm1650_n * tm1650n,uint8_t cmd, uint8_t data)
{
    TM1650Start(tm1650n);
    TM1650WriteByte(tm1650n,cmd);	
    TM1650ACK(tm1650n);
    TM1650WriteByte(tm1650n,data);
    TM1650ACK(tm1650n);
    TM1650Stop(tm1650n);
}

uint8_t TM1650ScanKey(Stdtm1650_n * tm1650n)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t reKey,i;
    TM1650Start(tm1650n);
    TM1650WriteByte(tm1650n,0x49);
    TM1650ACK(tm1650n);
    GPIO_WriteBit(tm1650n->sda.port,tm1650n->sda.pin,(BitAction)(1));
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = tm1650n->sda.pin;
    GPIO_Init(tm1650n->sda.port, &GPIO_InitStructure);
    
    for(i=0;i<8;i++)
    {
        GPIO_WriteBit(tm1650n->scl.port,tm1650n->scl.pin,(BitAction)(0));
        reKey=reKey>>1;
        Tm1650DelayMs(10);
        GPIO_WriteBit(tm1650n->scl.port,tm1650n->scl.pin,(BitAction)(1));
        if(GPIO_ReadInputDataBit(tm1650n->sda.port,tm1650n->sda.pin))
        {
            reKey=reKey|0x80;
        }
        else
        {
            reKey=reKey|0x00;
        }
        Tm1650DelayMs(10);
    }
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = tm1650n->sda.pin;
    GPIO_Init(tm1650n->sda.port, &GPIO_InitStructure);
    
    TM1650ACK(tm1650n);
    TM1650Stop(tm1650n);
    return(reKey);
}

void Tm1650ShowNex(Stdtm1650_n * tm1650n,uint8_t num,uint8_t cmd) {
    switch(num) {
        case 0:
        tm1650n->show_merry[0] = cmd;
        /*第一位*/
        TM1650Send(tm1650n,FIRST_POSITON,tm1650n->show_code[tm1650n->show_merry[0]]);    
        break;
        case 1:
        tm1650n->show_merry[1] = cmd;   
        /*第二位*/
        TM1650Send(tm1650n,SECOND_POSITION, tm1650n->show_code[tm1650n->show_merry[1]]);
        break;
    }
}

void Tm1650ShowLed(Stdtm1650_n * tm1650n,uint8_t num,uint8_t var) {
   
        switch(num) {
            case 0:
            if(var == 0) {
                tm1650n->show_merry[3] &= 0xef;
            } else {
                tm1650n->show_merry[3] |= 0x10;
            }
            break;
            case 1:
            if(var == 0) {
                tm1650n->show_merry[3] &= 0xf7;
            } else {
                tm1650n->show_merry[3] |= 0x08;
            }
            break;
            case 2:
            if(var == 0) {
                tm1650n->show_merry[3] &= 0xfb;
            } else {
                tm1650n->show_merry[3] |= 0x04;
            }
            break;
            case 3:
            if(var == 0) {
                tm1650n->show_merry[3] &= 0xdf;
            } else {
                tm1650n->show_merry[3] |= 0x20;
            }
            break;
            case 4:
            if(var == 0) {
                tm1650n->show_merry[3] &= 0xfe;
            } else {
                tm1650n->show_merry[3] |= 0x01;
            }
            break;
            case 5:
            if(var == 0) {
                tm1650n->show_merry[3] &= 0xfd;
            } else {
                tm1650n->show_merry[3] |= 0x02;
            }
            break;
            case 6:
            if(var == 0) {
                tm1650n->show_merry[3] &= 0xbf;
            } else {
                tm1650n->show_merry[3] |= 0x40;
            }
            break;
            case 7:
            if(var == 0) {
                tm1650n->show_merry[3] &= 0x7f;
            } else {
                tm1650n->show_merry[3] |= 0x80;
            }
            break;
            case 8:
            if(var == 0) {
                tm1650n->show_merry[3] &= 0x00;
                tm1650n->show_merry[2] &= 0xfb;
            } else {
                tm1650n->show_merry[3] |= 0xff;
                tm1650n->show_merry[2] |= 0x04;
            }
            /*第三位*/
            TM1650Send(tm1650n,THIRD_POSITION, tm1650n->show_merry[2]);
            break;
            case 9://运行指示灯
            if(var == 0) {
                tm1650n->show_merry[2] &= 0xfd;
            } else {
                tm1650n->show_merry[2] |= 0x02;
            }
            break;
            case 10://通讯指示灯
            if(var == 0) {
                tm1650n->show_merry[2] &= 0xfe;
            } else {
                tm1650n->show_merry[2] |= 0x01;
            }
            break;
        }
        if(num <= 8) {
            /*第四位*/
            TM1650Send(tm1650n,FOURTH_POSITON, tm1650n->show_merry[3]);
        } else {
            /*第三位*/
            TM1650Send(tm1650n,THIRD_POSITION, tm1650n->show_merry[2]);
        }
}

void Tm1650Init(Stdtm1650_n * tm1650n) {
    GPIO_InitTypeDef GPIO_InitStructure;
    
    Tm1650Clock(tm1650n->scl.port);
    Tm1650Clock(tm1650n->sda.port);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin = tm1650n->scl.pin;
    GPIO_Init(tm1650n->scl.port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = tm1650n->sda.pin;
    GPIO_Init(tm1650n->sda.port, &GPIO_InitStructure);
    Tm1650DelayMs(30);
    TM1650Send(tm1650n,DEFAULT_SETTING, NORMAL_DISPLAY);
    Tm1650ShowNex(tm1650n,0,0);
}
/***************************************************************END OF FILE****/
