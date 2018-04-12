/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "task_file.h"
#include "w25qxx.h"
#include "ff.h"
#include "stdio.h"
#include "delay.h"

//FRESULT scan_files (
//    char* path        /* Start node to be scanned (***also used as work area***) */
//)
//{
//    FRESULT res;
//    DIR dir;
//    UINT i;
//    static FILINFO fno;

//    res = f_opendir(&dir, path);                       /* Open the directory */
//    if (res == FR_OK) {
//        for (;;) {
//            res = f_readdir(&dir, &fno);                   /* Read a directory item */
//            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
//            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
//                i = strlen(path);
//                sprintf(&path[i], "/%s", fno.fname);
//                res = scan_files(path);                    /* Enter the directory */
//                if (res != FR_OK) break;
//                path[i] = 0;
//            } else {                                       /* It is a file. */
//               // printf("%s/%s\n", path, fno.fname);
//            }
//            printf("%s/%s\n", path, fno.fname);
//        }
//        f_closedir(&dir);
//    }

//    return res;
//}

void task_file_init(void) {
    FATFS *fs;
    fs = l_malloc(sizeof(FATFS));
    u8 res=f_mount(fs,"1:",1);
    if(res==0X0D) {//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
        printf("Flash Disk Formatting...\n");    //格式化FLASH
        res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇 4096
        if(res==0) {
            f_setlabel((const TCHAR *)"1:lhb");  
            printf("Flash Disk Format Finish\n");    //格式化完成
        } else { 
            printf("Flash Disk Format Error \n");    //格式化失败
        }
        delay_ms(1000);
    } else {
//        printf("Flash Disk ok \n");
//        f_mkdir("1:sub1");
//        f_mkdir("1:sub1/sub2");
//        
//        char buff[256];
//        strcpy(buff, "1:");
//        res = scan_files(buff);
//        printf("mu: %s \n",buff);
    }
}
