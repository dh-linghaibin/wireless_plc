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
    device_type type; /* 监听类型 */
    uint16_t address; /* 设备地址 */
    uint8_t  num;     /* 设备位 */
    uint16_t  val;     /* 监听量 */
    uint8_t  flag;    /* 状态标注 */
} levent;

void levent_init(void);
void levent_loop(lua_State * L);

#ifdef __cplusplus
}
#endif

#endif

