#include "userCom.h"
#include "stdint.h"
#include "freeRTOS.h"
#include "cmsis_os.h"

#define MAX_LOOP			3

#define LOOPA				0
#define LOOPB				1
#define LOOPC				2

#define COEF_A				0
#define COEF_B				1

#define RDY_FLAG_ADDR			0x0
#define VERSION_ADDR			0x100
#define INFORMATION_ADDR		0x10C
#define INFORMATION_LEN			24
#define BOD_ID_ADDR				0x124
#define BOD_ID_LEN				10
#define CFG_ADDR				0x200
#define SCALE_CALIBRATE_ADDR	0x247
#define ADDR_CH_ADDR			0x300

#define ERROR_CODE_ADDR			0x400
#define DATA_ADDR				0x800

//Queue
extern osMessageQId comRxBufQHandle;
extern osMessageQId userIDQueueHandle;
extern osMessageQId qualifiedIDQueueHandle;
extern osMessageQId infoIDQueueHandle;
extern osMessageQId comTxBufQHandle;
extern osMessageQId cmdQueueHandle;

uint8_t ucaRdyFlag[3] = {0, };
uint8_t comRxBuf[RX_BUF_SIZE][RX_FRAME_LENGTH] = {0, };
uint8_t ucCurrentCnt, ucCnt;

uint8_t	ucaScaleRang[MAX_LOOP] = {70, 70, 70};
uint8_t	ucRfidType = 1;

uint16_t uhaScaleY[MAX_LOOP][5] = {0, };
uint16_t uhaScaleCoef[MAX_LOOP][2][4] = {0, };

uint16_t uhaAddr[6] = {0, };                   //need to be updated to default addr
uint8_t ucaChannel[6] = {0, };				  //need to be updated to default channel

void cmdExeTask(void const * argument);

void userUart1Handler(UART_HandleTypeDef *huart)
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

/* cmdExeTask function */
void cmdExeTask(void const * argument)
{
	osEvent event;
	osStatus err = osOK;
	for(;;)
	{
		event = osMessageGet (comRxBufQHandle, osWaitForever);
		//osEvent osMessageGet (osMessageQId queue_id, uint32_t millisec)
		osDelay(100);
	}
	/* USER CODE END cmdExeTask */
}

void comHandle(uint8_t cmd)
{
    uint32_t i, j, k;
    switch (cmd)
    {
        case CMD_QUERY:
        	if ((comRxBuf[6] == comRxBuf[7]) && (comRxBuf[6] == comRxBuf[8]))
        	{
        		for (i = 1; i <= 5; i++)
        		{
        			//if (comRxBuf[i] = )
        		}
        	}
        	break;
        case CMD_CHG_PATH:
        	break;
        case CMD_CLR_ERR:
        	break;
        default:
        	break;
    }
}

void numToAscii(uint64_t ulNum, uint8_t ucArry[], uint8_t ucLen)
{
	uint8_t i, ucRem;
	uint64_t ulTemp, ulQuot;

	for (i = 0; i < ucLen; i++)
	{
		ulQuot = ulNum % 10;
		ulNum = ulNum / 10;
		ucArry[ucLen - i - 1] = ulQuot + 0x30;
	}
}

uint64_t asciiToNum(uint8_t ucArry[], uint8_t ucLen)
{
	uint8_t i;
	uint64_t ulNum = 0;
	for (i = 0; i < ucLen; i++)
	{
		if ((ucArry[i] > 0x30) && (ucArry[i] < 0x39))
		{
			ulNum += ucArry[i] - 0x30;
			if (i == (ucLen - 1))
				;
			else
				ulNum = ulNum * 10;
		}
		else
		{
			return 0;
		}
	}
	return ulNum;
}



