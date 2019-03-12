/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _READ_CONFIG_H_
#define _READ_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif
    
#include "app_configure.h"

typedef enum {
    PR_SET  = 1,
    PR_RESET = 0,
} par_reg_val;

typedef enum {
    SR_SET  = 1,
    SR_RESET = 0,
} SMA_RES_val;

typedef enum {
    RE_SET  = 1,
    RE_RESET = 0,
} RES_val;

typedef enum {
    PA_SET  = 1,
    PA_RESET = 0,
} PASS_val;

typedef enum {
    CC_1 = 0,
    CC_2 = 1,
    CC_3 = 2,
    CC_4 = 3,
    CC_5 = 4,
    CC_6 = 5,
    CC_7 = 6,
    CC_8 = 7,
    CC_NULL = 8
} check_channel;

typedef enum {
    RC_100R = 0,
    RC_1K = 1,
    RC_10K = 2,
    RC_NULL
} res_channel;

typedef enum {
    PT100 = 0,
    PT1000 = 1,
    VOLTAGE = 2,
    ELE_CURRENT = 3,
    AM_NULL = 4
} ads_read_mode;

void read_config_init(void);
void read_config_pass_set(check_channel passageway);
void read_config_mode(ads_read_mode mode,check_channel passageway);
float analog_quantity_get(check_channel ch);

#ifdef __cplusplus
}
#endif

#endif


