/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */
 
#include "si446x.h"
#include "spi.h"
#include "radio_config_Si446x_1k_26M.h"//26m ����
//#include "radio_config_Si446x_1k_30M.h"  //30m ����


const uint8_t config_table_1k[] = RADIO_CONFIGURATION_DATA_ARRAY;

/*read a array of command response*/
void SI446X_READ_RESPONSE(uint8_t *buffer, uint8_t size);

/*wait the device ready to response a command*/
void SI446X_WAIT_CTS(void);

/*write data to TX fifo*/
void SI446X_W_TX_FIFO(uint8_t *txbuffer, uint8_t size);

/*Exchange a byte via the SPI bus*/
uint8_t SPI_ExchangeByte(uint8_t input) {
    return SPI3_ReadWriteByte(input);
}

void si446x_init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE , ENABLE);//ʹ��GPIOBʱ��

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
    
    GPIO_SetBits(GPIOE,GPIO_Pin_2);//��Ƶʱ�ӣ�����ʹ�ܣ��ߵ�ƽ�򿪣��͵�ƽ�ضϣ�
    GPIO_SetBits(GPIOE,GPIO_Pin_4);//��Ƶ PA_VDD ��Դʹ�����ţ��ߵ�ƽ�򿪣��͵�ƽ�ضϣ�
    RF_Initial(0);
}

/*===========================================================================
SI446X_WAIT_CTS();
Function : wait the device ready to response a command
============================================================================*/
void SI446X_WAIT_CTS(void)
{
    uint8_t cts;
    IWDG_ReloadCounter();
    do
    {
        SI_CSN_LOW();
        SPI_ExchangeByte(READ_CMD_BUFF);
        cts = SPI_ExchangeByte(0xFF);
        SI_CSN_HIGH();
    }while(cts != 0xFF);
    IWDG_ReloadCounter();
}

/*===========================================================================
SI446X_CMD();
Function : Send a command to the device
INTPUT   : cmd, the buffer stores the command array
           cmdsize, the size of the command array
============================================================================*/
void SI446X_CMD(uint8_t *cmd, uint8_t cmdsize)
{
    SI446X_WAIT_CTS();
    SI_CSN_LOW();
    
    while (cmdsize--)   { SPI_ExchangeByte(*cmd++); }
    
    SI_CSN_HIGH();
}

/*===========================================================================
SI446X_POWER_UP();
Function : Power up the device
INTPUT   : f_xtal, the frequency of the external high-speed crystal
============================================================================*/
void SI446X_POWER_UP(uint32_t f_xtal)
{
    uint8_t cmd[7];
    cmd[0] = POWER_UP;
    cmd[1] = 0x01;
    cmd[2] = 0x00;
    cmd[3] = f_xtal>>24;
    cmd[4] = f_xtal>>16;
    cmd[5] = f_xtal>>8;
    cmd[6] = f_xtal;
    SI446X_CMD(cmd, 7);
}
/*===========================================================================
SI446X_READ_RESPONSE();
Function : read a array of command response
INTPUT   : buffer,  a buffer, stores the data responsed
           size,    How many bytes should be read
============================================================================*/
void SI446X_READ_RESPONSE(uint8_t *buffer, uint8_t size)
{
    SI446X_WAIT_CTS();
    SI_CSN_LOW();
    
	SPI_ExchangeByte(READ_CMD_BUFF);
	while (size--)  { *buffer++ = SPI_ExchangeByte(0xFF); }
    
    SI_CSN_HIGH();
}

/*===========================================================================
SI446X_NOP();
Function : NO Operation command
============================================================================*/
uint8_t SI446X_NOP(void)
{
    uint8_t cts;
    SI_CSN_LOW();
    cts = SPI_ExchangeByte(NOP);
    SI_CSN_HIGH();
	return cts;
}

/*===========================================================================
SI446X_PART_INFO();
Function : Read the PART_INFO of the device, 8 bytes needed
INTPUT   : buffer, the buffer stores the part information
============================================================================*/
void SI446X_PART_INFO(uint8_t *buffer)
{
    uint8_t cmd = PART_INFO;

    SI446X_CMD(&cmd, 1);
    SI446X_READ_RESPONSE(buffer, 8);

}

