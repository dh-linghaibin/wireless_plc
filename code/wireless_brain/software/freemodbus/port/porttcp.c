/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "sockets.h"

#include "app_configure.h"

#include <stdio.h>
#include <string.h>
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//lwip
#include <lwip/sockets.h>  
#include <lwip/err.h>  
#include <lwip/sys.h>  

#include "malloc.h"

#define MB_TCP_DEFAULT_PORT  502 /* TCP listening port. */

/* ----------------------- Prototypes ---------------------------------------*/
#define MB_BUF_LEN 150/*接收包长度*/
static UCHAR    *ucTCPRequestFrame;
static USHORT   ucTCPRequestLen;

static int newconn;
static void modbus_task(void *arg);
/* ----------------------- Begin implementation -----------------------------*/
BOOL xMBTCPPortInit( USHORT usTCPPort ) {
    ucTCPRequestFrame = l_malloc(sizeof(UCHAR)*MB_BUF_LEN);
    xTaskCreate( modbus_task, ( signed portCHAR * ) "ts1", 512,
                            NULL, tskIDLE_PRIORITY+1, NULL );  
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
    send(newconn, pucMBTCPFrame, usTCPLength, 0);
    return TRUE;
}

void modbus_tcp_server(int conn) {
    int ret;
    printf("start modbus tcp\r\n");
    ret = read(conn, ucTCPRequestFrame, MB_BUF_LEN); 
    while ( ret > 0 ) {
        ret = read(conn, ucTCPRequestFrame, MB_BUF_LEN);
//        printf("\r\n>:");  // debug print
//        for(i=0; i<ret; i++) {
//            printf("%x ", ucTCPRequestFrame[i]);
//        }
//        printf("\r\n>");
        ucTCPRequestLen = ret;
        xMBPortEventPost( EV_FRAME_RECEIVED );
        eMBPoll();
        eMBPoll();
    }
    close(conn);
    printf("close modbus tcp\r\n");
}

static void modbus_task(void *arg) {
    int sock, size;
    struct sockaddr_in address, remotehost;
    while(1) {
        /* create a TCP socket */
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("can not create socket\r\n");
            continue;
        }
        address.sin_family = AF_INET;
        address.sin_port = htons(502);   // mosbus tcp port
        address.sin_addr.s_addr = INADDR_ANY;
        if (bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0) {
            printf("can not bind socket\r\n");
            close(sock);
            continue;
        }
        /* listen for incoming connections (TCP listen backlog = 1) */
        listen(sock, 1);
        size = sizeof(remotehost);
        newconn = accept(sock, (struct sockaddr *)&remotehost, (socklen_t *)&size);
        if (newconn >= 0) {
            close(sock);  //一次只接受一个连接
            printf("connect socket\r\n");
            modbus_tcp_server(newconn);
        } else {
            close(sock);
            close(newconn);
        }
    }
}

