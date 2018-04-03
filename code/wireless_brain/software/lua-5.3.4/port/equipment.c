/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "equipment.h"

//设置Holding
static int equipment_holeing_set(lua_State *L){
    int address = luaL_checkinteger(L, 1);
    int val = luaL_checkinteger(L, 2);
    printf("%d %d \n",address,val);
    return 0;
}

static int equipment_holeing_get(lua_State *L){
    int address = luaL_checkinteger(L, 1);
    lua_pushinteger(L, 555+address);
    return 1;
}
//设置线圈Coils
static int equipment_coils_set(lua_State *L){
    int i = luaL_checkinteger(L, -1);
    return 0;
}

static int equipment_coils_get(lua_State *L){
    int i = luaL_checkinteger(L, -1);
    return 0;
}
//读取Input
static int equipment_input_get(lua_State *L){
    int i = luaL_checkinteger(L, -1);
    return 0;
}

static luaL_Reg equipment[] = {
    //c接口函数都可以放在这里在lua中声明  
    {"equ_holeing_set", equipment_holeing_set},
    {"equ_holeing_get", equipment_holeing_get},
    {"equ_coils_set", equipment_coils_set},
    {"equ_coils_get", equipment_coils_get},
    {"equ_input_get", equipment_input_get},
    {NULL, NULL}
};
 
LUAMOD_API int luaopen_equipment (lua_State *L) {
  luaL_newlib(L, equipment);
  return 1;
}