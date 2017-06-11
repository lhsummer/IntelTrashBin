/*
 * userRfid.c
 *
 *  Created on: Jun 11, 2017
 *      Author: LHANG
 */


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
