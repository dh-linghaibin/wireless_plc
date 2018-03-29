#include "mb.h"
#include "mbutils.h"
#include "app_configure.h"

#define REG_INPUT_START       0                     // ����Ĵ�����ʼ��ַ
#define REG_INPUT_NREGS       100                    // ����Ĵ�������

#define REG_HOLDING_START     0                // ���ּĴ�����ʼ��ַ
#define REG_HOLDING_NREGS     800                    // ���ּĴ�������

#define REG_COILS_START       0                // ��Ȧ��ʼ��ַ
#define REG_COILS_SIZE        100              // ��Ȧ����

#define REG_DISCRETE_START    0                // ���ؼĴ�����ʼ��ַ
#define REG_DISCRETE_SIZE     100                    // ���ؼĴ�������

static uint8_t modbus_coil[100];
static uint8_t modbus_input[100];
static int modbus_Holding[120];

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
                *pucRegBuffer++ = ( unsigned char )( 0 );
                *pucRegBuffer++ = ( unsigned char )( 8 );
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
                if((*pucRegBuffer++) == 0) {
                    modbus_coil[r_num] &= ~(1<<r_bit);
                } else {
                    modbus_coil[r_num] |= 1<<r_bit;
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
            *pucRegBuffer++ = modbus_input[adr_num];
            iNDiscrete -= 8;
            adr_num += 1;
        }
    } else {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}
