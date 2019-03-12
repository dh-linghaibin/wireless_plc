/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */
 
#include "ads1220.h"
#include "delay.h"

/*
6种命令操作设备
复位            0000 011x
启动/同步       0000 100x
关机            0000 001x
读命令          0001 xxxx
从地址RR开始读取NN寄存器 0010 rrnn
从地址rr开始写NN寄存器   0100 rrnn

00h 7 6 5 4  输入多路复用器的配置
Input multiplexer configuration
These bits configure the input multiplexer.
For settings where AINN = AVSS, the PGA must be disabled (PGA_BYPASS = 1)
and only gains 1, 2, and 4 can be used.
0000 : AINP = AIN0, AINN = AIN1 (default)
0001 : AINP = AIN0, AINN = AIN2
0010 : AINP = AIN0, AINN = AIN3
0011 : AINP = AIN1, AINN = AIN2
0100 : AINP = AIN1, AINN = AIN3
0110 : AINP = AIN1, AINN = AIN0
0111 : AINP = AIN3, AINN = AIN2
1000 : AINP = AIN0, AINN = AVSS
1001 : AINP = AIN1, AINN = AVSS
1010 : AINP = AIN2, AINN = AVSS
1011 : AINP = AIN3, AINN = AVSS
1100 : (V(REFPx) – V(REFNx)) / 4 monitor (PGA bypassed)
1101 : (AVDD – AVSS) / 4 monitor (PGA bypassed)
1110 : AINP and AINN shorted to (AVDD + AVSS) / 2
1111 : Reserved

00h 1 2 3 增益配置
These bits configure the device gain.
Gains 1, 2, and 4 can be used without the PGA. In this case, gain is obtained by
a switched-capacitor structure.
000 : Gain = 1 (default)
010 : Gain = 4
011 : Gain = 8
100 : Gain = 16
101 : Gain = 32
110 : Gain = 64
111 : Gain = 128 --

00h 0 禁用和绕过内部低噪声PGA
Disabling the PGA reduces overall power consumption and allows the commonmode voltage range (VCM) to span from AVSS – 0.1 V to AVDD + 0.1 V.
The PGA can only be disabled for gains 1, 2, and 4.
The PGA is always enabled for gain settings 8 to 128, regardless of the
PGA_BYPASS setting.
0 : PGA enabled (default)
1 : PGA disabled and bypassed

01h 5 6 7 数据速率
These bits control the data rate setting depending on the selected operating
mode. Table 18 lists the bit settings for normal, duty-cycle, and turbo mode
NORMAL MODE DUTY-CYCLE MODE TURBO MODE
000 = 20 SPS 000 = 5 SPS 000 = 40 SPS --
001 = 45 SPS 001 = 11.25 SPS 001 = 90 SPS
010 = 90 SPS 010 = 22.5 SPS 010 = 180 SPS
011 = 175 SPS 011 = 44 SPS 011 = 350 SPS
100 = 330 SPS 100 = 82.5 SPS 100 = 660 SPS
101 = 600 SPS 101 = 150 SPS 101 = 1200 SPS
110 = 1000 SPS 110 = 250 SPS 110 = 2000 SPS
111 = Reserved 111 = Reserved 111 = Reserved

01h 3 4 操作模式
These bits control the operating mode the device operates in.
00 : Normal mode (256-kHz modulator clock, default)
01 : Duty-cycle mode (internal duty cycle of 1:4)
10 : Turbo mode (512-kHz modulator clock)
11 : Reserved

01h 2 转换模式
This bit sets the conversion mode for the device.
0 : Single-shot mode (default)
1 : Continuous conversion mode

01h 1 温度传感器模式
This bit enables the internal temperature sensor and puts the device in
temperature sensor mode.
The settings of configuration register 0 have no effect and the device uses the
internal reference for measurement when temperature sensor mode is enabled.
0 : Disables temperature sensor (default)
1 : Enables temperature sensor

01h 0 烧毁电流源
This bit controls the 10-μA, burn-out current sources.
The burn-out current sources can be used to detect sensor faults such as wire
breaks and shorted sensors.
0 : Current sources off (default)
1 : Current sources on

02h 7 6 参考电压的选择
These bits select the voltage reference source that is used for the conversion.
00 : Internal 2.048-V reference selected (default)
01 : External reference selected using dedicated REFP0 and REFN0 inputs
10 : External reference selected using AIN0/REFP1 and AIN3/REFN1 inputs
11 : Analog supply (AVDD – AVSS) used as reference

02h 5 4 FIR滤波器配置
These bits configure the filter coefficients for the internal FIR filter.
These bits only affect the 20-SPS setting in normal mode and 5-SPS setting in
duty-cycle mode.
00 : No 50-Hz or 60-Hz rejection (default)
01 : Simultaneous 50-Hz and 60-Hz rejection
10 : 50-Hz rejection only
11 : 60-Hz rejection only

02h 3 低侧功率开关配置
This bit configures the behavior of the low-side switch connected between
AIN3/REFN1 and AVSS.
0 : Switch is always open (default)
1 : Switch automatically closes when the START/SYNC command is sent and opens when the POWERDOWN command is issued

02h 2 0 IDAC电流设置
These bits set the current for both IDAC1 and IDAC2 excitation current sources.
000 : Off (default)
001 : 10 μA
010 : 50 μA
011 : 100 μA
100 : 250 μA --
101 : 500 μA
110 : 1000 μA
111 : 1500 μA

03h 7 6 5 idac1路由配置
These bits select the channel where IDAC1 is routed to.
000 : IDAC1 disabled (default)
001 : IDAC1 connected to AIN0/REFP1
010 : IDAC1 connected to AIN1
011 : IDAC1 connected to AIN2
100 : IDAC1 connected to AIN3/REFN1
101 : IDAC1 connected to REFP0 --
110 : IDAC1 connected to REFN0
111 : Reserved

03h 4 3 2 idac2路由配置
These bits select the channel where IDAC2 is routed to.
000 : IDAC2 disabled (default)
001 : IDAC2 connected to AIN0/REFP1
010 : IDAC2 connected to AIN1 --
011 : IDAC2 connected to AIN2
100 : IDAC2 connected to AIN3/REFN1
101 : IDAC2 connected to REFP0
110 : IDAC2 connected to REFN0
111 : Reserved

03h 1 DRDY mode
This bit controls the behavior of the DOUT/DRDY pin when new data are ready.
0 : Only the dedicated DRDY pin is used to indicate when data are ready (default)
1 : Data ready is indicated simultaneously on DOUT/DRDY and DRDY

03h 0 保留
*/

