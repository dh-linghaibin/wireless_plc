
�޸ļ�¼ 


-----------------18-7-13--------------------------------------------------------

���絼�µ�����BUG

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define TCPIP_THREAD_STACKSIZE          1000
#define TCPIP_MBOX_SIZE                 16
#define DEFAULT_UDP_RECVMBOX_SIZE       2000
#define DEFAULT_TCP_RECVMBOX_SIZE       2000
#define DEFAULT_ACCEPTMBOX_SIZE         2000
#define DEFAULT_THREAD_STACKSIZE        500
#define TCPIP_THREAD_PRIO               (configMAX_PRIORITIES - 2)

�޸�Ϊ

#define TCPIP_THREAD_STACKSIZE          1000
#define TCPIP_MBOX_SIZE                 32
#define DEFAULT_UDP_RECVMBOX_SIZE       2000
#define DEFAULT_TCP_RECVMBOX_SIZE       2000
#define DEFAULT_ACCEPTMBOX_SIZE         2000
#define DEFAULT_THREAD_STACKSIZE        500
#define TCPIP_THREAD_PRIO               (configMAX_PRIORITIES - 2)

--------------------------------------------------------------------------------
---------------18-7-16--����λ��

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

---------����

/** LWIP_ETHERNET==1: enable ethernet support for PPPoE even though ARP
 * might be disabled
 */
#ifndef LWIP_ETHERNET
#define LWIP_ETHERNET                   0//(LWIP_ARP || PPPOE_SUPPORT)
#endif


�⼸����ѧϰlwip������stm32f2�Ĺٷ�Ĭ�ϵ�lwip�İ汾��1.3.2�ģ����Ǿ����������1.4.1�ģ��ڹ�����������Ӧ�Ŀ⣬��ֲ���ֹ�������������ping��ʱ��ͨ��һ��׷�ٷ��֣���icmp_input��������
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
�����������ʱĬ�ϱ����������У�飬����st��ʹ����Ӳ��У�飬�������ظ�У�飬���Է��ص�ping�������ˡ�
��������ֻ��Ҫ��lwipopts.h���
#define CHECKSUM_GEN_ICMP                    0
���Ͼ�����ͨ�ˡ�

