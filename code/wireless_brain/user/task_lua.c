/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "task_lua.h"
//lua
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "string.h"

static lua_State* L;

void task_lua_init(void) {
    L = luaL_newstate();
    luaL_openlibs(L);
    //Lua_task();
}

void Lua_task(void) {
    if (luaL_dofile(L,"1:test.lua")!=0)//����sd���е�Lua.lua������  
          printf(lua_tostring(L,-1));
}