#define ADS_CS_SET      gpio_bit_set(GPIOD,GPIO_PIN_9)
#define ADS_CS_ERSET    gpio_bit_reset(GPIOD,GPIO_PIN_9)

#define ADS1248_READY_PIN() gpio_input_bit_get(GPIOD,GPIO_PIN_8)//低电平表示READY

/*!
    \brief      configure different peripheral clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_SPI1);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* SPI0 GPIO config:SCK/PA5, MISO/PA6, MOSI/PA7 */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_15);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
    
    gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOD, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
}

/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config(void)
{
    spi_parameter_struct spi_init_struct;

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_256;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI1, &spi_init_struct);
    
    spi_nss_internal_high(SPI1);
}

uint8_t spi_send(uint8_t byte) {
    spi_i2s_data_transmit(SPI1, byte);
    while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE));
    while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE));
    return (spi_i2s_data_receive(SPI1));
}

uint8_t ads1220_reg(uint8_t regaddr) {
    //从ADS1220中地址为regaddr的寄存器读出一个字节
    uint8_t r=0, len =0;
    ADS_CS_ERSET;
    //	DS_delay_us(10);
    while(gpio_input_bit_get(GPIOD,GPIO_PIN_8) == SET);//当ADS1220_DRDY为低时才能写寄存器
    //写入地址
    spi_send(ADS1220_RREG_CMD | ((regaddr<<2|(len&0x0)) & 0xF));
    delay_us(10);  //min=50*(1/fCLKIN)=50*(1/7.68MHZ)=6500ns;max=whatever
    //读出regaddr地址指向的寄存器的数据
    r=spi_send(0);
    ADS_CS_SET;

    return r;//返回数据
}

uint8_t xxx[10];
signed long ads1220_read_data(void)  
{
    unsigned char i=0;
    unsigned long sum=0;
    unsigned long r=0;
    ADS_CS_ERSET;
    delay_us(100);
    spi_send(ADS1220_START_CMD);
    spi_send(ADS1220_RDATA_CMD);
    
    while(gpio_input_bit_get(GPIOD,GPIO_PIN_8) == SET);
    delay_us(1000);                  //min=50*(1/fCLKIN)=50*(1/7.68MHZ)=6500ns;max=whatever
    for(i=0;i<3;i++)
    {
        sum = sum << 8;
        r = spi_send(0xff); 
        xxx[i] = r;
        sum |= r;
    }
    ADS_CS_SET; 
    return sum;
}

unsigned char ADS1220RREG(unsigned char regaddr) {
    //从ADS1220中地址为regaddr的寄存器读出一个字节
    unsigned char r=0, len =0;
    ADS_CS_ERSET;
    //	usleep(10);
    //	while(ADS_DRDY);//当ADS1220_DRDY为低时才能写寄存器
    //写入地址

    spi_send(ADS1220_RREG_CMD | ((regaddr<<2|(len&0x0)) & 0xF));
    //写入读取数据的个数n-1
    // spi_send_byte(0);
    delay_ms(10);  //min=50*(1/fCLKIN)=50*(1/7.68MHZ)=6500ns;max=whatever
    //读出regaddr地址指向的寄存器的数据
    r=spi_send(0);
    ADS_CS_SET;

    return r;//返回数据
}


