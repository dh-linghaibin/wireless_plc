/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "ltime.h"
#include "l_list.h"

static l_list_t * time_head; /* ����ͷ */
static uint16_t l_tic = 0;

void ltime_init(void) {
    time_head = (l_list_t *)l_malloc(sizeof(l_list_t)); /* ����ͷ�ڵ� */
    list_init(time_head); //��ʼ������
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
    list_append(time_head, time); //׷�ӽ��
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
        if(time->tic_ms == l_tic) { /* ʱ�䵽�� */
            if( (l_tic+time->time_ms) > 0xffff ) {
                time->tic_ms = 0xffff-l_tic+time->time_ms;
            } else {
                time->tic_ms = l_tic+time->time_ms;
            }
            /*do something*/
            lua_rawgeti(L, LUA_REGISTRYINDEX, time->l_id); //ѹջ
//            lua_pushstring(L, pack);
//            lua_pushnumber(L, on_create);
            lua_pcall(L, 0, 0, 0);//����ջ
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
    //c�ӿں��������Է���������lua������  
    {"add", ltime_l_add},
    {"remove", ltime_l_remove},
    {NULL, NULL}
};

LUAMOD_API int luaopen_ltime (lua_State *L) {
  luaL_newlib(L, r_ltime);
  return 1;
}

  
//int static add(lua_State* L)  
//{  
//    //获取第一个参数  
//    double x=lua_tonumber(L,1);  
//    double y=lua_tonumber(L,2);  
//    //返回值压栈，压入两个返回值  
//    lua_pushnumber(L,x+y);  
//    lua_pushnumber(L,1000);  
//  
//    //返回值的个数，  
//    return 2;  
//}  

