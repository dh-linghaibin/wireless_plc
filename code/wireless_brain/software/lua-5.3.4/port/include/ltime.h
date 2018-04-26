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
    int l_id; /* lua回调ID */
    uint16_t id; /* time ID */
    uint32_t  time_ms;/* 定时器周期 */
} ltime;



#ifdef __cplusplus
}
#endif

#endif

