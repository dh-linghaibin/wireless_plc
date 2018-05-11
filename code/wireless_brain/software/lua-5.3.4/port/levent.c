/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "levent.h"
#include "l_list.h"
#include "task_modbus.h"

static l_list_t * event_head; /* ����ͷ */
 
void levent_init(void) {
    event_head = (l_list_t *)l_malloc(sizeof(l_list_t)); /* ����ͷ�ڵ� */
    list_init(event_head); //��ʼ������
}

uint16_t levent_add(int l_id, device_type type, uint16_t address, uint8_t num, uint16_t val) {
    levent * event;
    event = l_malloc(sizeof(levent));
    event->type = type;
    event->address = address;
    event->num = num;
    event->val = val;
    event->l_id = l_id;
    event->flag = 0;
    event->id = list_len(event_head);
    list_append(event_head, event); //׷�ӽ��
    //printf("add event \n");
    return event->id;
}

void levent_remove(uint16_t time_id) {
    levent * event;
    list_get(event_head, time_id, (void **)&event); 
    list_pop(event_head,time_id);
    l_free(event);
}

void levent_loop(lua_State * L) {
    levent * event;
    for (int i = 0; i < list_len(event_head); i++){ 
        list_get(event_head, i, (void **)&event);
        switch(event->type) {
            case HOLD: {
                if(task_modbus_get_holding(event->address) == event->val) {
                    if(event->flag == 0) {
                        event->flag = 1;
                        lua_rawgeti(L, LUA_REGISTRYINDEX, event->l_id);
                        lua_pcall(L, 0, 0, 0);
                    }
                } else {
                    event->flag = 0;
                }
            } break;
            case DO_8: {
                
            } break;
            case DO_4: {
            
            } break;
            case DI_4: {
                if(task_modbus_get_input_bit(event->address,event->num) == event->val) {
                    if(event->flag == 0) {
                        event->flag = 1;
                        lua_rawgeti(L, LUA_REGISTRYINDEX, event->l_id);
                        lua_pcall(L, 0, 0, 0);
                    }
                } else {
                    event->flag = 0;
                }
            } break;
        }
    }
}

static int levent_l_add(lua_State *L) {
    int type = luaL_checkinteger(L, 1);
    int address = luaL_checkinteger(L, 2);
    int num = luaL_checkinteger(L, 3);
    int val = luaL_checkinteger(L, 4);
    if(lua_isfunction(L, 5)) {
        int func_id = 0;
        lua_pushvalue(L, 5); 
        func_id = luaL_ref(L, LUA_REGISTRYINDEX);
        int id = levent_add(func_id,type,address,num,val);
        lua_pushinteger(L, id);
        return 1;
    } else {
        return 0;
    }
}

static int levent_l_remove(lua_State *L) {
    int id = luaL_checkinteger(L, 1);
    levent_remove(id);
    return 0;
}

static luaL_Reg r_levent[] = {
    //c�ӿں��������Է���������lua������  
    {"add", levent_l_add},
    {"remove", levent_l_remove},
    {NULL, NULL}
};

//�������������ʹ�����ü�ֵ  
#define LUA_ENUM(L, val) \
  lua_pushliteral(L, #val); \
  lua_pushnumber(L, val); \
  lua_settable(L, -3)
  
  
void register_enum_device(lua_State* L)  
{   //����һ��enumTable,���ڴ洢Enum������ö�١�ͨ��ö������=ö��ֵ  
    lua_newtable(L);  
    //���� enumTable["Monday"]=Monday  
    LUA_ENUM(L, DO_8);
    LUA_ENUM(L, DO_4);
    LUA_ENUM(L, DI_4);
    LUA_ENUM(L, HOLD);
    //��enumTable������_G ȫ�ֻ���(�̻߳���)��,��ö��������Ϊ��  
    lua_setglobal(L,"device");
}  

LUAMOD_API int luaopen_levent (lua_State *L) {
  register_enum_device(L);
  luaL_newlib(L, r_levent);
  return 1;
}
