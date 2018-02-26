/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#include "s_delay.h"

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void sdelay_us(uint32_t dly)
{
	uint8_t i;
	while(dly--) {
		for(i=0;i<5;i++);
	}
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void sdelay_ms(uint32_t dly)
{
	while(dly--) sdelay_us(1000);
}
