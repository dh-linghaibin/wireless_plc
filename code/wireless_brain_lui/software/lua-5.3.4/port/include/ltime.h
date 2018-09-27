/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */
 
#ifndef _LTIME_H_
#define _LTIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"
//lua
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "string.h"

typedef struct _ltime {
    int      l_id;        /* lua�ص�ID */
    uint16_t id;          /* time ID */
    uint16_t time_ms;/* ��ʱ������ */
    uint16_t tic_ms;  /* �ط�Χ��־ */
} ltime;

void ltime_init(void);
uint16_t ltime_add(int l_id, uint32_t time_ms);
void ltime_loop(lua_State * L);

#ifdef __cplusplus
}
#endif

#endif

