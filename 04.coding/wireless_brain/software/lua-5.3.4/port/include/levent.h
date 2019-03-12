/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */
 
#ifndef _LEVENT_H_
#define _LEVENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"
//lua
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "string.h"

#include "task_can.h"

typedef struct _levent {
    uint16_t id;
    int l_id;
    device_type type; /* �������� */
    uint16_t address; /* �豸��ַ */
    uint8_t  num;     /* �豸λ */
    uint16_t  val;     /* ������ */
    uint8_t  flag;    /* ״̬��ע */
} levent;

void levent_init(void);
void levent_loop(lua_State * L);

#ifdef __cplusplus
}
#endif

#endif

