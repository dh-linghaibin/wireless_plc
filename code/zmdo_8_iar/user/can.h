#ifndef __CAN_H
#define __CAN_H
/* 引用头文件 *****************************************************************/
#include "stm32f0xx.h"

/* 外部数据类型 ***************************************************************/
typedef struct {
	GPIO_TypeDef* 	port;
	uint16_t        pin;
} Stdlcan_pin_TypeDef;

typedef enum {
	B_250K = 4,
    B_125K = 3,
    B_50K = 2,
    B_20K = 1,
    B_10K = 0,
    B_5K = 6,
} btl_e;


typedef enum {
	P_HEAD = 2,
	P_CMD = 5,
	P_LEN = 1,
	P_ADDR = 2,
	P_O0 = 6,
	P_O1 = 7,
	P_O2 = 8,
	P_O3 = 10,
	P_O4 = 11,
	P_O5 = 12,
	P_O6 = 13,
	P_O7 = 14,
}packet_cmd_e;

typedef enum {
	F_NO_USE = 0,
	F_USE,
	F_PACK_OK,
}packet_flag_e;

#define PACKAGE_NUM 10

typedef struct _pacck_obj {
	uint8_t flag;
    uint32_t id;
	uint8_t dat[8];
}pack_obj;

typedef struct _can_package_obj {
	pack_obj package[PACKAGE_NUM];
}can_package_obj;

typedef struct _can_packr_obj {
	uint8_t flag;
	uint8_t pack_bum;
	uint8_t id;
	uint8_t device_id;
	uint8_t cmd;
	uint8_t len;
	uint8_t arr[32];
}can_packr_obj;

typedef struct _can_message_obj {
	CanTxMsg data;
}can_message_obj;

typedef struct {
    Stdlcan_pin_TypeDef rx;
    Stdlcan_pin_TypeDef tx;
    Stdlcan_pin_TypeDef s;
    uint16_t id;//can地址
    btl_e btl;//波特率
    uint8_t package[8];//缓存包
    CanTxMsg TxMessage;
}Stdcan_n; 


/* 设备类型 */
typedef enum {
    DO_8 = 0,
    DO_4 = 1,
    DI_4 = 2,
} device_type;

/* 消息频道 */
typedef enum {
    RADIO = 0x0155,
    RADIO_ASK = 0,
    RADIO_SET_BR = 1,
    RADIO_SET_ADDRESS = 2,
} device_channel;

typedef struct canbus{   
    Stdcan_n can_n;
    can_message_obj send_msg;
    void (*Init)(Stdcan_n* can);
    void (*Send)(struct canbus* can);
    void (*setid)(Stdcan_n* can,uint8_t id);
    void (*set_btl)(Stdcan_n* can,btl_e btl);
    can_package_obj* (*get_packget)(Stdcan_n* can);
}Stdcanbus; 
/* 外部常数宏 *****************************************************************/
/* 外部动作宏 *****************************************************************/
/* 外部变量 *******************************************************************/
/* 函数声明 *******************************************************************/
void CanInit(Stdcan_n* can);
void CanSend(struct canbus* can);
void CanSetID(Stdcan_n* can,uint8_t id);
void CanSetBlt(Stdcan_n* can,btl_e btl);
can_package_obj*  bxcan_get_packget(Stdcan_n* can);

#endif
/***************************************************************END OF FILE****/
