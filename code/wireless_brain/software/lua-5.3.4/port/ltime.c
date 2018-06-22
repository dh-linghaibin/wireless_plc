/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "ltime.h"
#include "l_list.h"
#include "ui_debug.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

static l_list_t * time_head; 
static uint16_t l_tic = 0;

void ltime_init(void) {
    time_head = (l_list_t *)l_malloc(sizeof(l_list_t)); 
    list_init(time_head); 
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
    list_append(time_head, time);
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
        l_tic+=10;
    } else {
        l_tic = 0;
    }
    for (int i = 0; i < list_len(time_head); i++){ 
        list_get(time_head, i, (void **)&time);
        if(time->tic_ms == l_tic) {
            if( (l_tic+time->time_ms) > 0xffff ) {
                time->tic_ms = 0xffff-l_tic+time->time_ms;
            } else {
                time->tic_ms = l_tic+time->time_ms;
            }
            /*do something*/
            printf("id %d\n",time->l_id);
            lua_rawgeti(L, LUA_REGISTRYINDEX, time->l_id);
            lua_pcall(L, 0, 0, 0);
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

static RTC_TimeTypeDef RTC_TimeStruct;
static RTC_DateTypeDef RTC_DateStruct;

static int ltime_time(lua_State *L) {
    RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
    
    lua_newtable(L);//创建一个表格，放在栈顶  
    lua_pushstring(L, "year"); 
    lua_pushinteger(L,RTC_DateStruct.RTC_Year);
    lua_settable(L,-3);  
    lua_pushstring(L, "month"); 
    lua_pushinteger(L,RTC_DateStruct.RTC_Month);
    lua_settable(L,-3);  
    lua_pushstring(L, "date"); 
    lua_pushinteger(L,RTC_DateStruct.RTC_Date);
    lua_settable(L,-3);  
    lua_pushstring(L, "hour"); 
    lua_pushinteger(L,RTC_TimeStruct.RTC_Hours);
    lua_settable(L,-3);  
    lua_pushstring(L, "min"); 
    lua_pushinteger(L,RTC_TimeStruct.RTC_Minutes);
    lua_settable(L,-3);  
    lua_pushstring(L, "sec"); 
    lua_pushinteger(L,RTC_TimeStruct.RTC_Seconds);
    lua_settable(L,-3);  
    return 1;
}

static luaL_Reg r_ltime[] = {
    {"add", ltime_l_add},
    {"remove", ltime_l_remove},
    {"time", ltime_time},
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


//int func_return_table(lua_State *L)  
//{  
// lua_newtable(L);//创建一个表格，放在栈顶  
// lua_pushstring(L, "mydata");//压入key  
// lua_pushnumber(L,66);//压入value  
// lua_settable(L,-3);//弹出key,value，并设置到table里面去  
// lua_pushstring(L, "subdata");//压入key  
// lua_newtable(L);//压入value,也是一个table  
// lua_pushstring(L, "mydata");//压入subtable的key  
// lua_pushnumber(L,53);//value  
// lua_settable(L,-3);//弹出key,value,并设置到subtable  
// lua_settable(L,-3);//这时候父table的位置还是-3,弹出key,value(subtable),并设置到table里去  
// lua_pushstring(L, "mydata2");//同上  
// lua_pushnumber(L,77);  
// lua_settable(L,-3);  
// return 1;//堆栈里现在就一个table.其他都被弹掉了。  
//}  
//返回的表结构是:
//{
// "mydata" = 66,
// "mydate2" = 77,
// "subdata" = 
// {
//  "mydata" = 53
// }
//}