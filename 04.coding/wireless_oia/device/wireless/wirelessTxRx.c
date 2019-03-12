


#include "radio.h"







SEGMENT_VARIABLE(Tx_Buffer[PACKET_LENGTH], U8 , SEG_XDATA);
SEGMENT_VARIABLE(Rx_Buffer[PACKET_LENGTH], U8 , SEG_XDATA);

void Tx_bufferload(void)
{
	Tx_Buffer[0]=0x01;
	Tx_Buffer[1]=0x01;
	Tx_Buffer[2]=0x01;
	Tx_Buffer[3]=0x01;
	Tx_Buffer[4]=0x01;
	Tx_Buffer[5]=0x01;
	Tx_Buffer[6]=0x01;
	Tx_Buffer[7]=0x01;
	Tx_Buffer[8]=0x01;
	Tx_Buffer[9]=0x01;
	Tx_Buffer[10]=0x10;
	Tx_Buffer[11]=0x10;
	Tx_Buffer[12]=0x10;
	Tx_Buffer[13]=0x10;
	Tx_Buffer[14]=0x10;
	Tx_Buffer[15]=0x10;
	Tx_Buffer[16]=0x10;
	Tx_Buffer[17]=0x10;
	Tx_Buffer[18]=0x10;
	Tx_Buffer[19]=0x10;

}



void WirelessTx_handler(U8 ChannelNum,U8 Tx[],U8 TxarrayLength)
{


	SEGMENT_VARIABLE(Countpos,U8, SEG_DATA);


  	for(Countpos = 0u; Countpos < TxarrayLength; Countpos++)
  	{
    	customRadioPacket[Countpos] = Tx[Countpos];
  	}
	vRadio_StartTx_Variable_Packet(ChannelNum, &customRadioPacket[0], TxarrayLength);
}



void WirelessRx_handler(U8 Rx[],U8 RxarrayLength)
{
	SEGMENT_VARIABLE(Countpos,U8, SEG_DATA);
  	for(Countpos = 0u; Countpos < RxarrayLength; Countpos++)
  	{
    	Rx[Countpos]=customRadioPacket[Countpos]; 
  	}
}