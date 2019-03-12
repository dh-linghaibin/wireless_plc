/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "app_configure.h"
#include "stdio.h"
//lua
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "string.h"
#include "ff.h"

typedef int FILEHANDLE;
#pragma import(__use_no_semihosting_swi)
#pragma import(_main_redirection)
const char __stdin_name[150];
const char __stdout_name[150];
const char __stderr_name[150];


FILEHANDLE _sys_open(const char *name, int openmode) {
    return 0;
}

int _sys_close(FILEHANDLE fh) {
    return 0;
}

int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode) {
    return 0;
}

int _sys_read(FILEHANDLE fh, unsigned char*buf, unsigned len, int mode) {
    return 0;
}

//检查句柄是否为终端
int _sys_istty(FILEHANDLE fh) {
    return 0;
}

int _sys_seek(FILEHANDLE fh, long pos) {
    return 0;
}

//刷新句柄关联的缓冲区
int _sys_ensure(FILEHANDLE fh) {
    return 0;
}

//返回文件当前长度
long _sys_flen(FILEHANDLE fh) {
    return 0;
}

int _sys_tmpnam(char *name, int fileno, unsigned maxlength) {
    return 0;
}

//将一个字符写入控制台
void _ttywrch(int ch) {

}

int remove(const char *filename) {    
    return 0;
}

char *_sys_command_string(char *cmd, int len) {
    return 0;
}

int time(int *t) {  
    return 0;  
}

unsigned int clock(void) {
    return 0;
}

int system(const char* a) {
    return 0;
}

int rename(const char *a, const char *b) {
    return 0;
}
