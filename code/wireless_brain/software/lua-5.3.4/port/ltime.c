/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "ltime.h"
#include "l_list.h"

static l_list_t * time_head; /* 链表头 */
static uint16_t l_tic = 0;

void ltime_init(void) {
    time_head = (l_list_t *)l_malloc(sizeof(l_list_t)); /* 创建头节点 */
    list_init(time_head); //初始化链表
}

uint16_t ltime_add(int l_id, uint32_t time_ms) {
    ltime * time;
    time = l_malloc(sizeof(ltime));
    time->l_id = l_id;
    time->time_ms = time_ms;
    if( (l_tic+time->time_ms) > 0xffff ) {
        time->tic_ms = 0xffff-l_tic+time->time_ms;
    } else {
        time->tic_ms = l_tic+time->time_ms;
    }
    time->id = list_len(time_head);
    list_append(time_head, time); //追加结点
    return time->id;
}

void ltime_remove(uint16_t time_id) {
    ltime * time;
    list_get(time_head, time_id, (void **)&time); 
    list_pop(time_head,time_id);
    l_free(time);
}

void ltime_loop(lua_State * L) {
    ltime * time;
    if(l_tic < 0xffff) {
        l_tic++;
    } else {
        l_tic = 0;
    }
    for (int i = 0; i < list_len(time_head); i++){ 
        list_get(time_head, i, (void **)&time);
        if(time->tic_ms == l_tic) { /* 时间到了 */
            if( (l_tic+time->time_ms) > 0xffff ) {
                time->tic_ms = 0xffff-l_tic+time->time_ms;
            } else {
                time->tic_ms = l_tic+time->time_ms;
            }
            /*do something*/
            lua_rawgeti(L, LUA_REGISTRYINDEX, time->l_id); //压栈
//            lua_pushstring(L, pack);
//            lua_pushnumber(L, on_create);
            lua_pcall(L, 0, 0, 0);//访问栈
        }
    }
}

static int ltime_l_add(lua_State *L) {
    int time_ms = luaL_checkinteger(L, 1);
    if(lua_isfunction(L, 2)) {
        int func_id = 0;
        lua_pushvalue(L, 2); 
        func_id = luaL_ref(L, LUA_REGISTRYINDEX);
        int id = ltime_add(func_id,time_ms);
        lua_pushinteger(L, id);
        return 1;
    } else {
        return 0;
    }
}

static int ltime_l_remove(lua_State *L) {
    int id = luaL_checkinteger(L, 1);
    ltime_remove(id);
    return 0;
}

static luaL_Reg r_ltime[] = {
    //c接口函数都可以放在这里在lua中声明  
    {"add", ltime_l_add},
    {"remove", ltime_l_remove},
    {NULL, NULL}
};

LUAMOD_API int luaopen_ltime (lua_State *L) {
  luaL_newlib(L, r_ltime);
  return 1;
}
