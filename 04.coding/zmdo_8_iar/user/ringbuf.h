#ifndef __RINGBUF_H
#define __RINGBUF_H
/* 引用头文件 *****************************************************************/
#include "stm32f0xx.h"

#define false   0       
#define true    1       
#define NULL    0
/* 外部数据类型 ***************************************************************/
typedef struct {
    uint8_t  *buf;             //指向队列数组的指针
    uint16_t length;    //长度
    uint16_t head;      //队头
    uint16_t tail;      //队尾
    int fill_cnt;           //队列计数
}Stdringbuf_n;

typedef struct {
    Stdringbuf_n ringbuf_n;
    int (*init)(Stdringbuf_n* r, uint8_t array[], uint8_t len);
    int (*put)(Stdringbuf_n* r, uint8_t data);
    int (*get)(Stdringbuf_n* r, uint8_t *c, uint16_t length);
}Stdringbuf;
/* 外部常数宏 *****************************************************************/
/* 外部动作宏 *****************************************************************/
/* 外部变量 *******************************************************************/
/* 函数声明 *******************************************************************/
int RingbufInit(Stdringbuf_n* r, uint8_t array[], uint8_t len);
int RingbufPut(Stdringbuf_n* r, uint8_t data);
int RingbufGut(Stdringbuf_n* r, uint8_t *c, uint16_t length);

#endif
/***************************************************************END OF FILE****/