/*===========================================================================
SI446X_FUNC_INFO();
Function : Read the FUNC_INFO of the device, 7 bytes needed
INTPUT   : buffer, the buffer stores the FUNC information
============================================================================*/
void SI446X_FUNC_INFO(uint8_t *buffer)
{
    uint8_t cmd = FUNC_INFO;

    SI446X_CMD(&cmd, 1);
    SI446X_READ_RESPONSE(buffer, 7);
}

/*===========================================================================
SI446X_INT_STATUS();
Function : Read the INT status of the device, 9 bytes needed
INTPUT   : buffer, the buffer stores the int status
============================================================================*/
void SI446X_INT_STATUS(uint8_t *buffer)
{
    uint8_t cmd[4];
    cmd[0] = GET_INT_STATUS;
    cmd[1] = 0;
    cmd[2] = 0;
    cmd[3] = 0;

    SI446X_CMD(cmd, 4);
    SI446X_READ_RESPONSE(buffer, 9);

}

/*===========================================================================
SI446X_GET_PROPERTY();
Function : Read the PROPERTY of the device
INTPUT   : buffer, the buffer stores the PROPERTY value
           GROUP_NUM, the group and number of the parameter
           NUM_GROUP, number of the group
============================================================================*/
void SI446X_GET_PROPERTY_X(SI446X_PROPERTY GROUP_NUM, uint8_t NUM_PROPS, 
                           uint8_t *buffer)
{
    uint8_t cmd[4];

    cmd[0] = GET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = NUM_PROPS;
    cmd[3] = GROUP_NUM;

    SI446X_CMD(cmd, 4);
    SI446X_READ_RESPONSE(buffer, NUM_PROPS + 1);
}
    
/*===========================================================================
SI446X_SET_PROPERTY_X();
Function : Set the PROPERTY of the device
INTPUT   : GROUP_NUM, the group and the number of the parameter
           NUM_GROUP, number of the group
           PAR_BUFF, buffer stores parameters
============================================================================*/
void SI446X_SET_PROPERTY_X(SI446X_PROPERTY GROUP_NUM, uint8_t NUM_PROPS, uint8_t *PAR_BUFF)
{
    uint8_t cmd[20], i = 0;
    if (NUM_PROPS >= 16)    { return; }
    cmd[i++] = SET_PROPERTY;
    cmd[i++] = GROUP_NUM>>8;
    cmd[i++] = NUM_PROPS;
    cmd[i++] = GROUP_NUM;
    while (NUM_PROPS--)     { cmd[i++] = *PAR_BUFF++; }
    SI446X_CMD(cmd, i);
}
    
/*===========================================================================
SI446X_SET_PROPERTY_1();
Function : Set the PROPERTY of the device, only 1 byte
INTPUT   : GROUP_NUM, the group and number index
           prioriry,  the value to be set
============================================================================*/
void SI446X_SET_PROPERTY_1(SI446X_PROPERTY GROUP_NUM, uint8_t proirity)
{
    uint8_t cmd[5];

    cmd[0] = SET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    cmd[4] = proirity;
    SI446X_CMD(cmd, 5);
}
    
/*===========================================================================
SI446X_GET_PROPERTY_1();
Function : Get the PROPERTY of the device, only 1 byte
INTPUT   : GROUP_NUM, the group and number index
OUTPUT   : the PROPERTY value read from device
============================================================================*/
uint8_t SI446X_GET_PROPERTY_1(SI446X_PROPERTY GROUP_NUM)
{
    uint8_t cmd[4];

    cmd[0] = GET_PROPERTY;
    cmd[1] = GROUP_NUM>>8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    SI446X_CMD(cmd, 4);
    SI446X_READ_RESPONSE(cmd, 2);
    return cmd[1];
}
    
/*===========================================================================
SI446X_RESET();
Function : reset the SI446x device
============================================================================*/
void SI446X_RESET(void)
{
    uint16_t x = 255;
    SI_SDN_HIGH();
    //while(x--); lhb
    delay_ms(50);
    SI_SDN_LOW();
    SI_CSN_HIGH();
}
    
