/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "equipment.h"
#include "task_can.h"
#include "task_modbus.h"

//设置Holding
static int equipment_holeing_set(lua_State *L){
    int address = luaL_checkinteger(L, 1);
    int val = luaL_checkinteger(L, 2);
    task_modbus_set_holding(address,val);
    return 0;
}

static int equipment_holeing_get(lua_State *L){
    int address = luaL_checkinteger(L, 1);
    lua_pushinteger(L, task_modbus_get_holding(address));
    return 1;
}
//设置线圈Coils
static int equipment_coils_set(lua_State *L){
    int address = luaL_checkinteger(L, 1);
    int num = luaL_checkinteger(L, 2);
    int val = luaL_checkinteger(L, 3);
    device_send send_msg;
    send_msg.type = DO_8;
    send_msg.address = address;
    send_msg.cmd = 0x00;
    send_msg.data[0] = num;
    send_msg.data[1] = val;
    task_can_set(send_msg);
    vTaskDelay(1 / portTICK_RATE_MS); /* 操作限制 不允许高速 */
    return 0;
}

static int equipment_coils_get(lua_State *L){
    int i = luaL_checkinteger(L, -1);
    return 0;
}
//读取Input
static int equipment_input_get(lua_State *L) {
    int address = luaL_checkinteger(L, 1);
    uint8_t val = 0;
    if(lua_isinteger(L, 2)) {
        int num = luaL_checkinteger(L, 2);
        val = task_modbus_get_input_bit(address,num);
    } else {
        val = task_modbus_get_input(address);
    }
    lua_pushinteger(L, val);
    return 1;
}
//延时
static int equipment_delay(lua_State *L) {
    int ms = luaL_checkinteger(L, -1);
    vTaskDelay(ms / portTICK_RATE_MS);
    return 0;
}

static luaL_Reg equipment[] = {
    //c接口函数都可以放在这里在lua中声明  
    {"equ_holeing_set", equipment_holeing_set},
    {"equ_holeing_get", equipment_holeing_get},
    {"equ_coils_set",   equipment_coils_set  },
    {"equ_coils_get",   equipment_coils_get  },
    {"equ_input_get",   equipment_input_get  },
    {"equ_delay",       equipment_delay      },
    {NULL, NULL}
};
 
LUAMOD_API int luaopen_equipment (lua_State *L) {
  luaL_newlib(L, equipment);
  return 1;
}
