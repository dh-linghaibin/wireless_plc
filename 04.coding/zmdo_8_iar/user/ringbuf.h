#ifndef __RINGBUF_H
#define __RINGBUF_H
/* ����ͷ�ļ� *****************************************************************/
#include "stm32f0xx.h"

#define false   0       
#define true    1       
#define NULL    0
/* �ⲿ�������� ***************************************************************/
typedef struct {
    uint8_t  *buf;             //ָ����������ָ��
    uint16_t length;    //����
    uint16_t head;      //��ͷ
    uint16_t tail;      //��β
    int fill_cnt;           //���м���
}Stdringbuf_n;

typedef struct {
    Stdringbuf_n ringbuf_n;
    int (*init)(Stdringbuf_n* r, uint8_t array[], uint8_t len);
    int (*put)(Stdringbuf_n* r, uint8_t data);
    int (*get)(Stdringbuf_n* r, uint8_t *c, uint16_t length);
}Stdringbuf;
/* �ⲿ������ *****************************************************************/
/* �ⲿ������ *****************************************************************/
/* �ⲿ���� *******************************************************************/
/* �������� *******************************************************************/
int RingbufInit(Stdringbuf_n* r, uint8_t array[], uint8_t len);
int RingbufPut(Stdringbuf_n* r, uint8_t data);
int RingbufGut(Stdringbuf_n* r, uint8_t *c, uint16_t length);

#endif
/***************************************************************END OF FILE****/
