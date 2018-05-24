/* 引用头文件 *****************************************************************/
#include "ringbuf.h"
/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
/* 私有动作宏 *****************************************************************/
/* 私有变量 *******************************************************************/
/* 全局变量 */
/* 私有函数声明 ***************************************************************/
/* 私有函数  ******************************************************************/
int RingbufInit(Stdringbuf_n* r, uint8_t array[], uint8_t len) {
    if (len < 2 || array==NULL){
        return false;
    }
    r->buf = array;
    r->length = len;
    r->fill_cnt = 0;
    r->head = r->tail = 0;
    return true;
}

int RingbufPut(Stdringbuf_n* r, uint8_t data) {
    //当tail+1等于head时，说明队列已满
    if (r->fill_cnt >= r->length) {
        //printf("BUF FULL!\n");
        return false;                  // 如果缓冲区满了，则返回错误
    }
    r->buf[r->tail] = data;
    r->tail++;
    r->fill_cnt++;
    //计算tail是否超出数组范围，如果超出则自动切换到0
    r->tail = r->tail % r->length;
    return true;
}

int RingbufGut(Stdringbuf_n* r, uint8_t *c, uint16_t length) {
    //当tail等于head时，说明队列空
    if (r->fill_cnt<=0) {
        //printf("BUF EMPTY!\n");
        return false;
    }
    //最多只能读取r->length长度数据
    if (length > r->length) {
        length = r->length;
    }
    
    if(r->tail > r->head) {
        if(r->tail < (length + r->head)) {
            return false;
        }
    } else {
        if( ((r->length-r->head)+r->tail) < length ) {
            return false;
        }
    }
    
    int i;
    for (i = 0; i<length; i++)
    {
        r->fill_cnt--;
        *c = r->buf[r->head++];                 // 返回数据给*c
        *c++;
        //计算head自加后的下标是否超出数组范围，如果超出则自动切换到0
        r->head = r->head % r->length;
    }
    return true;
}


/***************************************************************END OF FILE****/
