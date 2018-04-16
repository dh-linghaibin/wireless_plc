/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "task_modbus.h"
#include "mb.h"
#include "mbutils.h"
#include "task_can.h"

static uint8_t * modbus_coil; /* ÏßÈ¦ */
static uint8_t * modbus_input;
uint16_t * modbus_Holding;

void task_modbus_init(void) {
    modbus_coil = l_malloc(sizeof(uint8_t)*99);
    modbus_input = l_malloc(sizeof(uint8_t)*99);
    modbus_Holding = l_malloc(sizeof(uint16_t)*99);
    eMBTCPInit(502);
    eMBEnable(); 
}

void task_modbus_set_coil(uint16_t address,uint8_t val) {
    if(address <= 99) {
        modbus_coil[address] = val;
    }
}

void task_modbus_set_coil_bit(uint16_t address,uint8_t num,uint8_t val) {
    if(address <= 99) {
        if(val == 0) {
            modbus_coil[address] &= ~(1 << val);
        } else {
            modbus_coil[address] |= 1 << val;
        }
    }
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs ) {
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    if( ( (int16_t) usAddress >= REG_INPUT_START ) \
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) ) {
        iRegIndex = ( int )( usAddress);
        while( usNRegs > 0 ) {
            *pucRegBuffer++ = ( unsigned char )( 0xff );
            *pucRegBuffer++ = ( unsigned char )( 0xff );
            iRegIndex++;
            usNRegs--;
        }
    } else {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                eMBRegisterMode eMode ) {
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
    if( ( (int16_t)usAddress >= REG_HOLDING_START ) \
        && ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) ) {
        iRegIndex = ( int )( usAddress);
        switch ( eMode ) {
        case MB_REG_READ:            
            while( usNRegs > 0 ) {
                *pucRegBuffer++ = ( unsigned char )( modbus_Holding[0] >> 8 );
                *pucRegBuffer++ = ( unsigned char )( modbus_Holding[0] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;
        case MB_REG_WRITE:
            while( usNRegs > 0 ) {
                modbus_Holding[iRegIndex] = *pucRegBuffer++ << 8;
                modbus_Holding[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
            break;
        }
    } else {
        eStatus = MB_ENOREG;
    }
    
    return eStatus;
}

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
              eMBRegisterMode eMode ) {
    eMBErrorCode    eStatus = MB_ENOERR;
    short           iNCoils = ( short )usNCoils;
    unsigned short  usBitOffset;
    if( ( (int16_t)usAddress >= REG_COILS_START ) &&
       ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) ) {
        usBitOffset = ( unsigned short )( usAddress - REG_COILS_START );
        uint16_t adr_num = usBitOffset/8;
        switch ( eMode ) {
        case MB_REG_READ:
            while( iNCoils > 0 ) {
                *pucRegBuffer++ = modbus_coil[adr_num];
                iNCoils -= 8;
                adr_num += 1;
            }
            break;
        case MB_REG_WRITE:
            while( iNCoils > 0 ) {
                uint16_t r_num = usBitOffset/8;
                uint16_t r_bit = usBitOffset%8;
                
                device_send send_msg;
                send_msg.type = DO_8;
                send_msg.address = r_num;
                send_msg.cmd = 0x00;
                send_msg.data[0] = r_bit;
                send_msg.data[1] = (*pucRegBuffer++);
                task_can_set(send_msg);
                iNCoils -= 8;
                usBitOffset += 8;
            }
            break;
        }
    } else {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete ) {
    eMBErrorCode    eStatus = MB_ENOERR;
    short           iNDiscrete = ( short )usNDiscrete;
    unsigned short  usBitOffset;
    if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
       ( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) ) {
        usBitOffset = ( unsigned short )( usAddress - REG_DISCRETE_START );
        uint16_t adr_num = usBitOffset/8;
        while( iNDiscrete > 0 ) {
            *pucRegBuffer++ = modbus_input[adr_num];
            iNDiscrete -= 8;
            adr_num += 1;
        }
    } else {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

 