/*===========================================================================
SI446X_CONFIG_INIT();
Function : configuration the device
============================================================================*/
void SI446X_CONFIG_INIT(void)
{
    uint8_t i;
    uint16_t j = 0;
    const uint8_t   *ptr = config_table_1k;


    while((i = *(ptr+j)) != 0)
    {
        j += 1;
        SI446X_CMD((uint8_t*)ptr + j, i);
        IWDG_ReloadCounter();
        j += i;
    }
#if PACKET_LENGTH > 0           //fixed packet length
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_LENGTH_7_0, PACKET_LENGTH);
#else                           //variable packet length
    SI446X_SET_PROPERTY_1(PKT_CONFIG1, 0x00);
    SI446X_SET_PROPERTY_1(PKT_CRC_CONFIG, 0x00);
    SI446X_SET_PROPERTY_1(PKT_LEN_FIELD_SOURCE, 0x01);
    SI446X_SET_PROPERTY_1(PKT_LEN, 0x2A);
    SI446X_SET_PROPERTY_1(PKT_LEN_ADJUST, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_LENGTH_12_8, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_LENGTH_7_0, 0x01);
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_CONFIG, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_1_CRC_CONFIG, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_2_LENGTH_12_8, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_2_LENGTH_7_0, 0x10);
    SI446X_SET_PROPERTY_1(PKT_FIELD_2_CONFIG, 0x00);
    SI446X_SET_PROPERTY_1(PKT_FIELD_2_CRC_CONFIG, 0x00);
#endif //PACKET_LENGTH

    //��Ҫ�� 4463��GDO2��GDO3������Ƶ���أ�
    //����ʱ���룺 GDO2=1  GDO3=0
    //����ʱ���룺 GDO2=0��GDO3=1
    //���ԣ�����������4463�Ǳ���ģ�4460��4461����Ҫ��Ƶ���أ������ȥ�������
    SI446X_GPIO_CONFIG(35, 24, 33|0x40, 32|0x40, 0, 0, 0);    //��Ҫ
    //SI446X_GPIO_CONFIG(35, 24, 0x72, 0x73, 0, 0, 0);
}
    
/*===========================================================================
SI446X_W_TX_FIFO();
Function : write data to TX fifo
INTPUT   : txbuffer, a buffer stores TX array
           size,  how many bytes should be written
============================================================================*/
void SI446X_W_TX_FIFO(uint8_t *txbuffer, uint8_t size)
{
    SI_CSN_LOW();
    SPI_ExchangeByte(WRITE_TX_FIFO);
    while (size--)      { SPI_ExchangeByte(*txbuffer++); }
    SI_CSN_HIGH();
}
    
/*===========================================================================
SI446X_SEND_PACKET();
Function : send a packet
INTPUT   : txbuffer, a buffer stores TX array
           size,  how many bytes should be written
           channel, tx channel
           condition, tx condition
============================================================================*/
void SI446X_SEND_PACKET(uint8_t *txbuffer, uint8_t size, uint8_t channel, uint8_t condition)
{
    uint8_t tx_len = size;

    SI446X_TX_FIFO_RESET();
	SI446X_RX_FIFO_RESET();

    SI446X_CHANGE_STATE(2);
    while(SI446X_GET_DEVICE_STATE() != 2);

	SI446X_WAIT_CTS();
    SI_CSN_LOW();
    SPI_ExchangeByte(WRITE_TX_FIFO);
#if PACKET_LENGTH == 0
    tx_len ++;
    SPI_ExchangeByte(size);
#endif
    while(size --)    { SPI_ExchangeByte(*txbuffer++); }
    SI_CSN_HIGH();

	SI446X_START_TX(channel, condition, tx_len);
}
    
/*===========================================================================
SI446X_START_TX();
Function : start TX command
INTPUT   : channel, tx channel
           condition, tx condition
           tx_len, how many bytes to be sent
============================================================================*/
void SI446X_START_TX(uint8_t channel, uint8_t condition, uint16_t tx_len)
{
    uint8_t cmd[6];

    SI446X_CHANGE_STATE(2);
    while (SI446X_GET_DEVICE_STATE() != 2);
    SI446X_CHANGE_STATE(5);
    while (SI446X_GET_DEVICE_STATE() != 5);

    cmd[0] = START_TX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = tx_len>>8;
    cmd[4] = tx_len;
    cmd[5] = 128;
    SI446X_CMD(cmd, 6);
}
    
