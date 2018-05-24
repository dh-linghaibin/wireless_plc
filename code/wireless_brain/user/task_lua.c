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
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "ltime.h"
#include "levent.h"

#include "ui_debug.h"

static lua_State* L;
static xTaskHandle xhandle_lua; /* lua句柄 */
static xTaskHandle xhandle_lua_tic; /* lua句柄 */

static void task_lua(void *pvParameters);
static void task_lua_tic(void *pvParameters);

void task_lua_init(void) {
    ltime_init();
    levent_init();
    L = luaL_newstate();
    luaL_openlibs(L);
}

void task_lua_set(bool cmd) {
    if(cmd) {
        vTaskResume( xhandle_lua ); /* 恢复lua任务 */
    } else {
        vTaskSuspend( xhandle_lua ); /* 挂起lua任务 */
    }
}

void task_lua_create(void) {
     xTaskCreate( task_lua,"lua", 3072, NULL, tskIDLE_PRIORITY+3, &xhandle_lua );  
     xTaskCreate( task_lua_tic,"lua", 1024, NULL, tskIDLE_PRIORITY+3, &xhandle_lua_tic );  
}

static void task_lua(void *pvParameters) {
    for(;;) {
        if (luaL_dofile(L,"1:test.lua")!=0) {
            ui_debug_set_show(lua_tostring(L,-1));
        }
        vTaskDelay( 5000/portTICK_RATE_MS );  
    }
}

static void task_lua_tic(void *pvParameters) {
    for(;;) {
        ltime_loop(L);
        //levent_loop(L);
        vTaskDelay( 1/portTICK_RATE_MS );  
    }
}
