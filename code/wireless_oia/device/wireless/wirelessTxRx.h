#ifndef WIRELESSTXRX_H_
#define WIRELESSTXRX_H_

#define	PACKET_LENGTH						    0x14    //user define

extern SEGMENT_VARIABLE(Tx_Buffer[PACKET_LENGTH], U8 , SEG_XDATA);
extern SEGMENT_VARIABLE(Rx_Buffer[PACKET_LENGTH], U8 , SEG_XDATA);


void Tx_bufferload(void);
void WirelessTx_handler(U8 ChannelNum,U8 Tx[],U8 TxarrayLength);
void WirelessRx_handler(U8 Rx[],U8 RxarrayLength);


#endif