/*===========================================================================
SI446X_READ_PACKET();
Function : read RX fifo
INTPUT   : buffer, a buffer to store data read
OUTPUT   : received bytes
============================================================================*/
uint8_t SI446X_READ_PACKET(uint8_t *buffer)
{
    uint8_t length, i;
    SI446X_WAIT_CTS();
    SI_CSN_LOW();

    SPI_ExchangeByte(READ_RX_FIFO);
#if PACKET_LENGTH == 0
    length = SPI_ExchangeByte(0xFF);
#else
    length = PACKET_LENGTH;
#endif
    i = length;

    while (length--)    { *buffer++ = SPI_ExchangeByte(0xFF); }
    SI_CSN_HIGH();
    return i;
}
    
/*===========================================================================
SI446X_START_RX();
Function : start RX state
INTPUT   : channel, receive channel
           condition, receive condition
           rx_len, how many bytes should be read
           n_state1, next state 1
           n_state2, next state 2
           n_state3, next state 3
============================================================================*/
void SI446X_START_RX(uint8_t channel, uint8_t condition, uint16_t rx_len,
                      uint8_t n_state1, uint8_t n_state2, uint8_t n_state3)
{
    uint8_t cmd[8];
    SI446X_RX_FIFO_RESET();
    SI446X_TX_FIFO_RESET();

    SI446X_CHANGE_STATE(2);
    while (SI446X_GET_DEVICE_STATE() != 2);
    SI446X_CHANGE_STATE(6);
    while (SI446X_GET_DEVICE_STATE() != 6);
    cmd[0] = START_RX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = rx_len>>8;
    cmd[4] = rx_len;
    cmd[5] = n_state1;
    cmd[6] = n_state2;
    cmd[7] = n_state3;
    SI446X_CMD(cmd, 8);
}
    
/*===========================================================================
SI446X_RX_FIFO_RESET();
Function : reset the RX FIFO of the device
============================================================================*/
void SI446X_RX_FIFO_RESET(void)
{
    uint8_t cmd[2];
    cmd[0] = FIFO_INFO;
    cmd[1] = 0x02;
    SI446X_CMD(cmd, 2);
}
    
/*===========================================================================
SI446X_TX_FIFO_RESET();
Function : reset the TX FIFO of the device
============================================================================*/
void SI446X_TX_FIFO_RESET(void)
{
    uint8_t cmd[2];

    cmd[0] = FIFO_INFO;
    cmd[1] = 0x01;
    SI446X_CMD(cmd, 2);
}
    
/*===========================================================================
SI446X_PKT_INFO();
Function : read packet information
INTPUT   : buffer, stores the read information
           FIELD, feild mask
           length, the packet length
           diff_len, diffrence packet length
============================================================================*/
void SI446X_PKT_INFO(uint8_t *buffer, uint8_t FIELD, uint16_t length, uint16_t diff_len)
{
    uint8_t cmd[6];
    cmd[0] = PACKET_INFO;
    cmd[1] = FIELD;
    cmd[2] = length >> 8;
    cmd[3] = length;
    cmd[4] = diff_len >> 8;
    cmd[5] = diff_len;

    SI446X_CMD(cmd, 6);
    SI446X_READ_RESPONSE(buffer, 3);
}
    
/*===========================================================================
SI446X_FIFO_INFO();
Function : read fifo information
INTPUT   : buffer, stores the read information
============================================================================*/
void SI446X_FIFO_INFO(uint8_t *buffer)
{
    uint8_t cmd[2];
    cmd[0] = FIFO_INFO;
    cmd[1] = 0x03;

    SI446X_CMD(cmd, 2);
    SI446X_READ_RESPONSE(buffer, 3);
}
    
