

#ifndef RADIO_H_
#define RADIO_H_

#include "typex.h"
#include "radio_config.h"
#include "radio_hal.h"
#include "radio_comm.h"
#include "si446x_api_lib.h"
#include "si446x_defs.h"
#include "si446x_nirq.h"
//#include "si446x_patch.h"
#include "wirelessTxRx.h"
#include <stdarg.h>


#define RADIO_MAX_PACKET_LENGTH     64u


typedef struct
{
   const  U8   *Radio_ConfigurationArray;

    U8   Radio_ChannelNumber;
    U8   Radio_PacketLength;
    U8   Radio_State_After_Power_Up;

    U16  Radio_Delay_Cnt_After_Reset;

    U8   Radio_CustomPayload[RADIO_MAX_PACKET_LENGTH];
} tRadioConfiguration;


extern const SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration, SEG_CODE, SEG_CODE);
extern SEGMENT_VARIABLE(customRadioPacket[RADIO_MAX_PACKET_LENGTH], U8, SEG_XDATA);


extern const SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8, SEG_CODE);


void  vRadio_Init(void);
U8    bRadio_Check_Tx_RX(void);
void  vRadio_StartRX(U8,U8);
U8    bRadio_Check_Ezconfig(U16);
void  vRadio_StartTx_Variable_Packet(U8, U8*, U8);

#endif /* RADIO_H_ */
