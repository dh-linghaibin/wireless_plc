
/* 引用头文件 *****************************************************************/
#include "usart.h"
/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
/* 私有动作宏 *****************************************************************/
/* 私有变量 *******************************************************************/
/* 全局变量 */
/* 私有函数声明 ***************************************************************/
/* 私有函数  ******************************************************************/
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
/*配置DMA*/
void DMA_Config()
{
    DMA_InitTypeDef DMA_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /*配置DMA中断配置*/
    NVIC_Config();
    /*传输大小*/
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
    /*方向：从内存到外设*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    /*禁止内存到内存的传输*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    /*内存地址：要传输变量的指针*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Tx_Buffer;
    /*内存数据单位8bit*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    /*内存地址自增*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /*DMA模式：一次传输*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    /*设置DMA源：串口数据寄存器地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_Base;
    /*外设数据单位*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    /*外设地址不增*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /*优先级：中*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    /*配置DMA1的2通道*/
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);
    /*使能DMA*/
    DMA_Cmd(DMA1_Channel2, ENABLE);
    /*配置DMA发送完成后产生中断*/
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
    
}


/*串口初始化配置 */
void USART_Config(void)
{
    USART_InitTypeDef USART_InitStructure;
    /*初始化USART1时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_InitStructure.USART_BaudRate = 9600;//设置串口波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//设置流控制
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//设置工作模式
    USART_InitStructure.USART_Parity = USART_Parity_No;//设置效验位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//设置停止位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//设置数据位
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);//使能串口 1
}
/*GPIOA初始化配置 */
void GPIOA_Config()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*初始化GPIOA时钟*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);
    /* 配置PA9 ,PA10*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //设置端口复用
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/** -- 串口初始化 -- by lhb_steven -- 2017/6/14**/
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

    USART_InitStructure.USART_BaudRate = 9600;//设置串口波特率115200
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//设置数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//设置停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//设置效验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//设置流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//设置工作模式
    USART_Init(USART2, &USART_InitStructure); //配置入结构体

    USART_Cmd(USART2, ENABLE);//使能串口1
}

/** -- 发送一个字节 -- by lhb_steven -- 2017/6/14**/
void UsartSendByte(uint8_t byte) {
    while(!((USART2->ISR)&(1<<7)));
    USART2->TDR=byte;	
}

/** -- 发送字符串 -- by lhb_steven -- 2017/6/14**/
void UsartSend(uint8_t *Buffer, uint32_t Length) {
    while(Length != 0) {
        while(!((USART2->ISR)&(1<<7)));//等待发送完
        USART2->TDR= *Buffer;
        Buffer++;
        Length--;
    }
}

/** -- 读取接收到的数据 -- by lhb_steven -- 2017/6/14**/
uint8_t UART_Recive(void) {	
	while(!(USART2->ISR & (1<<5)));//等待接收到数据
	return(USART2->RDR);   //读出数据
}
/***************************************************************END OF FILE****/
