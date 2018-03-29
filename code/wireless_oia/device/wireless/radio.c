

#include "radio.h"
#include "gd32f10x.h"

const SEGMENT_VARIABLE(Radio_Configuration_Data_Array[], U8, SEG_CODE) =    RADIO_CONFIGURATION_DATA_ARRAY;
//const u8 Radio_Configuration_Data_Array[]= RADIO_CONFIGURATION_DATA_ARRAY;

const SEGMENT_VARIABLE(RadioConfiguration, tRadioConfiguration, SEG_CODE) =  RADIO_CONFIGURATION_DATA;
/*
const tRadioConfiguration RadioConfiguration= { \
                            Radio_Configuration_Data_Array,                            \
                            RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER,                   \
                            RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH,              \
                            RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP,       \
                            RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET,       \
                            RADIO_CONFIGURATION_DATA_CUSTOM_PAYLOAD                   \
                            };*/

const SEGMENT_VARIABLE_SEGMENT_POINTER(pRadioConfiguration, tRadioConfiguration, SEG_CODE, SEG_CODE) =   &RadioConfiguration;


SEGMENT_VARIABLE(customRadioPacket[RADIO_MAX_PACKET_LENGTH], U8, SEG_XDATA);


void vRadio_PowerUp(void)
{
  SEGMENT_VARIABLE(wDelay,  U16, SEG_XDATA) = 0u;

  /* Hardware reset the chip */
  si446x_reset();

  /* Wait until reset timeout or Reset IT signal */
  for (; wDelay < pRadioConfiguration->Radio_Delay_Cnt_After_Reset; wDelay++);
}





U8 bRadio_Check_Tx_RX(void)
{

  if (RFnIRQ == FALSE)
  {
      /* Read ITs, clear pending ones */
      si446x_get_int_status(0u, 0u, 0u);

	  if (Si446xCmd.GET_INT_STATUS.CHIP_PEND & SI446X_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_BIT)
      {
      	/* State change to */
      	si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_SLEEP);
	
	  	/* Reset FIFO */
      	si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
      
	  	/* State change to */
        si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_RX);
      }

      if(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT)
      {
        return SI446X_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT;
      }

      if(Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
      {
        /* Packet RX */

        /* Get payload length */
        si446x_fifo_info(0x00);

        si446x_read_rx_fifo(Si446xCmd.FIFO_INFO.RX_FIFO_COUNT, &customRadioPacket[0]);

        return SI446X_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT;
      }
      
	  if (Si446xCmd.GET_INT_STATUS.PH_PEND & SI446X_CMD_GET_INT_STATUS_REP_CRC_ERROR_BIT)
      {
      	/* Reset FIFO */
      	si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);
      }


  }

  return 0;
}

/*!
 *  Set Radio to RX mode. .
 *
 *  @param channel Freq. Channel,  packetLength : 0 Packet handler fields are used , nonzero: only Field1 is used
 *
 *  @note
 *
 */
void vRadio_StartRX(U8 channel, U8 packetLenght )
{
  // Read ITs, clear pending ones
  si446x_get_int_status(0u, 0u, 0u);

   // Reset the Rx Fifo
   si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_RX_BIT);

  /* Start Receiving packet, channel 0, START immediately, Packet length used or not according to packetLength */
  si446x_start_rx(channel, 0u, packetLenght,
                  SI446X_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_NOCHANGE,
                  SI446X_CMD_START_RX_ARG_RXVALID_STATE_ENUM_READY,
                  SI446X_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
}

/*!
 *  Set Radio to TX mode, variable packet length.
 *
 *  @param channel Freq. Channel, Packet to be sent length of of the packet sent to TXFIFO
 *
 *  @note
 *
 */
void vRadio_StartTx_Variable_Packet(U8 channel, U8 *pioRadioPacket, U8 length)
{
  /* Leave RX state */
  si446x_change_state(SI446X_CMD_CHANGE_STATE_ARG_NEW_STATE_ENUM_READY);

  /* Read ITs, clear pending ones */
  si446x_get_int_status(0u, 0u, 0u);

  /* Reset the Tx Fifo */
  si446x_fifo_info(SI446X_CMD_FIFO_INFO_ARG_TX_BIT);

  /* Fill the TX fifo with datas */
  si446x_write_tx_fifo(length, pioRadioPacket);

  /* Start sending packet, channel 0, START immediately */
   si446x_start_tx(channel, 0x80, length);
 
}


void DemoApp_Pollhandler(void)
{
 u32  bMain_IT_Status = bRadio_Check_Tx_RX();	 //check INT  signal and Process

  switch (bMain_IT_Status)
  {
    case SI446X_CMD_GET_INT_STATUS_REP_PACKET_SENT_PEND_BIT:	  //Tx_End INT
		 vRadio_StartRX(0, pRadioConfiguration->Radio_PacketLength);
      break;

    case SI446X_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT:	 //Rx INT
			WirelessRx_handler(Rx_Buffer,PACKET_LENGTH); //Rx_buffer[] receive data
			//WirelessTx_handler(0,Rx_Buffer,PACKET_LENGTH); //Send back Rx_buffer[]' data to Master
		vRadio_StartRX(0, pRadioConfiguration->Radio_PacketLength);
      break;

    default:
      break;
  } /* switch */

}


void vRadio_Init(void)//初始化函数
{

  radio_hal_Init();
  /* Power Up the radio chip */
  vRadio_PowerUp();
  DelayMs(10);
  /* Load radio configuration */
  while (SI446X_SUCCESS != si446x_configuration_init(pRadioConfiguration->Radio_ConfigurationArray))
  {
    /* Power Up the radio chip */
    vRadio_PowerUp();
    DelayMs(10);
	
  }
  si446x_get_int_status(0u, 0u, 0u);
	vRadio_StartRX(0,0u);
	//WirelessTx_handler(0,"xzw168",6);
while(0){
	DemoApp_Pollhandler();
}
}



//vRadio_StartTx_Variable_Packet(0, data, 10);//发送数据
//WirelessRx_handler(Rx_Buffer,PACKET_LENGTH); //Rx_buffer[] receive data/




//例子
/*
int main(void)	
{
s32 i;
u8 data[100] = { 1 }, Write [10] = { 0 }, Read[ 10 ];
SetSysClockTo72();	
delay_init(72);
RCCINIT();
	
dingshiqi1shezhi();
	
chuankoushezhi(115200);

LEDINIT();


vRadio_Init();

rece_buf[ 0 ] = 0;

	while(1)
{
	

	chuankoufasong(205,data[ 0 ] );

	vRadio_StartTx_Variable_Packet(0, data, 32);
		delay_ms(10);
	 bRadio_Check_Tx_RX();
	
	chuankoufasong(203,ir);
	delay_ms(10);
	

	
	led = !led;

	




	chuankoufasong(204,ir);
	Write [0] = bRadio_Check_Tx_RX();
	if(Write [0] != 0)
	{
		if(Write [0] == 16)
		{
		WirelessRx_handler(data,10);
		chuankoufasong(207,Write [0]);
		vRadio_StartRX(0, 32);
		}
	}	
	chuankoufasong(206,ir);
	
}

*/
