/* 引用头文件 *****************************************************************/
#include "outsignal.h"
/* 私有数据类型 ***************************************************************/
/* 私有常数宏 *****************************************************************/
/* 私有动作宏 *****************************************************************/
#define ADC1_DR_Address    0x40012440
/* 私有变量 *******************************************************************/
/* 全局变量 */
/* 私有函数声明 ***************************************************************/
/* 私有函数  ******************************************************************/
static void OutsignalClock(GPIO_TypeDef* port){
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

void OutsignedInit(Stdoutsignal_n* outsignal) {
    GPIO_InitTypeDef    GPIO_InitStructure;
//    ADC_InitTypeDef     ADC_InitStructure;
//    DMA_InitTypeDef     DMA_InitStructure;
    
    OutsignalClock(outsignal->DO1.port);
    OutsignalClock(outsignal->DO2.port);
    OutsignalClock(outsignal->DO3.port);
    OutsignalClock(outsignal->DO4.port);
    OutsignalClock(outsignal->DO5.port);
    OutsignalClock(outsignal->DO6.port);
    OutsignalClock(outsignal->DO7.port);
    OutsignalClock(outsignal->DO8.port);
    
    OutsignalClock(outsignal->urgent.port);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    GPIO_InitStructure.GPIO_Pin = outsignal->DO1.pin;
    GPIO_Init(outsignal->DO1.port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = outsignal->DO2.pin;
    GPIO_Init(outsignal->DO2.port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = outsignal->DO3.pin;
    GPIO_Init(outsignal->DO3.port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = outsignal->DO4.pin;
    GPIO_Init(outsignal->DO4.port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = outsignal->DO5.pin;
    GPIO_Init(outsignal->DO5.port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = outsignal->DO6.pin;
    GPIO_Init(outsignal->DO6.port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = outsignal->DO7.pin;
    GPIO_Init(outsignal->DO7.port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = outsignal->DO8.pin;
    GPIO_Init(outsignal->DO8.port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    
    GPIO_InitStructure.GPIO_Pin = outsignal->urgent.pin;
    GPIO_Init(outsignal->urgent.port, &GPIO_InitStructure);
    
//    
//    /* ADC1 DeInit */  
//    ADC_DeInit(ADC1);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
//    
//    /* Initialize ADC structure */
//    ADC_StructInit(&ADC_InitStructure);
//
//    /* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
//    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
//    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
//    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
//    ADC_Init(ADC1, &ADC_InitStructure); 
//    ADC_ChannelConfig(ADC1, ADC_Channel_5 , ADC_SampleTime_55_5Cycles); 
//    /* Convert the ADC1 temperature sensor  with 55.5 Cycles as sampling time */ 
//    ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor , ADC_SampleTime_55_5Cycles);  
//    ADC_TempSensorCmd(ENABLE);
//
//    /* Convert the ADC1 Vref  with 55.5 Cycles as sampling time */ 
//    ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint , ADC_SampleTime_55_5Cycles); 
//    ADC_VrefintCmd(ENABLE);
//
//    /* Convert the ADC1 Vbat with 55.5 Cycles as sampling time */ 
//    ADC_ChannelConfig(ADC1, ADC_Channel_Vbat , ADC_SampleTime_55_5Cycles);  
//    ADC_VbatCmd(ENABLE);
//
//    /* ADC Calibration */
//    ADC_GetCalibrationFactor(ADC1);
//
//    /* ADC DMA request in circular mode */
//    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
//
//    /* Enable ADC_DMA */
//    ADC_DMACmd(ADC1, ENABLE);  
//
//    /* Enable the ADC peripheral */
//    ADC_Cmd(ADC1, ENABLE);     
//
//    /* Wait the ADRDY flag */
//    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
//
//    /* ADC1 regular Software Start Conv */ 
//    ADC_StartOfConversion(ADC1);
//    
//    /* DMA1 clock enable */
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
//
//    /* DMA1 Channel1 Config */
//    DMA_DeInit(DMA1_Channel1);
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
//    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)insignal->Data;
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//    DMA_InitStructure.DMA_BufferSize = 4;
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
//    /* DMA1 Channel1 enable */
//    DMA_Cmd(DMA1_Channel1, ENABLE);
}

void OutsignalSetout(Stdoutsignal_n* outsignal,uint8_t num,uint8_t var) {
    Stdoutsignal_pin_TypeDef out_pin;
    switch(num) {
        case 0:
            out_pin = outsignal->DO1;
        break;
        case 1:
            out_pin = outsignal->DO2;
        break;
        case 2:
            out_pin = outsignal->DO3;
        break;
        case 3:
            out_pin = outsignal->DO4;
        break;
        case 4:
            out_pin = outsignal->DO5;
        break;
        case 5:
            out_pin = outsignal->DO6;
        break;
        case 6:
            out_pin = outsignal->DO7;
        break;
        case 7:
            out_pin = outsignal->DO8;
        break;
        case 8:
            outsignal->all_pin_var = var;
            out_pin = outsignal->DO1;
            GPIO_WriteBit(out_pin.port,out_pin.pin,(BitAction)(var));
            out_pin = outsignal->DO2;
            GPIO_WriteBit(out_pin.port,out_pin.pin,(BitAction)(var));
            out_pin = outsignal->DO3;
            GPIO_WriteBit(out_pin.port,out_pin.pin,(BitAction)(var));
            out_pin = outsignal->DO4;
            GPIO_WriteBit(out_pin.port,out_pin.pin,(BitAction)(var));
            out_pin = outsignal->DO5;
            GPIO_WriteBit(out_pin.port,out_pin.pin,(BitAction)(var));
            out_pin = outsignal->DO6;
            GPIO_WriteBit(out_pin.port,out_pin.pin,(BitAction)(var));
            out_pin = outsignal->DO7;
            GPIO_WriteBit(out_pin.port,out_pin.pin,(BitAction)(var));
            out_pin = outsignal->DO8;
            break;
    }
    if(num < 8) {
        if(var == 0) {
            outsignal->coil_val &= ~(1 << num);
        } else {
            outsignal->coil_val |= (1 << num);
        }
    } else {
        if(var == 0) {
            outsignal->coil_val = 0x00;
        } else {
            outsignal->coil_val = 0xff;
        }
    }
    GPIO_WriteBit(out_pin.port,out_pin.pin,(BitAction)(var));
}

uint8_t OutsignalReadout(Stdoutsignal_n* outsignal,uint8_t num) {
    Stdoutsignal_pin_TypeDef out_pin;
    switch(num) {
        case 0:
            out_pin = outsignal->DO1;
        break;
        case 1:
            out_pin = outsignal->DO2;
        break;
        case 2:
            out_pin = outsignal->DO3;
        break;
        case 3:
            out_pin = outsignal->DO4;
        break;
        case 4:
            out_pin = outsignal->DO5;
        break;
        case 5:
            out_pin = outsignal->DO6;
        break;
        case 6:
            out_pin = outsignal->DO7;
        break;
        case 7:
            out_pin = outsignal->DO8;
        break;
        case 8:
            return outsignal->all_pin_var;
        break;
    }
    return GPIO_ReadOutputDataBit(out_pin.port,out_pin.pin);
}

uint8_t OutsignalEmergencyStop(Stdoutsignal_n* outsignal) {
    return GPIO_ReadInputDataBit(outsignal->urgent.port,outsignal->urgent.pin);
}

/***************************************************************END OF FILE****/
