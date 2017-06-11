/*
 * userSrc.c
 *
 *  Created on: May 9, 2017
 *      Author: LHANG
 */
#include "userSrc.h"

#define MEASURE_TIMES	5
#define COM_MODE_NORMAL				0
#define COM_MODE_WAKEUP				1
#define COM_MODE_SAVING				2
#define COM_MODE_SLEEP				3

//semaphores
extern osSemaphoreId comFrameRxSemHandle;
extern osSemaphoreId comFrameTxSemHandle;
extern osSemaphoreId authorizeResultSemHandle;
extern osSemaphoreId loopARxSemHandle;
extern osSemaphoreId loopBRxSemHandle;
extern osSemaphoreId loopCRxSemHandle;

//Queue
extern osMessageQId comRxBufQHandle;
extern osMessageQId userIDQueueHandle;
extern osMessageQId qualifiedIDQueueHandle;
extern osMessageQId infoIDQueueHandle;
extern osMessageQId comTxBufQHandle;
extern osMessageQId cmdQueueHandle;

//DMA
extern DMA_HandleTypeDef hdma_usart1_rx;          //for debug and communication
extern DMA_HandleTypeDef hdma_usart2_rx;					//for loop A
extern DMA_HandleTypeDef hdma_usart3_rx;					//for loop B
extern DMA_HandleTypeDef hdma_usart6_rx;					//for loop C

uint8_t ucCom1RxBuf[32] = {0, };
uint8_t ucCom3RxBuf[32] = {0, };
uint8_t	ucCom2RxBuf[32] = {0, };
uint8_t ucCom6RxBuf[32] = {0, };
uint8_t ucCom1TxBuf[32] = {0, };

//number of received data in byte
uint8_t ucCom1RxLen = 0;
uint8_t ucCom2RxLen = 0;
uint8_t ucCom3RxLen = 0;
uint8_t ucCom6RxLen = 0;

uint32_t uiTmp= 0;


uint8_t rfidAskCmd[RFID_ASK_CMD_LEN] = {0x07, 0x12, 0x41, 0x01, 0x52, 0xf8, 0x03};
uint8_t rfidReadCmd[RFID_READ_CMD_LEN] = {0x0c, 0x22, 0x42, 0x06, 0x93, 0x00, 0x78, 0x01, 0xa6, 0x00, 0xd9, 0x03};
uint8_t	rfidStopCmd[RFID_STOP_CMD_LEN] = {0x06, 0x32, 0x44, 0x00, 0x8f, 0x03};

uint8_t ucDataTest[256] = {0, };
uint8_t ucTestRead[256] = {0, };

void clearBuf(uint8_t *buf, int cnt);
void errorRpt(uint32_t err);

osEvent uart1Event;

void userUartHandler(UART_HandleTypeDef *huart)
{
	uint32_t tmpFlag = 0;
	uint32_t tmp;
	osStatus result = 0;
	osStatus error = osOK;
	tmpFlag = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
	if (tmpFlag != RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		tmp = huart->Instance->SR;
		tmp = huart->Instance->DR;
		HAL_UART_DMAStop(huart);

		//tmp = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx); //__HAL_DMA_GET_COUNTER(); //
		//ucCom1RxLen = UART_BUF_SIZE - tmp;
		//ucCom1RxLen = __HAL_DMA_GET_COUNTER(huart->hdmarx);
		//__HAL_DMA_SET_COUNTER(huart->hdmarx, 32);

		if (huart->Instance == USART1)
		{
			//osMessagePut (osMessageQId queue_id, uint32_t info, uint32_t millisec)
			error = osMessagePut (comRxBufQHandle, ucCom1RxBuf, osWaitForever);
			result = osSemaphoreRelease(comFrameRxSemHandle);
		}
		else if(huart->Instance == USART2)
		{
			//if (uiTmp++ >= 5)
			{
				uiTmp = 0;
				result = osSemaphoreRelease(loopARxSemHandle);
			}

		}
		else if (huart->Instance == USART3)
		{
			result = osSemaphoreRelease(loopBRxSemHandle);
		}
		else if (huart->Instance == USART6)
		{
			result = osSemaphoreRelease(loopCRxSemHandle);
		}
		else
		{
			;
		}
	}
}

