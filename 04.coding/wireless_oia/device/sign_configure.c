/*
 * This file is part of the 
 *
 * Copyright (c) 2017-2018 linghaibin
 *
 */

#include "sign_configure.h"
#include "delay.h"

static uint16_t adc_value[4];
/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void rcu_config(void) {
    rcu_periph_clock_enable(RCU_GPIOC); /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_ADC0);              /* enable ADC clock */
    rcu_periph_clock_enable(RCU_DMA0);           /* enable DMA0 clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8); /* config ADC clock */
    rcu_periph_clock_enable(RCU_DAC);             /* enable DAC clock */
}
/**
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
  */
static void gpio_config(void)
{
    /*Configure PC11 PC12 PD2 PB3 DI*/
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    /*Configure PC14 PC15 DO*/
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
    /*Configure PB1 PB0 PC5 PA6 AI*/
    gpio_init(GPIOB, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_init(GPIOB, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    /*Configure PA5 PA4 AO*/
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
    /*Configure PB11 PB10 PC4 PA7 500Å·µç×è½ÓÈë*/
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
}
/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void dma_config(void) {
    dma_parameter_struct dma_data_parameter; /* ADC_DMA_channel configuration */
    dma_deinit(DMA0, DMA_CH0);               /* ADC DMA_channel configuration */
    
    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr  = (uint32_t)(&adc_value);
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;  
    dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number       = 4;
    dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH0, dma_data_parameter);

    dma_circulation_enable(DMA0, DMA_CH0);
  
    dma_channel_enable(DMA0, DMA_CH0); /* enable DMA channel */
}
/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void adc_config(void) {
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);  /* ADC trigger config */
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
    
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);                         /* ADC data alignment config */
    adc_mode_config(ADC_MODE_FREE);                                               /* ADC mode config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 4);                      /* ADC channel length config */
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_8, ADC_SAMPLETIME_239POINT5); /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_9, ADC_SAMPLETIME_239POINT5); /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_15, ADC_SAMPLETIME_239POINT5); /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_6, ADC_SAMPLETIME_239POINT5); /* ADC regular channel config */
    adc_discontinuous_mode_config(ADC0, ADC_REGULAR_CHANNEL, 3);                  /* ADC discontinuous mode */
    adc_enable(ADC0);                                                               /* enable ADC interface */
    delay_ms(1);                                                                   /* wait adc enable must*/
    adc_calibration_enable(ADC0);                                                   /* ADC calibration and reset calibration */
    adc_dma_mode_enable(ADC0);                                                       /* ADC DMA function enable */
}

/*!
    \brief      configure the DAC
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void dac_config(void)
{
    dac_deinit();
    /* configure the DAC0 */
    dac_trigger_disable(DAC0);
    dac_wave_mode_config(DAC0, DAC_WAVE_DISABLE);
    dac_output_buffer_enable(DAC0);
    
    /* configure the DAC1 */
    dac_trigger_disable(DAC1);
    dac_wave_mode_config(DAC1, DAC_WAVE_DISABLE);
    dac_output_buffer_enable(DAC1);
    
    /* enable DAC concurrent mode and set data */
    dac_concurrent_enable();
    //dac_concurrent_data_set(DAC_ALIGN_12B_L, 0xFFFF, 0xFFFF);
    dac_data_set(DAC0,DAC_ALIGN_12B_L,0x00);
    dac_data_set(DAC1,DAC_ALIGN_12B_L,0x00);
}

void
sign_configure(void) {
    rcu_config();
    gpio_config();
    dma_config();
    adc_config();
    dac_config();

    gpio_bit_write(GPIOC,GPIO_PIN_14,RESET);
    gpio_bit_write(GPIOC,GPIO_PIN_15,SET);
}

void
sign_update(void) {
    for(uint8_t i = 0; i < 4;i++) {
        adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); /* ADC software trigger enable */
    }
}

uint16_t
sign_read(sign_name name) {
    switch(name) {
        case S_DI1: {
            return gpio_input_bit_get(GPIOC,GPIO_PIN_11);
        } //break;
        case S_DI2: {
            return gpio_input_bit_get(GPIOC,GPIO_PIN_12);
        } //break;
        case S_DI3: {
            return gpio_input_bit_get(GPIOD,GPIO_PIN_2);
        } //break;
        case S_DI4: {
            return gpio_input_bit_get(GPIOB,GPIO_PIN_3);
        } //break;
        case S_AI1: {
            sign_update();
            return adc_value[0];
        }
        case S_AI2: {
            sign_update();
            return adc_value[1];
        }
        case S_AI3: {
            sign_update();
            return adc_value[2];
        }
        case S_AI4: {
            sign_update();
            return adc_value[3];
        }
        default: {
            return 0;
        } //break;
    }
    //return 0;
}

void sign_set(sign_name name, uint16_t val) {
    switch(name) {
        case S_DO1: {
            gpio_bit_write(GPIOC,GPIO_PIN_14,(bit_status)val);
        } break;
        case S_DO2: {
            gpio_bit_write(GPIOC,GPIO_PIN_15,(bit_status)val);
        } break;
        case S_AO1: {
            dac_data_set(DAC0,DAC_ALIGN_12B_L,val);
        } break;
        case S_AO2: {
            dac_data_set(DAC1,DAC_ALIGN_12B_L,val);
        } break;
    }
} 