/*===========================================================================
SI446X_GPIO_CONFIG();
Function : config the GPIOs, IRQ, SDO
============================================================================*/
void SI446X_GPIO_CONFIG(uint8_t G0, uint8_t G1, uint8_t G2, uint8_t G3,
                        uint8_t IRQ, uint8_t SDO, uint8_t GEN_CONFIG)
{
    uint8_t cmd[10];
    cmd[0] = GPIO_PIN_CFG;
    cmd[1] = G0;
    cmd[2] = G1;
    cmd[3] = G2;
    cmd[4] = G3;
    cmd[5] = IRQ;
    cmd[6] = SDO;
    cmd[7] = GEN_CONFIG;
    SI446X_CMD(cmd, 8);
    SI446X_READ_RESPONSE(cmd, 8);
}
/*===========================================================================
SI446X_CHANGE_STATE();
Function : change a new state
INTPUT   : NewState, input the new state
============================================================================*/
void SI446X_CHANGE_STATE(uint8_t NewState)
{
    uint8_t cmd[2];
    cmd[0] = 0x34;
    cmd[1] = NewState;
    SI446X_CMD(cmd, 2);
}
/*===========================================================================
SI446X_CHIP_STATUS();
Function : Get the chip status
INTPUT   : buffer_8, a buffer to store the read date
============================================================================*/
void SI446X_CHIP_STATUS(uint8_t* buffer_8)
{
    uint8_t cmd[2];
    cmd[0] = GET_CHIP_STATUS;
    cmd[1] = 0xFF;
    SI446X_CMD(cmd, 8);
    SI446X_READ_RESPONSE(buffer_8, 4);
}
/*===========================================================================
SI446X_SET_SYNC();
Function : Set the SYNC bytes of the device
INTPUT   : sync, the SYNC word
============================================================================*/
void SI446X_SET_SYNC(uint32_t sync)
{
    SI446X_SET_PROPERTY_1(SYNC_BITS_31_24, sync>>24);
    SI446X_SET_PROPERTY_1(SYNC_BITS_23_16, sync>>16);
    SI446X_SET_PROPERTY_1(SYNC_BITS_15_8,  sync>>8);
    SI446X_SET_PROPERTY_1(SYNC_BITS_7_0,   sync   );
}
    
/*===========================================================================
SI446X_SET_POWER();
Function : Set the PA Power of the device
INTPUT   : Power_Level,  0- 7F
============================================================================*/
void SI446X_SET_POWER(uint8_t Power_Level)
{
    SI446X_SET_PROPERTY_1(PA_PWR_LVL, Power_Level);
}

uint8_t SI446X_GET_DEVICE_STATE(void)
{
   uint8_t cmd[3];

   cmd[0] = REQUEST_DEVICE_STATE;
   SI446X_CMD(cmd, 1);
   SI446X_READ_RESPONSE(cmd, 3);
   return cmd[1] & 0x0F;
}


#define TX              1       // ����ģʽ
#define RX              0       // ����ģʽ
    
#define ACK_LENGTH      10      // Ӧ���źų���   
#define SEND_LENGTH     10      // ��������ÿ���ĳ���

uint16_t  RecvCnt = 0;            // �������յ����ݰ���                
// ��ҪӦ�������
uint8_t   AckBuffer[ACK_LENGTH] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

void RF_Initial(uint8_t mode)
{
    SI446X_RESET();         // SI446X ģ�鸴λ
    SI446X_CONFIG_INIT();   // �Ĵ�����ʼ�����Ĵ�������WDS���õ�ͷ�ļ�
    SI446X_SET_POWER(0x7F); // �������������Ϊ���
    SI446X_START_RX(0, 0, PACKET_LENGTH, 8, 8, 8);  // �������ģʽ               
}

void rf_recv(void) {
    uint8_t error=0, i=0, length=0, recv_buffer[65]={ 0 };
    
    SI446X_INT_STATUS(recv_buffer);     // ����Ƿ��յ�һ�����ݰ�
    
    // �յ�һ�����ݰ�����תLED
    if (recv_buffer[3] & (1<<4))
    {       
        length = SI446X_READ_PACKET(recv_buffer);

        // �ж������Ƿ����󣬽��յ����ź�Ӧ��Ϊ0-9
        for (i=0, error=0; i<10; i++)
        {
            if (recv_buffer[i] != i)    { error=1; break; } // ���ݳ���
        }

        if ((length==10) && (error==0))                     // ������ȷ
        {
//            LED0_TOG();// LED��˸������ָʾ�յ���ȷ����
//            USART_Send("Receive ok\r\n", 12);
//            DelayMs(10);

            // ����Ӧ���ź�,Ӧ������Ϊ10-19
            SI446X_SEND_PACKET(AckBuffer, ACK_LENGTH, 0, 0);
            do
            {
                SI446X_INT_STATUS(recv_buffer);
            }while (!(recv_buffer[3] & (1<<5)));    //�ȴ�������ɣ��жϲ�����
            
            RecvCnt++; 
        }
        
        //�ص�����ģʽ�������ȴ���һ������
        SI446X_START_RX(0, 0, PACKET_LENGTH,8, 8, 8);
    }   
}