void ads1220_init(void) {
    /* peripheral clock enable */
    rcu_config();
    /* GPIO configure */
    gpio_config();
    /* SPI configure */
    spi_config();
    /* SPI enable */
    spi_enable(SPI1);
    
    unsigned char config[4];
    //config[0] = ADS1220_MUX_AIN0_AIN1 + ADS1220_GAIN_16 + ADS1220_USE_PGA;
    config[0] = 0x0a;//ADS1220_MUX_AIN0_AIN1 + ADS1220_GAIN_32 + ADS1220_USE_PGA + ADS1220_DATA_RATE_20SPS;//ADS1220_MUX_AIN0_AIN1 + ADS1220_GAIN_128 + ADS1220_USE_PGA;
    config[1] = 0x04;//0x04ADS1220_DATA_RATE_20SPS + ADS1220_OP_MODE_NORMAL + ADS1220_CONVERSION_CONTINUOUS + ADS1220_TEMP_SENSOR_OFF + ADS1220_BURN_OUT_CURRENT_OFF;//ADS1220_DATA_RATE_20SPS + ADS1220_IDAC_CURRENT_250_UA;//ADS1220_OP_MODE_NORMAL + ADS1220_CONVERSION_CONTINUOUS + ADS1220_TEMP_SENSOR_OFF + ADS1220_BURN_OUT_CURRENT_OFF +
    config[2] = 0x05;//;c//ADS1220_FIR_50_60 + ADS1220_VREF_EXT_REF0_PINS + ADS1220_LOW_SIDE_POWER_OPEN + ADS1220_IDAC_CURRENT_OFF; 0x06
    config[3] = 0xa8;//ADS1220_IDAC1_REFP0 + ADS1220_IDAC2_AIN1;//ADS1220_DRDY_ON_DRDY_ONLY
    
//    config[0] = ADS1220_MUX_SHIFT + ADS1220_MUX_AIN0_AIN1 + ADS1220_GAIN_SHIFT + ADS1220_GAIN_32 + ADS1220_USE_PGA;
//    config[1] = ADS1220_DATA_RATE_20SPS + ADS1220_OP_MODE_NORMAL + ADS1220_CONVERSION_CONTINUOUS + ADS1220_TEMP_SENSOR_OFF + ADS1220_BURN_OUT_CURRENT_OFF;
//    config[2] = ADS1220_VREF_INTERNAL + ADS1220_FIR_50_60 + ADS1220_LOW_SIDE_POWER_CLOSES + ADS1220_IDAC_CURRENT_500_UA;
//    config[3] = ADS1220_IDAC1_REFP0 + ADS1220_IDAC2_AIN1 + ADS1220_DRDY_ON_DRDY_ONLY;
    
    ADS_CS_ERSET;
    delay_ms(1000);
    spi_send(ADS1220_RESET_CMD);
    delay_ms(100);
    spi_send(ADS1220_START_CMD);                 //同步命令
    spi_send(ADS1220_START_CMD);               //同步唤醒
    while(gpio_input_bit_get(GPIOD,GPIO_PIN_8) == SET);
    spi_send(ADS1220_WREG_CMD | 3);//连续写入4个寄存器
    spi_send(config[0]);
    spi_send(config[1]);//高位在前，使用内部校准，不使用缓存
    spi_send(config[2]);//端口输入A2为正，A3位负
    spi_send(config[3]);//放大倍数设置
    delay_ms(100);
    while(gpio_input_bit_get(GPIOD,GPIO_PIN_8) == SET);
    ADS_CS_SET;
    delay_ms(100);
    ads1220_reg(0);
    
//    config[0] = ADS1220RREG(0);
//    config[1] = ADS1220RREG(0);
    
    signed long ctemp = ads1220_read_data();
    float cval = 0;
    //cval = ctemp * 0.064/0x800000;//* 2.048 / 0x7fffff)
    cval = (ctemp * 0.064/0x7fffff)/3;
    if(cval == 0x00) {
        cval = 2;
    }
}

void ads1220_set_mode(ads_read_mode mode) {
    switch(mode) {
        case PH100: {
        
        } break;
        case PT1000: {
        
        } break;
        case NTC10K: {
        
        } break;
    }
}

//float ADS1248_ResToTemp(float resistance)
//{
//    u16 low = 0;
//    u16 mid = sizeof(pt100_table) / sizeof(pt100_table[0]) / 2;
//    u16 high = sizeof(pt100_table) / sizeof(pt100_table[0]) - 1;

//    if((resistance < pt100_table[0]) || (resistance > pt100_table[high]))
//    {
//        return -1; // 超限，不做特殊处理
//    }

//    while(high >= low)
//    {
//        mid = (high + low) / 2;

//        if(resistance >= pt100_table[mid])
//        {
//            if(resistance < pt100_table[mid + 1])
//            {
//                return (0.1 * mid + (resistance - pt100_table[mid]) * 0.1 / ((pt100_table[mid + 1]) - pt100_table[mid]));
//            }
//            else
//            {
//                low = mid + 1;
//            }
//        }
//        else if(resistance < pt100_table[mid])
//        {
//            high = mid - 1;
//        }
//    }

//    return -1; // 查找失败，不做特殊处理
//}
