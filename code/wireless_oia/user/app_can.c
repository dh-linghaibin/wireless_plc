/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2017 linghaibin
 *
 */

#include "app_can.h"
#include "can.h"
#include "tm1650.h"


void app_can_init(void) {
     bxcan_init(BR_50K);
}
