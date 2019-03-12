/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
#include "sram.h"

#define SRAMIN     0//内部内存池
#define SRAMCCM  1//CCM内存池(此部分SRAM仅仅CPU可以访问!!!)

#define SRAMBANK 2//定义支持的SRAM块数.

//mem1内存参数设定.mem1完全处于内部SRAM里面.
#define MEM1_BLOCK_SIZE            32//内存块大小为32字节
#define MEM1_MAX_SIZE            1*1024//最大管理内存 100K
#define MEM1_ALLOC_TABLE_SIZE    MEM1_MAX_SIZE/MEM1_BLOCK_SIZE     //内存表大小

//mem3内存参数设定.mem3处于CCM,用于管理CCM(特别注意,这部分SRAM,仅CPU可以访问!!)
#define MEM3_BLOCK_SIZE            32//内存块大小为32字节
#define MEM3_MAX_SIZE            1 *1024//最大管理内存60K
#define MEM3_ALLOC_TABLE_SIZE    MEM3_MAX_SIZE/MEM3_BLOCK_SIZE//内存表大小


//内存管理控制器
struct _m_mallco_dev
{
    void (*init)(u8);//初始化
    u8 (*perused)(u8);//内存使用率
    u8     *membase[SRAMBANK];//内存池 管理SRAMBANK个区域的内存
    u16 *memmap[SRAMBANK];//内存管理状态表
    u8  memrdy[SRAMBANK];//内存管理是否就绪
};
extern struct _m_mallco_dev mallco_dev;     //在mallco.c里面定义

//内存池(32字节对齐)
__align(32) u8 mem1base[MEM1_MAX_SIZE];//内部SRAM内存池
__align(32) u8 mem3base[MEM3_MAX_SIZE] __attribute__((at(0X10000000)));//内部CCM内存池
//内存管理表
u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];//内部SRAM内存池MAP
u16 mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000+MEM3_MAX_SIZE)));    //内部CCM内存池MAP
//内存管理参数       
const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};//内存表大小
const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM3_BLOCK_SIZE};//内存分块大小
const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM3_MAX_SIZE};//内存总大小


void mymemset(void *s,u8 c,u32 count);//设置内存
void mymemcpy(void *des,void *src,u32 n);//复制内存
void my_mem_init(u8 memx);//内存管理初始化函数(外/内部调用)
u32 my_mem_malloc(u8 memx,u32 size);//内存分配(内部调用)
u8 my_mem_free(u8 memx,u32 offset);//内存释放(内部调用)
u8 my_mem_perused(u8 memx);//获得内存使用率(外/内部调用) 

//内存管理控制器
struct _m_mallco_dev mallco_dev=
{
    my_mem_init,//内存初始化
    my_mem_perused,//内存使用率
    mem1base,mem3base,//内存池
    mem1mapbase,mem3mapbase,//内存管理状态表
    0,0,//内存管理未就绪
};

