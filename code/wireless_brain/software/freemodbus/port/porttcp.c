/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "sockets.h"

#include "app_configure.h"

#include <stdio.h>
#include <string.h>

#define MB_TCP_DEFAULT_PORT  502          /* TCP listening port. */
#define MB_TCP_BUF_SIZE     ( 50 + 7 )   /* Must hold a complete Modbus TCP frame. */

/* ----------------------- Prototypes ---------------------------------------*/
UCHAR    ucTCPRequestFrame[50];
USHORT   ucTCPRequestLen;

UCHAR    ucTCPResponseFrame[50];
USHORT   ucTCPResponseLen;

BOOL   bFrameSent = FALSE;

/* ----------------------- Begin implementation -----------------------------*/
BOOL xMBTCPPortInit( USHORT usTCPPort ) {
    return TRUE;
}

void vMBTCPPortClose( void ) {
    
}

void vMBTCPPortDisable( void ) {
    
}

BOOL xMBTCPPortGetRequest( UCHAR ** ppucMBTCPFrame, USHORT * usTCPLength ) {
    *ppucMBTCPFrame = &ucTCPRequestFrame[0];
    *usTCPLength = ucTCPRequestLen;
    /* Reset the buffer. */
    ucTCPRequestLen = 0;
    return TRUE;
}

BOOL xMBTCPPortSendResponse( const UCHAR * pucMBTCPFrame, USHORT usTCPLength ) {
    memcpy( ucTCPResponseFrame , pucMBTCPFrame , usTCPLength);
    ucTCPResponseLen = usTCPLength;
    
    bFrameSent = TRUE;      // 通过uip_poll发送数据
    return bFrameSent;
}