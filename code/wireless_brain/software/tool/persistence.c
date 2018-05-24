/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "persistence.h"
#include "w25qxx.h"
#include "can.h"
 
void persistence_init(void) {
    uint8_t dat[6];
    persistence_get_res(dat);
    if( 0x51 != dat[0] ) {
        dat[0] = 0x51;
        
        dat[0] = BAUD_RATE_50K;
        persistence_set_can_adr(dat);
        
        persistence_set_res(dat);
        dat[0] = 192;
        dat[1] = 168;
        dat[2] = 1;
        dat[3] = 193;
        persistence_set_ip(dat);
        dat[0] = 255;
        dat[1] = 255;
        dat[2] = 255;
        dat[3] = 0;
        persistence_set_mask(dat);
        dat[0] = '1';
        dat[1] = '2';
        dat[2] = '3';
        dat[3] = '4';
        dat[4] = '5';
        dat[5] = '6';
        persistence_set_password(dat);
        dat[0] = 0;
        persistence_set_lock(dat);
    }
}

void persistence_set_res(uint8_t * flag) {
    W25QXX_Write(flag,PA_RES,1);
}

void persistence_get_res(uint8_t * flag) {
    W25QXX_Read(flag,PA_RES,1);
}

void persistence_set_can_adr(uint8_t * can_adr) {
    W25QXX_Write(can_adr,PA_CAN_ADR,1);
}

void persistence_get_can_adr(uint8_t * can_adr) {
    W25QXX_Read(can_adr,PA_CAN_ADR,1);
}

void persistence_set_pro_flag(uint8_t * flag) {
    W25QXX_Write(flag,PA_PRO_FLAG,1);
}

void persistence_get_pro_flag(uint8_t * flag) {
    W25QXX_Read(flag,PA_PRO_FLAG,1);
}

void persistence_set_ip(uint8_t * ip) {
    W25QXX_Write(ip,PA_IP,4);
}

void persistence_get_ip(uint8_t * ip) {
    W25QXX_Read(ip,PA_IP,4);
}

void persistence_get_mask(uint8_t * mask) {
    W25QXX_Read(mask,PA_MASK,4);
}

void persistence_set_mask(uint8_t * mask) {
     W25QXX_Write(mask,PA_MASK,4);
}

void persistence_get_password(uint8_t * pas) {
    W25QXX_Read(pas,PA_PAS,6);
}

void persistence_set_password(uint8_t * pas) {
    W25QXX_Write(pas,PA_PAS,6);
}

void persistence_get_lock(uint8_t * lock) {
    W25QXX_Read(lock,PA_LOCK,1);
}

void persistence_set_lock(uint8_t * lock) {
    W25QXX_Write(lock,PA_LOCK,1);
}

