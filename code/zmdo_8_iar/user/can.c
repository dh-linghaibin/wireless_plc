/* 引用头文件 *****************************************************************/
#include "can.h"
#include "ringbuf.h"
/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
/* 私有动作宏 *****************************************************************/
/* 私有变量 *******************************************************************/
/* 全局变量 */
uint8_t can_package_merry[80] = {0};
/** -- 环形队列初始化 -- by lhb_steven -- 2017/6/30**/
Stdringbuf RINGBUF = {
    {
        0
    },
    &RingbufInit,
    &RingbufPut,
    &RingbufGut
};

static uint8_t can_id = 0;

/* 私有函数声明 ***************************************************************/
/* 私有函数  ******************************************************************/

static void CanClock(GPIO_TypeDef* port){
    if (port == GPIOA)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    else if(port == GPIOB)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    else if(port == GPIOC)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    else if(port == GPIOD)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
    else if(port == GPIOE)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
    else if(port == GPIOF)
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
}
void CanInit(Stdcan_n* can) {
    GPIO_InitTypeDef                    GPIO_InitStructure;
    CAN_InitTypeDef                     CAN_InitStructure;
    CAN_FilterInitTypeDef               CAN_FilterInitStructure;
    NVIC_InitTypeDef                    NVIC_InitStructure;
    
    RINGBUF.init(&RINGBUF.ringbuf_n, can_package_merry, 80);
    can_id = can->id;
    
    
    CanClock(can->s.port);
    GPIO_InitStructure.GPIO_Pin = can->s.pin;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(can->s.port, &GPIO_InitStructure);
    GPIO_ResetBits(can->s.port,can->s.pin);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    /* Enable GPIO clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    /* Enable CAN clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);
    
    /* Connect the involved CAN pins to AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_4);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_4);
    
    /* Configure CAN RX and TX pins */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
     //CAN中断使能和优先级设置
    NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;         
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
        
    //CAN单元设置
    CAN_InitStructure.CAN_TTCM = DISABLE; 
    CAN_InitStructure.CAN_ABOM = DISABLE;  
    CAN_InitStructure.CAN_AWUM = DISABLE; 
    CAN_InitStructure.CAN_NART = ENABLE; 
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//CAN_Mode_Normal; 
    //CAN波特率设置,500KHz@48MHz
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    switch(can->btl) {
        case B_250K:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_7tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
            CAN_InitStructure.CAN_Prescaler = 12;
        break;
        case B_125K:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq;
            CAN_InitStructure.CAN_Prescaler = 96;
        break;
        case B_50K:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
            CAN_InitStructure.CAN_Prescaler = 160;
        break;
        case B_20K:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
            CAN_InitStructure.CAN_Prescaler = 240;
        break;
        case B_10K:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
            CAN_InitStructure.CAN_Prescaler = 600;
        break;
        case B_5K:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
            CAN_InitStructure.CAN_Prescaler = 686;
        break;
    }
    CAN_Init(CAN, &CAN_InitStructure);
    
    //配置过滤器
    CAN_FilterInitStructure.CAN_FilterNumber=0;      //过滤器0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
    CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
    
    //使能CAN中断
    CAN_ClearITPendingBit(CAN, CAN_IT_FF0);
    CAN_ITConfig(CAN, CAN_IT_TME, DISABLE);
    CAN_ITConfig(CAN, CAN_IT_FMP0, ENABLE);
    CAN_ITConfig(CAN, CAN_IT_FF0, ENABLE);
}

void CanSend(struct canbus* can) {
    CanTxMsg transmit_message;
	transmit_message.StdId = can->send_msg.data.StdId;
	transmit_message.RTR   = CAN_RTR_DATA;
	transmit_message.IDE   = CAN_ID_STD;
    transmit_message.DLC   = can->send_msg.data.DLC;
    
	transmit_message.Data[0] = can->send_msg.data.Data[0];
    transmit_message.Data[1] = can->send_msg.data.Data[1];
    transmit_message.Data[2] = can->send_msg.data.Data[2];
    transmit_message.Data[3] = can->send_msg.data.Data[3];
    transmit_message.Data[4] = can->send_msg.data.Data[4];
    transmit_message.Data[5] = can->send_msg.data.Data[5];
    transmit_message.Data[6] = can->send_msg.data.Data[6];
	transmit_message.Data[7] = can->send_msg.data.Data[7];
    
	uint8_t TransmitMailbox = CAN_Transmit(CAN, &transmit_message); 
    uint8_t i = 0;
    while((CAN_TransmitStatus(CAN, TransmitMailbox) != CANTXOK) && (i != 0xFF)) {
        i++;
    }
}

void CanSetID(Stdcan_n* can,uint8_t id) {
    can->id = id;
    can_id = can->id;
}

void CanSetBlt(Stdcan_n* can,btl_e btl) {
    CAN_InitTypeDef                     CAN_InitStructure;
    //CAN单元设置
    CAN_InitStructure.CAN_TTCM = DISABLE; 
    CAN_InitStructure.CAN_ABOM = DISABLE;  
    CAN_InitStructure.CAN_AWUM = DISABLE; 
    CAN_InitStructure.CAN_NART = ENABLE; 
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//CAN_Mode_Normal; 
    //CAN波特率设置,500KHz@48MHz
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    can->btl = btl;
    switch(can->btl) {
        case B_250K:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_7tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
            CAN_InitStructure.CAN_Prescaler = 12;
        break;
        case B_125K:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq;
            CAN_InitStructure.CAN_Prescaler = 96;
        break;
        case B_50K:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
            CAN_InitStructure.CAN_Prescaler = 160;
        break;
        case B_20K:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
            CAN_InitStructure.CAN_Prescaler = 240;
        break;
        case B_10K:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
            CAN_InitStructure.CAN_Prescaler = 600;
        break;
        case B_5K:
            CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
            CAN_InitStructure.CAN_Prescaler = 686;
        break;
    }
    CAN_Init(CAN, &CAN_InitStructure);
}

static can_package_obj can_rx_package = {
	.package = {0,},
};

can_package_obj*  bxcan_get_packget(Stdcan_n* can) {
	return &can_rx_package;
}

/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
void CEC_CAN_IRQHandler(void) {
    CanRxMsg RxMessage_data = {0};
    CAN_Receive(CAN, CAN_FIFO0, &RxMessage_data);
    if( (RxMessage_data.StdId == RADIO) || (RxMessage_data.StdId == can_id) )  {
        for(int i = 0;i < PACKAGE_NUM;i++) {
            if(can_rx_package.package[i].flag == F_NO_USE) {
                can_rx_package.package[i].id = RxMessage_data.StdId;
                for(int j = 0;j < 8;j++) {
                    can_rx_package.package[i].dat[j] = RxMessage_data.Data[j];
                }
                can_rx_package.package[i].flag  = F_USE;
                break;
            }
        }
    }
}

/***************************************************************END OF FILE****/
