/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "mb.h"
#include "mbutils.h"
#include "app_configure.h"

#define REG_INPUT_START       0                     // 输入寄存器起始地址
#define REG_INPUT_NREGS       10                    // 输入寄存器数量

#define REG_HOLDING_START     0                // 保持寄存器起始地址
#define REG_HOLDING_NREGS     10                    // 保持寄存器数量

#define REG_COILS_START       0                // 线圈起始地址
#define REG_COILS_SIZE        10              // 线圈数量

#define REG_DISCRETE_START    0                // 开关寄存器起始地址
#define REG_DISCRETE_SIZE     10                    // 开关寄存器数量

//static uint8_t modbus_coil[10];
//static uint8_t modbus_input[10];
//static int modbus_Holding[10];

uint16_t modbus_Holding[10];

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
                *pucRegBuffer++ = 0x00;//modbus_coil[adr_num];
                iNCoils -= 8;
                adr_num += 1;
            }
            break;
        case MB_REG_WRITE:
            while( iNCoils > 0 ) {
                uint16_t r_num = usBitOffset/8;
                uint16_t r_bit = usBitOffset%8;
                if((*pucRegBuffer++) == 0) {
                    //modbus_coil[r_num] &= ~(1<<r_bit);
                } else {
                   // modbus_coil[r_num] |= 1<<r_bit;
                }
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
            *pucRegBuffer++ = 0x00;//modbus_input[adr_num];
            iNDiscrete -= 8;
            adr_num += 1;
        }
    } else {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}
