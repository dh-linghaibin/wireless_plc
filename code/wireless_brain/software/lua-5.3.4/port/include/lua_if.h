/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _LUA_IF_H_
#define _LUA_IF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "app_configure.h"

#include "ff.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define LUAIF_BUFFER_ADDSIZE 64

extern FIL * mos_stdin;
extern FIL * mos_stdout;
extern FIL * mos_stderr;

extern  int     mos_getc      (FIL * fp);
extern  int     mos_ungetc    (int c, FIL * fp);
extern  char*   mos_fgets     (char * buf, int bufsize, FIL * fp);
extern  int     mos_fputs     (const char * s, FIL * fp);
extern  int     mos_fprintf   (FIL * fp, const char * format, ...);

extern  int     mos_setvbuf   (FIL * fp, char * buf, int type, unsigned size);
extern  void    mos_clearerr  (FIL * fp);
extern  int     mos_ferror    (FIL * fp);
extern  FIL*    mos_tmpfile   (void);

extern  FIL*    mos_fopen     (const char * path, const char * mode);
extern  FIL*    mos_freopen   (const char * filename, const char * mode, FIL * fp);
extern  int     mos_fclose    (FIL * fp);
extern  int     mos_remove    (const char * filename);
extern  int     mos_rename    (const char * oldn, const char * newn);
extern  size_t  mos_fread     (void * buffer, size_t size, size_t count, FIL * fp);
extern  size_t  mos_fwrite    (const void * buffer, size_t size, size_t count, FIL * fp);
extern  int     mos_fflush    (FIL * fp);
extern  int     mos_fseek     (FIL * fp, long o, int w);
extern  long    mos_ftell     (FIL * fp);
extern  int     mos_feof      (FIL * fp);

#define         mos_memcpy    madMemCopy
#define         mos_memcmp    madMemCmp
#define         mos_free      madMemFree
#define         mos_realloc   madMemRealloc

size_t  l_writestring (const void * s, size_t l);
//#define lua_writestring lua_writestring
#define         lua_writeline()       printf("\n")
#define         lua_writestringerror  printf
    
#ifdef __cplusplus
}
#endif

#endif