void ReadRFID(void const * argument)
{

}


void comRxTask(void const * argument)
{
	//BaseType_t err = pdFALSE;
	osStatus err = osOK;
	uint32_t ind = 0; //indicator for LED state
	uint32_t i, dmaCnt;
	uint8_t strFlashID[16] = "";
	uint32_t uiLenID;
	uint32_t uiCnt;
	uint32_t uiPage;
	TickType_t xTimeStart, xTimeStop, xExeTime;
	uint32_t uiWeight[MEASURE_TIMES] = {0, };

	for (uiCnt = 0; uiCnt <= 255; uiCnt++)
	{
		ucDataTest[uiCnt] = uiCnt;
	}
	uiCnt = 0;
	//debug flash
	//HAL_UART_Receive_DMA(&huart1, ucCom1RxBuf, 32);

	while(1)
	{


		HAL_UART_Receive_DMA(&huart1, ucCom1RxBuf, 32);
		err = osSemaphoreWait(comFrameRxSemHandle, osWaitForever);
		//W25Qx_Init();
		//W25Qx_Read_ID(strFlashID);
		//uiCnt = 0;
		//for (i = 0; i < MEASURE_TIMES; i++)
		//{
		//	uiWeight[i] = uiReadScalesA();
		//	uiCnt += uiWeight[i];
		//}
/*		uiCnt = (uiReadScalesA()-163100)*10/809;//uiCnt/MEASURE_TIMES;
		itoa(uiCnt, strFlashID, 10);
		uiLenID = sizeof (strFlashID);
		HAL_UART_Transmit(&huart1, "Weight:", 7, 50);
		HAL_UART_Transmit(&huart1, strFlashID, uiLenID, 50);
		HAL_UART_Transmit(&huart1, "\n", 1, 20);

========debug flash=====================
		xTimeStart = xTaskGetTickCount();
		//Write Flash
		for (uiPage = 0; uiPage < 200; uiPage++)
		{
			ucDataTest[0] = uiPage;
			//W25Qx_Write(ucDataTest, uiPage<<8, 256);
		}
		xTimeStop = xTaskGetTickCount();
		xExeTime = xTimeStop - xTimeStart;
		itoa((int)xExeTime, strFlashID, 10);
		HAL_UART_Transmit(&huart1, "Write Flash Time:", 17, 50);
		HAL_UART_Transmit(&huart1, strFlashID, 16, 50);
		HAL_UART_Transmit(&huart1, "\n", 1, 20);

		//Read Flash
		xTimeStart = xTaskGetTickCount();
		for (uiPage = 0; uiPage < 190; uiPage++)
		{
			W25Qx_Read(ucTestRead, uiPage << 8, 256);
		}

		xTimeStop = xTaskGetTickCount();
		xExeTime = xTimeStop - xTimeStart;
		itoa((int)xExeTime, strFlashID, 10);
		HAL_UART_Transmit(&huart1, "Read Flash Time:", 16, 50);
		HAL_UART_Transmit(&huart1, strFlashID, 16, 50);
		HAL_UART_Transmit(&huart1, "\n", 1, 20);
		HAL_UART_Transmit(&huart1, "Read Flash Data:\n", 17, 50);
		HAL_UART_Transmit(&huart1, ucTestRead, 256, portMAX_DELAY);
		HAL_UART_Transmit(&huart1, "\n", 1, 20);

		//Erase chip
		xTimeStart = xTaskGetTickCount();
		//W25Qx_Erase_Chip();
		xTimeStop = xTaskGetTickCount();
		xExeTime = xTimeStop - xTimeStart;
		itoa((int)xExeTime, strFlashID, 10);
		HAL_UART_Transmit(&huart1, "Erase Flash Time:", 17, 50);
		HAL_UART_Transmit(&huart1, strFlashID, 16, 50);
		HAL_UART_Transmit(&huart1, "\n", 1, 20);

		HAL_UART_Transmit(&huart1, "Confirm Erasing:\n", 17, 50);
		W25Qx_Read(ucTestRead, 120 << 8, 256);
		W25Qx_Read(ucTestRead, 121 << 8, 256);
		HAL_UART_Transmit(&huart1, ucTestRead, 256, portMAX_DELAY);
		HAL_UART_Transmit(&huart1, "\n", 1, 20);

		while(1)
		{
			osDelay(100);
		}
*/


		/* ========debug flash=====================
		//err = xSemaphoreTake(rxDataRdySemHandle, portMAX_DELAY);
		//err == pdTRUE;
		err = osSemaphoreWait(comFrameRxSemHandle, osWaitForever);
		dmaCnt = UART_BUF_SIZE - __HAL_DMA_GET_COUNTER(huart1.hdmarx);
		if(err != osErrorOS)
		{
			if (ind == 0)
			{
				HAL_GPIO_WritePin(coreLED1_GPIO_Port, coreLED1_Pin, GPIO_PIN_SET);
				ind = 1;
			}
			else
			{
				HAL_GPIO_WritePin(coreLED1_GPIO_Port, coreLED1_Pin, GPIO_PIN_RESET);
				ind = 0;
			}


			for (i = 0; i < dmaCnt; i++)
			{
				ucCom1TxBuf[i] = ucCom1RxBuf[i];
			}

			HAL_UART_Transmit(&huart1, ucCom1TxBuf, dmaCnt, UART_TIMEOUT_MS);
			clearBuf(ucCom1TxBuf, dmaCnt);
			__HAL_DMA_SET_COUNTER(huart1.hdmarx, UART_BUF_SIZE);
			//HAL_UART_Transmit(&huart3, rfidCMD, 7, 30);
			//HAL_UART_Receive_DMA(&huart3, ucCom3RxBuf, 24);
			HAL_UART_Receive_DMA(&huart1, ucCom1RxBuf, 32);
		}
		*/
		osDelay(500);
	}
}

