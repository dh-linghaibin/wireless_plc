/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "wireless.h"

/*!
    \brief      configure different peripheral clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void rcu_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
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
static void gpio_config(void)
{
    /* SPI0 GPIO config:SCK/PA5, MISO/PA6, MOSI/PA7 */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_15);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
    
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
}

/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void spi_config(void)
{
//    spi_parameter_struct spi_init_struct;

//    /* SPI0 parameter config */
//    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
//    spi_init_struct.device_mode          = SPI_MASTER;
//    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
//    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
//    spi_init_struct.nss                  = SPI_NSS_SOFT;
//    spi_init_struct.prescale             = SPI_PSC_256;
//    spi_init_struct.endian               = SPI_ENDIAN_MSB;
//    spi_init(SPI1, &spi_init_struct);
    spi_parameter_struct spi_init_struct;

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_16;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI1, &spi_init_struct);
    
    //spi_nss_internal_high(SPI1);
}

uint8_t spi_send(uint8_t byte) {
    spi_i2s_data_transmit(SPI1, byte);
    while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_TBE));
    while (RESET == spi_i2s_flag_get(SPI1, SPI_FLAG_RBNE));
    return (spi_i2s_data_receive(SPI1));
}

void
wireless_init(void)
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
