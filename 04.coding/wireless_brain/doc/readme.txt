
修改记录 


-----------------18-7-13--------------------------------------------------------

网络导致的死机BUG

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define TCPIP_THREAD_STACKSIZE          1000
#define TCPIP_MBOX_SIZE                 16
#define DEFAULT_UDP_RECVMBOX_SIZE       2000
#define DEFAULT_TCP_RECVMBOX_SIZE       2000
#define DEFAULT_ACCEPTMBOX_SIZE         2000
#define DEFAULT_THREAD_STACKSIZE        500
#define TCPIP_THREAD_PRIO               (configMAX_PRIORITIES - 2)

修改为

#define TCPIP_THREAD_STACKSIZE          1000
#define TCPIP_MBOX_SIZE                 32
#define DEFAULT_UDP_RECVMBOX_SIZE       2000
#define DEFAULT_TCP_RECVMBOX_SIZE       2000
#define DEFAULT_ACCEPTMBOX_SIZE         2000
#define DEFAULT_THREAD_STACKSIZE        500
#define TCPIP_THREAD_PRIO               (configMAX_PRIORITIES - 2)

--------------------------------------------------------------------------------
---------------18-7-16--出错位置

#if LWIP_ETHERNET
      if (msg->msg.inp.netif->flags & (NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET)) {
        ethernet_input(msg->msg.inp.p, msg->msg.inp.netif);
      } else
#endif /* LWIP_ETHERNET */
      {
        ip_input(msg->msg.inp.p, msg->msg.inp.netif);
      }
      memp_free(MEMP_TCPIP_MSG_INPKT, msg);
      break;

---------更改

/** LWIP_ETHERNET==1: enable ethernet support for PPPoE even though ARP
 * might be disabled
 */
#ifndef LWIP_ETHERNET
#define LWIP_ETHERNET                   0//(LWIP_ARP || PPPOE_SUPPORT)
#endif


这几天在学习lwip，由于stm32f2的官方默认的lwip的版本是1.3.2的，于是就想把它换成1.4.1的，在官网下载了相应的库，移植后发现功能正常，但是ping超时，通过一番追踪发现，在icmp_input函数里面
#if CHECKSUM_GEN_ICMP
    /* adjust the checksum */
    if (iecho->chksum >= PP_HTONS(0xffffU - (ICMP_ECHO << 8))) {
      iecho->chksum += PP_HTONS(ICMP_ECHO << 8) + 1;
    } else {
      iecho->chksum += PP_HTONS(ICMP_ECHO << 8);
    }
#else /* CHECKSUM_GEN_ICMP */
    iecho->chksum = 0;
#endif /* CHECKSUM_GEN_ICMP */
由于这个编译时默认编译了软件的校验，但是st又使用了硬件校验，导致了重复校验，所以返回的ping包出错了。
所以我们只需要在lwipopts.h里把
#define CHECKSUM_GEN_ICMP                    0
加上就屏得通了。