void loopATask(void const * argument)
{
	osStatus err = osOK;
	int i;
	uint32_t dmaCnt;
	for (;;)
	{
		/*
		//rfid ask process
		HAL_UART_Receive_DMA(&huart2, ucCom2RxBuf, UART_BUF_SIZE);
		HAL_UART_Transmit(&huart2, rfidAskCmd, RFID_READ_CMD_LEN, RFID_TIMEOUT_MS);
		//

		osDelay(100);
		HAL_UART_DMAStop(&huart2);
		//err = osSemaphoreWait(loopARxSemHandle, RFID_TIMEOUT_MS);
		dmaCnt = 32 - __HAL_DMA_GET_COUNTER(huart2.hdmarx);
		if (err == osOK)
		{
			//if (RFID_ASK_ANS_LEN == dmaCnt)          //__HAL_DMA_GET_COUNTER(huart2.hdmarx))
			{
				//if (0x08 == ucCom2RxBuf[0])
				{
					for (i = 0; i < dmaCnt; i++)     //__HAL_DMA_GET_COUNTER(huart2.hdmarx); i++)
					{
						ucCom1TxBuf[i] = ucCom2RxBuf[i];
					}
					HAL_UART_Transmit(&huart1, ucCom1TxBuf, dmaCnt, UART_TIMEOUT_MS);
					clearBuf(ucCom1TxBuf, dmaCnt);            //__HAL_DMA_GET_COUNTER(huart2.hdmarx));
					__HAL_DMA_SET_COUNTER(huart2.hdmarx, UART_BUF_SIZE);

					osDelay(100);


					HAL_UART_Transmit(&huart2, rfidReadCmd, RFID_READ_CMD_LEN, RFID_TIMEOUT_MS*2);
					//
					HAL_UART_Receive_DMA(&huart2, ucCom2RxBuf, UART_BUF_SIZE);
					osDelay(100);
					HAL_UART_DMAStop(&huart2);
					dmaCnt = 32 - __HAL_DMA_GET_COUNTER(huart2.hdmarx);
					for (i = 0; i < dmaCnt; i++)     //__HAL_DMA_GET_COUNTER(huart2.hdmarx); i++)
					{
						ucCom1TxBuf[i] = ucCom2RxBuf[i];
					}
					HAL_UART_Transmit(&huart1, ucCom1TxBuf, dmaCnt, UART_TIMEOUT_MS);
					clearBuf(ucCom1TxBuf, dmaCnt);            //__HAL_DMA_GET_COUNTER(huart2.hdmarx));
					__HAL_DMA_SET_COUNTER(huart2.hdmarx, UART_BUF_SIZE);

					osDelay(100);

					HAL_UART_Transmit(&huart2, rfidReadCmd, RFID_READ_CMD_LEN, RFID_TIMEOUT_MS*2);
					//
					HAL_UART_Receive_DMA(&huart2, ucCom2RxBuf, UART_BUF_SIZE);
					osDelay(100);
					HAL_UART_DMAStop(&huart2);
					dmaCnt = 32 - __HAL_DMA_GET_COUNTER(huart2.hdmarx);
					for (i = 0; i < dmaCnt; i++)     //__HAL_DMA_GET_COUNTER(huart2.hdmarx); i++)
					{
						ucCom1TxBuf[i] = ucCom2RxBuf[i];
					}
					HAL_UART_Transmit(&huart1, ucCom1TxBuf, dmaCnt, UART_TIMEOUT_MS);
					clearBuf(ucCom1TxBuf, dmaCnt);            //__HAL_DMA_GET_COUNTER(huart2.hdmarx));
					__HAL_DMA_SET_COUNTER(huart2.hdmarx, UART_BUF_SIZE);



					osDelay(100);
					HAL_UART_Transmit(&huart2, rfidStopCmd, RFID_STOP_CMD_LEN, RFID_TIMEOUT_MS);
					//
					HAL_UART_Receive_DMA(&huart2, ucCom2RxBuf, UART_BUF_SIZE);
					osDelay(300);
					HAL_UART_DMAStop(&huart2);
					dmaCnt = 32 - __HAL_DMA_GET_COUNTER(huart2.hdmarx);
					for (i = 0; i < dmaCnt; i++)     //__HAL_DMA_GET_COUNTER(huart2.hdmarx); i++)
					{
						ucCom1TxBuf[i] = ucCom2RxBuf[i];
					}
					HAL_UART_Transmit(&huart1, ucCom1TxBuf, dmaCnt, UART_TIMEOUT_MS);
					clearBuf(ucCom1TxBuf, dmaCnt);            //__HAL_DMA_GET_COUNTER(huart2.hdmarx));
					__HAL_DMA_SET_COUNTER(huart2.hdmarx, UART_BUF_SIZE);
				}
			}
			__HAL_DMA_SET_COUNTER(huart2.hdmarx, UART_BUF_SIZE);


		}

		osDelay(300);
		errorRpt(err);
		*/
		osDelay(3000);
	}
}