//复制内存
//*des:目的地址
//*src:源地址
//n:需要复制的内存长度(字节为单位)
void mymemcpy(void *des,void *src,u32 n)  
{  
    u8 *xdes=des;
    u8 *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  
//设置内存
//*s:内存首地址
//c :要设置的值
//count:需要设置的内存大小(字节为单位)
void mymemset(void *s,u8 c,u32 count)  
{  
    u8 *xs = s;  
    while(count--)*xs++=c;  
}       
//内存管理初始化  
//memx:所属内存块
void my_mem_init(u8 memx)  
{  
    mymemset(mallco_dev.memmap[memx], 0,memtblsize[memx]*2);//内存状态表数据清零  
    mymemset(mallco_dev.membase[memx], 0,memsize[memx]);//内存池所有数据清零  
    mallco_dev.memrdy[memx]=1;//内存管理初始化OK  
}  
//获取内存使用率
//memx:所属内存块
//返回值:使用率(0~100)
u8 my_mem_perused(u8 memx)  
{  
    u32 used=0;
    u32 i;
    for(i=0;i<memtblsize[memx];i++)  
    {  
        if(mallco_dev.memmap[memx][i])used++; 
    } 
    return (used*100)/(memtblsize[memx]);  
}  
//内存分配(内部调用)
//memx:所属内存块
//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
u32 my_mem_malloc(u8 memx,u32 size)  
{  
    signed long offset=0;  
    u32 nmemb;    //需要的内存块数  
    u32 cmemb=0;//连续空内存块数
    u32 i;  
    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);//未初始化,先执行初始化 
    if(size==0)return 0XFFFFFFFF;//不需要分配
    nmemb=size/memblksize[memx];//获取需要分配的连续内存块数
    if(size%memblksize[memx])nmemb++;
    for(offset=memtblsize[memx]-1;offset>=0;offset--)//搜索整个内存控制区  
    {     
        if(!mallco_dev.memmap[memx][offset])cmemb++;//连续空内存块数增加
        else cmemb=0;//连续内存块清零
        if(cmemb==nmemb)//找到了连续nmemb个空内存块
        {
            for(i=0;i<nmemb;i++)//标注内存块非空 
            {  
                mallco_dev.memmap[memx][offset+i]=nmemb;  
            }  
            return (offset*memblksize[memx]);//返回偏移地址  
        }
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  
//释放内存(内部调用) 
//memx:所属内存块
//offset:内存地址偏移
//返回值:0,释放成功;1,释放失败;  
u8 my_mem_free(u8 memx,u32 offset)  
{  
    int i;  
    if(!mallco_dev.memrdy[memx])//未初始化,先执行初始化
    {
        mallco_dev.init(memx);    
        return 1;//未初始化  
    }  
    if(offset<memsize[memx])//偏移在内存池内. 
    {  
        int index=offset/memblksize[memx];            //偏移所在内存块号码  
        int nmemb=mallco_dev.memmap[memx][index];    //内存块数量
        for(i=0;i<nmemb;i++)                          //内存块清零
        {  
            mallco_dev.memmap[memx][index+i]=0;  
        }  
        return 0;  
    }else return 2;//偏移超区了.  
}  
//释放内存(外部调用) 
//memx:所属内存块
//ptr:内存首地址 
void myfree(u8 memx,void *ptr)  
{  
    u32 offset;   
    if(ptr==NULL)return;//地址为0.  
     offset=(u32)ptr-(u32)mallco_dev.membase[memx];     
    my_mem_free(memx,offset);    //释放内存      
}  
//分配内存(外部调用)
//memx:所属内存块
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *mymalloc(u8 memx,u32 size)  
{  
    u32 offset;   
    offset=my_mem_malloc(memx,size);                     
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)mallco_dev.membase[memx]+offset);  
}  
//重新分配内存(外部调用)
//memx:所属内存块
//*ptr:旧内存首地址
//size:要分配的内存大小(字节)
//返回值:新分配到的内存首地址.
void *myrealloc(u8 memx,void *ptr,u32 size)  
{  
    u32 offset;    
    offset=my_mem_malloc(memx,size);       
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {                                         
        mymemcpy((void*)((u32)mallco_dev.membase[memx]+offset),ptr,size);    //拷贝旧内存内容到新内存   
        myfree(memx,ptr);                                                        //释放旧内存
        return (void*)((u32)mallco_dev.membase[memx]+offset);                  //返回新内存首地址
    }  
}

void l_malloc_init(void) {
    my_mem_init(SRAMIN);//初始化内部内存池
    my_mem_init(SRAMCCM);//初始化CCM内存池
}

void * malloc(u32 size) {
    return mymalloc(SRAMIN,size);
}

void free(void * ptr) {
    return myfree(SRAMIN,ptr);
}

void * realloc(void *ptr,u32 size) {
    return myrealloc(SRAMIN,ptr,size);
}
