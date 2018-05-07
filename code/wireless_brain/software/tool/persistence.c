/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */
 
#include "persistence.h"
#include "w25qxx.h"
 
void persistence_init(void) {
    
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
    
}

void persistence_set_password(uint8_t * pas) {
    
}

