/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId StartTaskHandle;
osThreadId LoopATaskHandle;
osThreadId LoopBTaskHandle;
osThreadId IAP_TaskHandle;
osThreadId ComTxTaskHandle;
osThreadId ComRxTaskHandle;
osThreadId MemoryTaskHandle;
osMessageQId comRxBufQHandle;
osMessageQId userIDQueueHandle;
osMessageQId qualifiedIDQueueHandle;
osMessageQId infoIDQueueHandle;
osMessageQId comTxBufQHandle;
osMessageQId memoryQueueHandle;
osTimerId rfidTimerHandle;
osMutexId authorizeResultMutexHandle;
osSemaphoreId comFrameRxSemHandle;
osSemaphoreId comFrameTxSemHandle;
osSemaphoreId authorizeResultSemHandle;
osSemaphoreId loopARxSemHandle;
osSemaphoreId loopBRxSemHandle;
osSemaphoreId loopCRxSemHandle;
osSemaphoreId tempADC_SemHandle;
osSemaphoreId voltADC_SemHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void startTask(void const * argument);
void loopATask(void const * argument);
void loopBTask(void const * argument);
void iapTask(void const * argument);
void comTxTask(void const * argument);
void comRxTask(void const * argument);
void memoryTask(void const * argument);
extern void ReadRFID(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Pre/Post sleep processing prototypes */
void PreSleepProcessing(uint32_t *ulExpectedIdleTime);
void PostSleepProcessing(uint32_t *ulExpectedIdleTime);

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 4 */
__weak void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN PREPOSTSLEEP */
__weak void PreSleepProcessing(uint32_t *ulExpectedIdleTime)
{
/* place for user code */ 
}

__weak void PostSleepProcessing(uint32_t *ulExpectedIdleTime)
{
/* place for user code */
}
/* USER CODE END PREPOSTSLEEP */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of authorizeResultMutex */
  osMutexDef(authorizeResultMutex);
  authorizeResultMutexHandle = osMutexCreate(osMutex(authorizeResultMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of comFrameRxSem */
  osSemaphoreDef(comFrameRxSem);
  comFrameRxSemHandle = osSemaphoreCreate(osSemaphore(comFrameRxSem), 1);

  /* definition and creation of comFrameTxSem */
  osSemaphoreDef(comFrameTxSem);
  comFrameTxSemHandle = osSemaphoreCreate(osSemaphore(comFrameTxSem), 1);

  /* definition and creation of authorizeResultSem */
  osSemaphoreDef(authorizeResultSem);
  authorizeResultSemHandle = osSemaphoreCreate(osSemaphore(authorizeResultSem), 1);

  /* definition and creation of loopARxSem */
  osSemaphoreDef(loopARxSem);
  loopARxSemHandle = osSemaphoreCreate(osSemaphore(loopARxSem), 1);

  /* definition and creation of loopBRxSem */
  osSemaphoreDef(loopBRxSem);
  loopBRxSemHandle = osSemaphoreCreate(osSemaphore(loopBRxSem), 1);

  /* definition and creation of loopCRxSem */
  osSemaphoreDef(loopCRxSem);
  loopCRxSemHandle = osSemaphoreCreate(osSemaphore(loopCRxSem), 1);

  /* definition and creation of tempADC_Sem */
  osSemaphoreDef(tempADC_Sem);
  tempADC_SemHandle = osSemaphoreCreate(osSemaphore(tempADC_Sem), 1);

  /* definition and creation of voltADC_Sem */
  osSemaphoreDef(voltADC_Sem);
  voltADC_SemHandle = osSemaphoreCreate(osSemaphore(voltADC_Sem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of rfidTimer */
  osTimerDef(rfidTimer, ReadRFID);
  rfidTimerHandle = osTimerCreate(osTimer(rfidTimer), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityLow, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of StartTask */
  osThreadDef(StartTask, startTask, osPriorityIdle, 0, 128);
  StartTaskHandle = osThreadCreate(osThread(StartTask), NULL);

  /* definition and creation of LoopATask */
  osThreadDef(LoopATask, loopATask, osPriorityBelowNormal, 0, 128);
  LoopATaskHandle = osThreadCreate(osThread(LoopATask), NULL);

  /* definition and creation of LoopBTask */
  osThreadDef(LoopBTask, loopBTask, osPriorityBelowNormal, 0, 128);
  LoopBTaskHandle = osThreadCreate(osThread(LoopBTask), NULL);

  /* definition and creation of IAP_Task */
  osThreadDef(IAP_Task, iapTask, osPriorityHigh, 0, 512);
  IAP_TaskHandle = osThreadCreate(osThread(IAP_Task), NULL);

  /* definition and creation of ComTxTask */
  osThreadDef(ComTxTask, comTxTask, osPriorityNormal, 0, 128);
  ComTxTaskHandle = osThreadCreate(osThread(ComTxTask), NULL);

  /* definition and creation of ComRxTask */
  osThreadDef(ComRxTask, comRxTask, osPriorityIdle, 0, 128);
  ComRxTaskHandle = osThreadCreate(osThread(ComRxTask), NULL);

  /* definition and creation of MemoryTask */
  osThreadDef(MemoryTask, memoryTask, osPriorityLow, 0, 64);
  MemoryTaskHandle = osThreadCreate(osThread(MemoryTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of comRxBufQ */
  osMessageQDef(comRxBufQ, 6, comBuf_typeDef);
  comRxBufQHandle = osMessageCreate(osMessageQ(comRxBufQ), NULL);

  /* definition and creation of userIDQueue */
  osMessageQDef(userIDQueue, 4, comBuf_typeDef);
  userIDQueueHandle = osMessageCreate(osMessageQ(userIDQueue), NULL);

  /* definition and creation of qualifiedIDQueue */
  osMessageQDef(qualifiedIDQueue, 4, comBuf_typeDef);
  qualifiedIDQueueHandle = osMessageCreate(osMessageQ(qualifiedIDQueue), NULL);

  /* definition and creation of infoIDQueue */
  osMessageQDef(infoIDQueue, 4, comBuf_typeDef);
  infoIDQueueHandle = osMessageCreate(osMessageQ(infoIDQueue), NULL);

  /* definition and creation of comTxBufQ */
  osMessageQDef(comTxBufQ, 5, comBuf_typeDef);
  comTxBufQHandle = osMessageCreate(osMessageQ(comTxBufQ), NULL);

  /* definition and creation of memoryQueue */
  osMessageQDef(memoryQueue, 16, uint32_t);
  memoryQueueHandle = osMessageCreate(osMessageQ(memoryQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* startTask function */
void startTask(void const * argument)
{
  /* USER CODE BEGIN startTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END startTask */
}

/* loopATask function */
__weak void loopATask(void const * argument)
{
  /* USER CODE BEGIN loopATask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END loopATask */
}

/* loopBTask function */
__weak void loopBTask(void const * argument)
{
  /* USER CODE BEGIN loopBTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END loopBTask */
}

/* iapTask function */
__weak void iapTask(void const * argument)
{
  /* USER CODE BEGIN iapTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END iapTask */
}

/* comTxTask function */
__weak void comTxTask(void const * argument)
{
  /* USER CODE BEGIN comTxTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END comTxTask */
}

/* comRxTask function */
__weak void comRxTask(void const * argument)
{
  /* USER CODE BEGIN comRxTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END comRxTask */
}

/* memoryTask function */
__weak void memoryTask(void const * argument)
{
  /* USER CODE BEGIN memoryTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END memoryTask */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
