

#include "radio.h"



#if 1//(defined SILABS_RADIO_SI446X) || (defined SILABS_RADIO_SI4455)
BIT ctsWentHigh = 0;
#endif

#if 1//(defined SILABS_RADIO_SI446X) || (defined SILABS_RADIO_SI4455)

/*!
 * Gets a command response from the radio chip
 *
 * @param byteCount     Number of bytes to get from the radio chip
 * @param pData         Pointer to where to put the data
 *
 * @return CTS value
 */
U8 radio_comm_GetResp(U8 byteCount, U8* pData)
{
  SEGMENT_VARIABLE(ctsVal = 0u, U8, SEG_DATA);
  SEGMENT_VARIABLE(errCnt = RADIO_CTS_TIMEOUT, U16, SEG_DATA);
 
  while (errCnt != 0)      //wait until radio IC is ready with the data
  {
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(0x44);    //read CMD buffer
	//   radio_hal_SpiWriteByte(0x01);
	//   radio_hal_SpiWriteByte(0x01);

	
	
    ctsVal = radio_hal_SpiReadByte();
	 
	 

	
	  //////////////////////////////问题点  johnson
    if (ctsVal == 0xFF)
    {

	  
      if (byteCount)
      {
        radio_hal_SpiReadData(byteCount, pData);
      }
      radio_hal_SetNsel();

	
      break;
    }

    radio_hal_SetNsel();
    errCnt--;
  }

 

  if (errCnt == 0)
  {
 	
    while(1)
    {
      /* ERROR!!!!  CTS should never take this long. */
      #ifdef RADIO_COMM_ERROR_CALLBACK	
        RADIO_COMM_ERROR_CALLBACK();
      #endif
    }
  }

  
  if (ctsVal == 0xFF)
  {
    ctsWentHigh = 1;
  }

  return ctsVal;
}

/*!
 * Sends a command to the radio chip
 *
 * @param byteCount     Number of bytes in the command to send to the radio device
 * @param pData         Pointer to the command to send.
 */
void radio_comm_SendCmd(U8 byteCount, U8* pData)
{
    /* There was a bug in A1 hardware that will not handle 1 byte commands. 
       It was supposedly fixed in B0 but the fix didn't make it at the last minute, so here we go again */
    if (byteCount == 1)
        byteCount++;
    while (!ctsWentHigh)
    {
		
        radio_comm_PollCTS();
	
    }
	
    radio_hal_ClearNsel();
    radio_hal_SpiWriteData(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;

	
}

/*!
 * Gets a command response from the radio chip
 *
 * @param cmd           Command ID
 * @param pollCts       Set to poll CTS
 * @param byteCount     Number of bytes to get from the radio chip.
 * @param pData         Pointer to where to put the data.
 */
void radio_comm_ReadData(U8 cmd, BIT pollCts, U8 byteCount, U8* pData)
{
    if(pollCts)
    {
        while(!ctsWentHigh)
        {
            radio_comm_PollCTS();
        }
    }
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(cmd);
    radio_hal_SpiReadData(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}


/*!
 * Gets a command response from the radio chip
 *
 * @param cmd           Command ID
 * @param pollCts       Set to poll CTS
 * @param byteCount     Number of bytes to get from the radio chip
 * @param pData         Pointer to where to put the data
 */
void radio_comm_WriteData(U8 cmd, BIT pollCts, U8 byteCount, U8* pData)
{
    if(pollCts)
    {
        while(!ctsWentHigh)
        {
            radio_comm_PollCTS();
        }
    }
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(cmd);
    radio_hal_SpiWriteData(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}

/*!
 * Waits for CTS to be high
 *
 * @return CTS value
 */
U8 radio_comm_PollCTS(void)
{
#ifdef RADIO_USER_CFG_USE_GPIO1_FOR_CTS
    while(!radio_hal_Gpio1Level())
    {
        /* Wait...*/
    }
    ctsWentHigh = 1;
    return 0xFF;
#else
	
    return radio_comm_GetResp(0, 0);
#endif
}

/**
 * Clears the CTS state variable.
 */
void radio_comm_ClearCTS()
{
  ctsWentHigh = 0;
}

#elif (defined SILABS_RADIO_SI4012)

/*!
 * Gets a command response from the radio chip
 *
 * @param byteCount     Number of bytes to get from the radio chip
 * @param pData         Pointer to where to put the data
 *
 * @return CTS value
 */
U8 radio_comm_GetResp(U8 byteCount, U8* pData)
{
  SEGMENT_VARIABLE(ctsVal = 0u, U8, SEG_DATA);

  if (qSmbus_SMBusRead(SI4012_SMBUS_ADDRESS, byteCount, pData) != \
                                                          SMBUS_RX_FINISHED) {
    return FALSE;
  }

  if (pData[0] == 0x80) {
    return TRUE;
  }

  return FALSE;
}

/*!
 * Sends a command to the radio chip
 *
 * @param byteCount     Number of bytes in the command to send to the radio device
 * @param pData         Pointer to the command to send.
 */
U8 radio_comm_SendCmd(U8 byteCount, U8* pData)
{
  if (qSmbus_SMBusWrite(SI4012_SMBUS_ADDRESS, byteCount, pData) != \
                                                      SMBUS_TRANSMISSION_OK) {
    return FALSE;
  }

  return TRUE;
}

#endif

/*!
 * 发送命令到射频芯片，得到响应
 *
 * @param cmdByteCount 命令字节发送到无线电设备的数量
 * @param pCmdData     指针，指向命令数据
 * @param respByteCount 获取响应中字节数
 * @param pRespData     响应数据指针
 *
 * @return CTS value
 */
U8 radio_comm_SendCmdGetResp(U8 cmdByteCount, U8* pCmdData, U8 respByteCount, U8* pRespData)
{
	
    radio_comm_SendCmd(cmdByteCount, pCmdData);
	//卡死 20131115 14:41

    return radio_comm_GetResp(respByteCount, pRespData);
}

