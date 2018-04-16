/*
 * This file is part of the 
 *
 * Copyright (c) 2016-2018 linghaibin
 *
 */

#include "sys.h"
#include "task_set.h"
#include "task_can.h"
//freertos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

int main(void) {
    sys_init_rcc();
    task_set_init();
    task_can_init();
    task_set_create();
    task_can_create();
    /* Start the scheduler. */
    vTaskStartScheduler();
    for(;;);
}

#ifdef  USE_FULL_ASSERT
/** °Ñ#ifdef DEBUGÌæ»»Îª#ifdef USE_FULL_ASSERT
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  //#ifdef __DEBUG_Example
    //printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  //#else
    /* Infinite loop */
    while (1)
    {
    }
  //#endif
}
#endif


