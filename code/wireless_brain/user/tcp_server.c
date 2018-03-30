#include <lwip/sockets.h>  
#include <lwip/err.h>  
#include <lwip/sys.h>  
#include "app_configure.h"

#include "mb.h"
#include "mbutils.h"

#define PORT 502  
  
uint8_t data_buffer[100];  
  
static void tcp_server_thread(void *p_arg)  
{  
    struct sockaddr_in server_addr;
    struct sockaddr_in conn_addr;
    int sock_fd;/* server socked */
    int sock_conn;/* request socked */
    socklen_t addr_len;
    int err;
    int length;
    int count = 0;
    
    eMBTCPInit(PORT);
    eMBEnable();
      
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);  
    if (sock_fd == -1) {
        
    }
      
    server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr =htonl(INADDR_ANY);  
    server_addr.sin_port = htons(PORT);  
      
    err = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));  
    if (err < 0) {  
        
    }

    err = listen(sock_fd, 1);  
    if (err < 0) {  
        
    }

    addr_len = sizeof(struct sockaddr_in);  
      
    printf("before accept!\n");  
    sock_conn = accept(sock_fd, (struct sockaddr *)&conn_addr, &addr_len);  
    printf("after accept!\n");  
      
    while (1) {        
        eMBPoll();
//        length = recv(sock_conn, (unsigned int *)data_buffer, 20, 0);  
//        if(length > 0) {
//            printf("length received %d\n", length);  
//            printf("received string: %s\n", data_buffer);  
//            printf("received count: %d\n", count);  
//      
//            //send(sock_conn, "good", 5, 0);
//        } else {
//            printf("before accept!\n");  
//            sock_conn = accept(sock_fd, (struct sockaddr *)&conn_addr, &addr_len);  
//            printf("after accept!\n"); 
//        }
    }  
}  

extern UCHAR    ucTCPRequestFrame[263];
extern USHORT   ucTCPRequestLen;

extern UCHAR    ucTCPResponseFrame[263];
extern USHORT   ucTCPResponseLen;

extern BOOL   bFrameSent;

void modbus_tcp_server(int conn) 
{
  int buflen = 1500;
  int ret;

  int i;
    
  printf("start modbus tcp\r\n");
  ret = read(conn, ucTCPRequestFrame, 263); 
  while ( ret > 0 )
  {
      ret = read(conn, ucTCPRequestFrame, 263);
      printf("\r\n>:");  // debug print
      for(i=0; i<ret; i++)
      {
         printf("%x ", ucTCPRequestFrame[i]);
      }
        printf("\r\n>");
        ucTCPRequestLen = ret;
      xMBPortEventPost( EV_FRAME_RECEIVED );
  }
    
   close(conn);
   printf("close modbus tcp\r\n");
}
/**
  * @brief  modbus_task
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
int newconn;

static void modbus_task(void *arg)
{
  int sock, size;
  struct sockaddr_in address, remotehost;
  while(1)
  {
//      while(1) {
//         eMBPoll();
//      }
       /* create a TCP socket */
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
       {
           printf("can not create socket\r\n");
           continue;
       }

      address.sin_family = AF_INET;
      address.sin_port = htons(502);   // mosbus tcp port
      address.sin_addr.s_addr = INADDR_ANY;

     if (bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
     {
         printf("can not bind socket\r\n");
         close(sock);
         continue;
     }

     /* listen for incoming connections (TCP listen backlog = 1) */
     listen(sock, 1);

     size = sizeof(remotehost);
     newconn = accept(sock, (struct sockaddr *)&remotehost, (socklen_t *)&size);
     if (newconn >= 0)
     {
         close(sock);  //一次只接受一个连接
         printf("connect socket\r\n");
         modbus_tcp_server(newconn);
     }
     else
     {
         close(sock);
         close(newconn);
     }
   }
}

void modbus_task2(void *p) {
   eMBTCPInit(502);      
  eMBEnable();
    while(1) {
        vTaskDelay(1/portTICK_RATE_MS);
        eMBPoll();
    }
}

void modbus_task3(void *p) {
    while(1){
        vTaskDelay(1/portTICK_RATE_MS);
        if(bFrameSent) {
            bFrameSent = FALSE;
            printf("send \n");
            // uIP发送Modbus应答数据包
            send(newconn, ucTCPResponseFrame, ucTCPResponseLen, 0);
        }
    }
}

void tcp_server_init(void) {
    sys_thread_new("tcp_server_thread",  modbus_task, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO - 1);  
    sys_thread_new("tcp_server_thread",  modbus_task2, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO - 1);  
    sys_thread_new("tcp_server_thread",  modbus_task3, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO - 1);  
}  