/*
 * tx711ad.c
 *
 *  Created on: May 15, 2017
 *      Author: LHANG
 */

#include "hx711ad.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "freertos.h"
#include "cmsis_os.h"

uint32_t uaScaleCoef_A[];


volatile void vDelay(uint32_t uiDelay)
{
	uint32_t i, j;
	for (i = 0; i < uiDelay; i++)
	{
		for (j = 0; j < 10; j++)
			;
	}
	return;
}

uint32_t uiReadScalesA()
{
	uint8_t ucCnt;
	uint32_t result = 0, totalResult = 0;
	uint8_t i;
	uint8_t times = 4;

	for (i = 0; i < times; i++)
	{
		result = 0;
		resetScalesA_Clk();
		while (readScalesA_Data())
		{
			osDelay(10);
		}

		for (ucCnt = 0; ucCnt < 24; ucCnt++)
		{
			vDelay(8);
			vPortEnterCritical();
			setScalesA_Clk();
			vDelay(5);
			resetScalesA_Clk();
			vPortExitCritical();
			vDelay(5);                      //must less than 50uS
			result |= readScalesA_Data();
			result = result << 1;
		}
		vDelay(5);
		setScalesA_Clk();
		vDelay(5);
		resetScalesA_Clk();
		totalResult += result;

	}

	return totalResult/times;
}

uint32_t uiReadScalesB()
{
	uint8_t ucCnt;
	uint32_t result = 0, totalResult = 0;
	uint8_t i;
	uint8_t times = 4;

	for (i = 0; i < times; i++)
	{
		result = 0;
		resetScalesB_Clk();
		while (readScalesB_Data())
		{
			osDelay(10);
		}

		for (ucCnt = 0; ucCnt < 24; ucCnt++)
		{
			vDelay(5);
			vPortEnterCritical();
			setScalesB_Clk();
			vDelay(5);
			resetScalesB_Clk();
			vPortExitCritical();
			vDelay(5);
			result |= readScalesB_Data();
			result = result << 1;
		}
		vDelay(5);
		setScalesB_Clk();
		vDelay(5);
		resetScalesB_Clk();
		totalResult += result;

	}

	return totalResult/times;
}

uint32_t uiReadScalesC()
{
	uint8_t ucCnt;
	uint32_t result = 0, totalResult = 0;
	uint8_t i;
	uint8_t times = 4;

	for (i = 0; i < times; i++)
	{
		result = 0;
		resetScalesC_Clk();
		while (readScalesC_Data())
		{
			osDelay(10);
		}

		for (ucCnt = 0; ucCnt < 24; ucCnt++)
		{
			vDelay(5);
			vPortEnterCritical();
			setScalesC_Clk();
			vDelay(5);
			resetScalesC_Clk();
			vPortExitCritical();
			vDelay(5);
			result |= readScalesC_Data();
			result = result << 1;
		}
		vDelay(5);
		setScalesC_Clk();
		vDelay(5);
		resetScalesC_Clk();
		totalResult += result;

	}

	return totalResult/times;
}

