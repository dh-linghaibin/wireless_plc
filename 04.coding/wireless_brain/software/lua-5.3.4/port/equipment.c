/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "equipment.h"
#include "task_can.h"
#include "task_modbus.h"
#include "rs485.h"

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
    uint8_t val = 0;
    int address = luaL_checkinteger(L, 1);
    vTaskDelay(15 / portTICK_RATE_MS); 
    if(lua_isinteger(L, 2)) {
        int num = luaL_checkinteger(L, 2);
        val = task_modbus_get_coil_bit(address,num);
    } else {
        val = task_modbus_get_coil_bit(address,8);
    }
    lua_pushinteger(L, val);
    return 1;
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
    int lx = luaL_checkinteger(L, 5);//连续读
    int hold_num = luaL_checkinteger(L, 6); //读出来的值存放位置
    tack_can_modbus_read_to_hold_set(hold_num);
    device_send send_msg;
    send_msg.type = CAN_485;
    send_msg.address = address;
    send_msg.cmd = 0x00;
    send_msg.data[0] = 0x10;
    send_msg.data[1] = modbus_address;
    send_msg.data[2] = num;
    send_msg.data[3] = val;
    send_msg.data[4] = lx;
    send_msg.data[5] = hold_num;
    send_msg.data[6] = (hold_num >> 8);
    task_can_set(send_msg);
    vTaskDelay(10 / portTICK_RATE_MS); 
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

static int equipment_re485_write(lua_State *L) {
    size_t count;
    const char * buf = luaL_checklstring(L, -1,&count);
    uint8_t * sbuf;
    sbuf = l_malloc(sizeof(uint8_t)*(count/2));
    for(int i = 0; i < count/2; i++) {
        uint8_t num1 = 0;
        uint8_t num2 = 0;
        if(buf[i*2] >= 'a') {
            num1 = buf[i*2] - 87;
        } else {
            num1 = buf[i*2]-'0';
        }
        if(buf[i*2+1] >= 'a') {
            num2 = buf[i*2+1] - 87;
        } else {
            num2 = buf[i*2+1]-'0';
        }
        sbuf[i] = (num1)*16 + (num2);
    }
    rs485_send(sbuf,count/2);
    l_free(sbuf);
    return 0;
}

static int equipment_re485_baud(lua_State *L) {
    int baud = luaL_checkinteger(L, -1);
    rs485_baud_set(baud);
    return 0;
}

static int equipment_re485_read_set(lua_State *L) {
    size_t count;
    const char * buf = luaL_checklstring(L, 1,&count);
    int num = luaL_checkinteger(L, 2);
    uint8_t head = (buf[0]-'0')*16 + (buf[1]-'0');
    if(num > 30) {
        num = 0;
    }
    rs485_rec_set(head,num);
    return 0;
}

static int equipment_re485_read_flag_clear(lua_State *L) {
    rs485_rec_flag_set(0);
    return 0;
}

static int equipment_re485_read_flag(lua_State *L) {
    if(rs485_rec_flag() == 0) {
        lua_pushinteger(L, 0);
    } else {
        lua_pushinteger(L, 1);
    }
    return 1;
}

static int equipment_re485_read(lua_State *L) {
    uint8_t * sbuf = rs485_rec_get();
    uint8_t * buf = l_malloc(sizeof(char)*(rs485_rec_len()*2));
    for(int i = 0; i < rs485_rec_len(); i++) {
        if(sbuf[i]/16 > 10) {
            buf[i*2] = sbuf[i]/16 + 87;
        } else {
            buf[i*2] = sbuf[i]/16 + '0';
        }
        if(sbuf[i]%16 > 10) {
            buf[i*2+1] = sbuf[i]%16 + 87;
        } else {
            buf[i*2+1] = sbuf[i]%16 + '0';
        }
    }
    lua_pushlstring(L,(const char *)buf,rs485_rec_len()*2);
    l_free(buf);
    return 1;
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
    {"re485_write", equipment_re485_write},
    {"re485_baud",  equipment_re485_baud},
    {"re485_read_set",  equipment_re485_read_set},
    {"re485_flag_clear",  equipment_re485_read_flag_clear},
    {"re485_read_flag",  equipment_re485_read_flag},
    {"re485_read",  equipment_re485_read},
    {NULL, NULL}
};
 
LUAMOD_API int luaopen_equipment (lua_State *L) {
  luaL_newlib(L, equipment);
  return 1;
}
