/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _TSAK_LUA_H_
#define _TSAK_LUA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"
#include "stdbool.h"

void task_lua_init(void);
void task_lua_create(void);
void task_lua_set(bool cmd);

#ifdef __cplusplus
}
#endif

#endif


