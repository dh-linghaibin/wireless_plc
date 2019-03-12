
/* ����ͷ�ļ� *****************************************************************/
#include "usart.h"
/* ˽���������� ***************************************************************/
/* ˽�г����� *****************************************************************/
/* ˽�ж����� *****************************************************************/
/* ˽�б��� *******************************************************************/
/* ȫ�ֱ��� */
/* ˽�к������� ***************************************************************/
/* ˽�к���  ******************************************************************/
#define USART_DR_Base 0x40013828
#define SENDBUFF_SIZE 500
uint8_t Tx_Buffer[SENDBUFF_SIZE];
uint16_t i;

void NVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;//DMA1_Channel4_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_Init(&NVIC_InitStructure);
}
/*����DMA*/
void DMA_Config()
{
    DMA_InitTypeDef DMA_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /*����DMA�ж�����*/
    NVIC_Config();
    /*�����С*/
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
    /*���򣺴��ڴ浽����*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    /*��ֹ�ڴ浽�ڴ�Ĵ���*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    /*�ڴ��ַ��Ҫ���������ָ��*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Tx_Buffer;
    /*�ڴ����ݵ�λ8bit*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    /*�ڴ��ַ����*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /*DMAģʽ��һ�δ���*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    /*����DMAԴ���������ݼĴ�����ַ*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_Base;
    /*�������ݵ�λ*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    /*�����ַ����*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /*���ȼ�����*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    /*����DMA1��2ͨ��*/
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);
    /*ʹ��DMA*/
    DMA_Cmd(DMA1_Channel2, ENABLE);
    /*����DMA������ɺ�����ж�*/
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
    
}


/*���ڳ�ʼ������ */
void USART_Config(void)
{
    USART_InitTypeDef USART_InitStructure;
    /*��ʼ��USART1ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_InitStructure.USART_BaudRate = 9600;//���ô��ڲ�����
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����������
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//���ù���ģʽ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����Ч��λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//����ֹͣλ
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//��������λ
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);//ʹ�ܴ��� 1
}
/*GPIOA��ʼ������ */
void GPIOA_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*��ʼ��GPIOAʱ��*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);
    /* ����PA9 ,PA10*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ö˿ڸ���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/** -- ���ڳ�ʼ�� -- by lhb_steven -- 2017/6/14**/
void UsartInit(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE );

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);        
    /*
    *  USART1_TX -> PA9 , USART1_RX ->        PA3
    */                                
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;                 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);        

    USART_InitStructure.USART_BaudRate = 9600;//���ô��ڲ�����115200
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//��������λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//����ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����Ч��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���ù���ģʽ
    USART_Init(USART2, &USART_InitStructure); //������ṹ��

    USART_Cmd(USART2, ENABLE);//ʹ�ܴ���1
}

/** -- ����һ���ֽ� -- by lhb_steven -- 2017/6/14**/
void UsartSendByte(uint8_t byte) {
    while(!((USART2->ISR)&(1<<7)));
    USART2->TDR=byte;	
}

/** -- �����ַ��� -- by lhb_steven -- 2017/6/14**/
void UsartSend(uint8_t *Buffer, uint32_t Length) {
    while(Length != 0) {
        while(!((USART2->ISR)&(1<<7)));//�ȴ�������
        USART2->TDR= *Buffer;
        Buffer++;
        Length--;
    }
}

/** -- ��ȡ���յ������� -- by lhb_steven -- 2017/6/14**/
uint8_t UART_Recive(void) {	
	while(!(USART2->ISR & (1<<5)));//�ȴ����յ�����
	return(USART2->RDR);   //��������
}
/***************************************************************END OF FILE****/
