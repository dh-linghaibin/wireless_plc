/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "equipment.h"
#include "task_can.h"
#include "task_modbus.h"


static int equipment_holeing_set(lua_State *L){
    int address = luaL_checkinteger(L, 1);
    int val = luaL_checkinteger(L, 2);
    vTaskDelay(15 / portTICK_RATE_MS); 
    task_modbus_set_holding(address,val);
    return 0;
}

static int equipment_holeing_get(lua_State *L){
    int address = luaL_checkinteger(L, 1);
    vTaskDelay(15 / portTICK_RATE_MS); 
    lua_pushinteger(L, task_modbus_get_holding(address));
    return 1;
}

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
    vTaskDelay(15 / portTICK_RATE_MS); 
    return 0;
}

static int equipment_coils_get(lua_State *L){
    vTaskDelay(15 / portTICK_RATE_MS); 
    int i = luaL_checkinteger(L, -1);
    return 0;
}

static int equipment_input_get(lua_State *L) {
    int address = luaL_checkinteger(L, 1);
    uint8_t val = 0;
    vTaskDelay(15 / portTICK_RATE_MS); 
    if(lua_isinteger(L, 2)) {
        int num = luaL_checkinteger(L, 2);
        val = task_modbus_get_input_bit(address,num);
    } else {
        val = task_modbus_get_input(address);
    }
    lua_pushinteger(L, val);
    return 1;
}

static int equipment_modbus_get(lua_State *L) {
    int address = luaL_checkinteger(L, 1); //设备地址
    int modbus_address = luaL_checkinteger(L, 2); //从机地址
    int num = luaL_checkinteger(L, 3); //寄存器地址
    int val = luaL_checkinteger(L, 4); //类型
    int hold_num = luaL_checkinteger(L, 5); //读出来的值存放位置
    tack_can_modbus_read_to_hold_set(hold_num);
    device_send send_msg;
    send_msg.type = CAN_485;
    send_msg.address = address;
    send_msg.cmd = 0x00;
    send_msg.data[0] = 0x10;
    send_msg.data[1] = modbus_address;
    send_msg.data[2] = num;
    send_msg.data[3] = val;
    task_can_set(send_msg);
    vTaskDelay(50 / portTICK_RATE_MS); 
    return 1;
}

static int equipment_modbus_set(lua_State *L) {
    int address = luaL_checkinteger(L, 1); //设备地址
    int modbus_address = luaL_checkinteger(L, 2); //从机地址
    int num = luaL_checkinteger(L, 3); //寄存器地址
    int val = luaL_checkinteger(L, 4); //类型
    int hold_num = luaL_checkinteger(L, 5); //读出来的值存放位置
    device_send send_msg;
    send_msg.type = CAN_485;
    send_msg.address = address;
    send_msg.cmd = 0x00;
    send_msg.data[0] = 0x11;
    send_msg.data[1] = modbus_address;
    send_msg.data[2] = num;
    send_msg.data[3] = val;
    send_msg.data[4] = hold_num;
    send_msg.data[5] = hold_num >> 8;
    task_can_set(send_msg);
    vTaskDelay(50 / portTICK_RATE_MS); 
    return 1;
}

static int equipment_delay(lua_State *L) {
    int ms = luaL_checkinteger(L, -1);
    vTaskDelay(ms / portTICK_RATE_MS);
    return 0;
}

static luaL_Reg equipment[] = {
    {"holeing_set", equipment_holeing_set},
    {"holeing_get", equipment_holeing_get},
    {"coils_set",   equipment_coils_set  },
    {"coils_get",   equipment_coils_get  },
    {"input_get",   equipment_input_get  },
    {"modbus_get",  equipment_modbus_get },
    {"modbus_set",  equipment_modbus_set },
    {"delay",       equipment_delay      },
    {NULL, NULL}
};
 
LUAMOD_API int luaopen_equipment (lua_State *L) {
  luaL_newlib(L, equipment);
  return 1;
}