void clearBuf(uint8_t *buf, int cnt)
{
	int i;
	for (i = 0; i < cnt; i++)
	{
		buf[i] = 0;
	}
}

void errorRpt(uint32_t err)
{
	if (err != 0)
	{
		char strErrText[10] = "Error: ";
		char strErrCode[10]= "";
		char *strError = NULL;
		itoa(err, strErrCode, 10);
		//strcat(strError, "Error: ");
		strcat(strErrText, strErrCode);
		HAL_UART_Transmit(&huart1, strErrText, sizeof(strErrText), UART_TIMEOUT_MS);
		//clearBuf(strErrorCode, 10);
		//clearBuf(strError, 20);
	}
}

//set the wireless UART mode by
void setComMode(uint8_t ucMode)
{
	switch (ucMode)
	{
	case COM_MODE_NORMAL:
		HAL_GPIO_WritePin(doComMode0_GPIO_Port, doComMode0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(doComMode1_GPIO_Port, doComMode1_Pin, GPIO_PIN_RESET);
		osDelay(2);
		break;
	case COM_MODE_WAKEUP:
		HAL_GPIO_WritePin(doComMode0_GPIO_Port, doComMode0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(doComMode1_GPIO_Port, doComMode1_Pin, GPIO_PIN_SET);
		osDelay(2);
		break;
	case COM_MODE_SAVING:
		HAL_GPIO_WritePin(doComMode0_GPIO_Port, doComMode0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(doComMode1_GPIO_Port, doComMode1_Pin, GPIO_PIN_RESET);
		osDelay(2);
		break;
	case COM_MODE_SLEEP:
		HAL_GPIO_WritePin(doComMode0_GPIO_Port, doComMode0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(doComMode1_GPIO_Port, doComMode1_Pin, GPIO_PIN_SET);
		osDelay(2);
		break;
	default:
		break;
	}
}

