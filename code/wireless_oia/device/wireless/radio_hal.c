
#include 		"radio.h"
#include "gd32f10x.h"

#define SI446X_IRQ_GPIO        	GPIOC
#define	SI446X_PIN_IRQ         	GPIO_PIN_7 //D
#define SI446X_IRQ_IN     	  	gpio_input_bit_get(SI446X_IRQ_GPIO,SI446X_PIN_IRQ)

#define SI446X_SDN_GPIO       	GPIOB
#define	SI446X_PIN_SDN        	GPIO_PIN_12 //D
#define SI446X_SDN_HIGH()      	gpio_bit_set(SI446X_SDN_GPIO, SI446X_PIN_SDN)
#define SI446X_SDN_LOW()  	  	gpio_bit_reset(SI446X_SDN_GPIO, SI446X_PIN_SDN)

#define SI446X_CS_GPIO       		GPIOC
#define	SI446X_PIN_CS        		GPIO_PIN_6 //D
#define SI446X_CS_HIGH()      	gpio_bit_set(SI446X_CS_GPIO, SI446X_PIN_CS)
#define SI446X_CS_LOW()  	  	gpio_bit_reset(SI446X_CS_GPIO, SI446X_PIN_CS)



//#define	SPIX_PIN_SCK         	GPIO_Pin_5
//#define	SPIX_PIN_MISO         GPIO_Pin_6
//#define	SPIX_PIN_MOSI         GPIO_Pin_7
//#define SPIX_GPIO        			GPIOA
//#define SPIX_DEVX           	SPI1


void radio_hal_AssertShutdown(void)
{
	SI446X_SDN_HIGH();
}

void radio_hal_DeassertShutdown(void)
{
		SI446X_SDN_LOW();
}

void radio_hal_ClearNsel(void)
{
		SI446X_CS_LOW();
}
void radio_hal_SetNsel(void)
{
	SI446X_CS_HIGH();
	  
}

BIT radio_hal_NirqLevel(void)
{
	return SI446X_IRQ_IN ? 1:0;
}

U8 SpiReadWrite(U8 buf)
{
    spi_i2s_data_transmit(SPI1, buf);
    while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE));
    while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE));
    return (spi_i2s_data_receive(SPI1));
}

void radio_hal_SpiWriteByte(U8 byteToWrite)
{
	SpiReadWrite(byteToWrite);

}

U8 radio_hal_SpiReadByte(void)
{

  return SpiReadWrite(0xFF);

}

void radio_hal_SpiWriteData(U8 byteCount, U8* pData)
{
	U8 i;
	for(i=0;i<byteCount;i++){
		SpiReadWrite(pData[i]);
	}
}

void radio_hal_SpiReadData(U8 byteCount, U8* pData)
{
	U8 i;
	for(i=0;i<byteCount;i++){
		pData[i]=SpiReadWrite(0xff);
	}
}


/*!
    \brief      configure different peripheral clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void rcu_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_SPI1);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void gpio_config(void)
{
    /* SPI0 GPIO config:SCK/PA5, MISO/PA6, MOSI/PA7 */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_15);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
    
    gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOD, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    
    
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
}

/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void spi_config(void)
{
    spi_parameter_struct spi_init_struct;

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI1, &spi_init_struct);
    
    spi_nss_internal_high(SPI1);
}

void radio_hal_Init(void)
{
 /* peripheral clock enable */
    rcu_config();
    /* GPIO configure */
    gpio_config();
    /* SPI configure */
    spi_config();
    /* SPI enable */
    spi_enable(SPI1);
}

