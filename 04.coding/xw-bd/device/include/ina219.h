/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#ifndef _INA219_H_
#define _INA219_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h" 
#include "stdbool.h"
#define INA219_ADDRESS  (0x40 << 1)     // A0 = GND, A1 = GND

// �Ĵ�����ַ
#define INA219_REG_CONFIG                       (u8)(0x00)      // ģʽ���üĴ��� (R/W)
#define INA219_REG_SHUNTVOLTAGE                 (u8)(0x01)      // ���������ѹ�Ĵ��� (R)
#define INA219_REG_BUSVOLTAGE                   (u8)(0x02)      // ���ߵ�ѹ�Ĵ��� (R)
#define INA219_REG_POWER                        (u8)(0x03)      // ���ʼĴ��� (R)
#define INA219_REG_CURRENT                      (u8)(0x04)      // �����Ĵ��� (R)
#define INA219_REG_CALIBRATION                  (u8)(0x05)      // ��׼ֵ�Ĵ��� (R/W)

//�Ĵ�����0x01����������
#define INA219_CONFIG_RESET                    (0x8000)  // Reset Bit

#define INA219_CONFIG_BVOLTAGERANGE_MASK       (0x2000)  // Bus Voltage Range Mask
#define INA219_CONFIG_BVOLTAGERANGE_16V        (0x0000)  // 0-16V Range
#define INA219_CONFIG_BVOLTAGERANGE_32V        (0x2000)  // 0-32V Range

#define INA219_CONFIG_GAIN_MASK                (0x1800)  // Gain Mask
#define INA219_CONFIG_GAIN_1_40MV              (0x0000)  // Gain 1, 40mV Range        //���ü������ϵ�������ѹ
#define INA219_CONFIG_GAIN_2_80MV              (0x0800)  // Gain 2, 80mV Range
#define INA219_CONFIG_GAIN_4_160MV             (0x1000)  // Gain 4, 160mV Range
#define INA219_CONFIG_GAIN_8_320MV             (0x1800)  // Gain 8, 320mV Range

#define INA219_CONFIG_BADCRES_MASK             (0x0780)  // Bus ADC Resolution Mask
#define INA219_CONFIG_BADCRES_9BIT             (0x0080)  // 9-bit bus res = 0..511
#define INA219_CONFIG_BADCRES_10BIT            (0x0100)  // 10-bit bus res = 0..1023
#define INA219_CONFIG_BADCRES_11BIT            (0x0200)  // 11-bit bus res = 0..2047
#define INA219_CONFIG_BADCRES_12BIT            (0x0400)  // 12-bit bus res = 0..4097

#define INA219_CONFIG_SADCRES_MASK             (0x0078)  // Shunt ADC Resolution and Averaging Mask
#define INA219_CONFIG_SADCRES_9BIT_1S_84US     (0x0000)  // 1 x 9-bit shunt sample
#define INA219_CONFIG_SADCRES_10BIT_1S_148US   (0x0008)  // 1 x 10-bit shunt sample
#define INA219_CONFIG_SADCRES_11BIT_1S_276US   (0x0010)  // 1 x 11-bit shunt sample
#define INA219_CONFIG_SADCRES_12BIT_1S_532US   (0x0018)  // 1 x 12-bit shunt sample
#define INA219_CONFIG_SADCRES_12BIT_2S_1060US  (0x0048)     // 2 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_4S_2130US  (0x0050)  // 4 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_8S_4260US  (0x0058)  // 8 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_16S_8510US (0x0060)  // 16 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_32S_17MS   (0x0068)  // 32 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_64S_34MS   (0x0070)  // 64 x 12-bit shunt samples averaged together
#define INA219_CONFIG_SADCRES_12BIT_128S_69MS  (0x0078)  // 128 x 12-bit shunt samples averaged together

#define INA219_CONFIG_MODE_MASK                (0x0007)  // Operating Mode Mask
#define INA219_CONFIG_MODE_POWERDOWN           (0x0000)
#define INA219_CONFIG_MODE_SVOLT_TRIGGERED     (0x0001)
#define INA219_CONFIG_MODE_BVOLT_TRIGGERED     (0x0002)
#define INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED (0x0003)
#define INA219_CONFIG_MODE_ADCOFF              (0x0004)
#define INA219_CONFIG_MODE_SVOLT_CONTINUOUS    (0x0005)
#define INA219_CONFIG_MODE_BVOLT_CONTINUOUS    (0x0006)
#define INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS (0x0007)    
//INA���õĲ���
#define INA_R           0.1                                    //��ֵ��С����λohm��
#define INA_I_MAX       3                                //Ӳ��������������λ��A��    INA219_CONFIG_GAIN_��ѹ/INA_R
#define IAN_I_LSB       0.1                            //(0x04)����ֵ��Ӧÿλ����ֵ    ѡ��ӽ� = (INA_I_MAX/32767(2^15))*1000(Ϊ�˵õ���λΪA)        (A)
#define INA_Power_LSB   2                        //(0x03)����ֵ��Ӧÿλ����ֵ    = 20 * IAN_I_LSB    (MW)
#define INA_CAL         4096                            //(0x05)�Ĵ�����׼ֵ
//�������涨������ ���üĴ�����0x00����ֵ��0x3C1F
#define INA219_CONFIG_value INA219_CONFIG_BVOLTAGERANGE_32V|INA219_CONFIG_GAIN_8_320MV|INA219_CONFIG_BADCRES_12BIT|INA219_CONFIG_SADCRES_12BIT_1S_532US|INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS

#define INA_INSIDE_IIC    //ʹ���Դ���ioģ��iic
#ifdef INA_INSIDE_IIC    
        #define IIC_PORT        GPIOB                                                                                //IIC IO�˿�
        #define IIC_RCC         RCC_AHB1Periph_GPIOB                                                //�˿�ʱ��
        #define IIC_SDA         GPIO_Pin_11                                                                        //SDA����
        #define IIC_SDA_H       GPIO_SetBits(IIC_PORT,IIC_SDA)                            //����SDA
        #define IIC_SDA_L       GPIO_ResetBits(IIC_PORT,IIC_SDA)                        //����SDA
        #define IIC_SDA_READ()  GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA)    //��ȡSDA

        #define IIC_SCL         GPIO_Pin_10                                                                        //SCL����
        #define IIC_SCL_H       GPIO_SetBits(IIC_PORT,IIC_SCL)                            //����SCL
        #define IIC_SCL_L       GPIO_ResetBits(IIC_PORT,IIC_SCL)                        //����SCL
#endif

void ina219_init(void );
unsigned int ina219_get_voltage_mv(void);//��ȡ��ѹ����λ��mv��
unsigned int ina219_get_current_ma(void);//��ȡ��������λ��mA��
unsigned int ina219_get_power_mv(void);    //��ȡ��ǰ���ʣ���λ��mw��
unsigned int ina219_get_r_mv(void);


#ifdef __cplusplus
}
#endif

#endif


