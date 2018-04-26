/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "ltime.h"
#include "l_list.h"

static l_list_t * time_head; /* ����ͷ */

void ltime_init(void) {
    time_head = (l_list_t *)l_malloc(sizeof(l_list_t)); /* ����ͷ�ڵ� */
    list_init(time_head); //��ʼ������
}

uint16_t ltime_add(int l_id, uint32_t time_ms) {
    ltime * time;
    time = l_malloc(sizeof(ltime));
    time->l_id = l_id;
    time->time_ms = time_ms;
    time->id = list_len(time_head);
    list_append(time_head, time); //׷�ӽ��
    return time->id;
}

void ltime_remove(uint16_t time_id) {
    ltime * time;
    list_get(time_head, time_id, (void **)&time); 
    list_pop(time_head,time_id);
    l_free(time);
}

void ltime_loop(void) {
    ltime * time;
    for (int i = 0; i < list_len(time_head); i++){ 
        list_get(time_head, i, (void **)&time); 
    }
}

//static luaL_Reg r_ltime[] = {
//    //c�ӿں��������Է���������lua������  
//    {"add", ltime_add},
//    {"remove", ltime_remove},
//    {NULL, NULL}
//};
