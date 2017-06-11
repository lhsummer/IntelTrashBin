/*
 * userSys.c
 *
 *  Created on: May 25, 2017
 *      Author: LHANG
 */

#include "userSys.h"
#include "w25qxx.h"
#include "freertos.h"
#include "cmsis_os.h"

#define LIMIT_RMV_NUM		2
#define TEMP_LOW_THRESHOLD		300
#define VOLT_LOW_THRESHOLD		300

uint32_t uiTempA[7] = {0, };
uint8_t ucTempCnt = 0;
uint32_t uiVoltA[7] = {0, };
uint8_t ucVoltCnt = 0;
uint32_t uiTemp = 0;
uint32_t uiVolt = 0;

uint8_t ucVersionBlock[VER_BLOCK_SIZE] = {0, };
uint32_t uiSystemVersion = 0;
uint32_t uiHwVersion = 0;
uint32_t uiSwVersion = 0;
uint8_t ucBoardInfo[24] = {0, };

uint8_t ucBoardID[16] = {0, };

uint8_t ucConfigBlock[CFG_BLOCK_SIZE] = {0, };
uint8_t ucLockDly = 10;
uint8_t ucSclRngA = 0;
uint8_t ucSclRngB = 0;
uint8_t ucSclRngC = 0;
uint8_t ucRfidTyp = 0;
uint8_t ucSclDly = 0;
uint8_t ucSclTimeStep = 0;
int16_t ihSclAY[SCL_SEG_NUM] = {0, };
int16_t ihSclBY[SCL_SEG_NUM] = {0, };
int16_t ihSclCY[SCL_SEG_NUM] = {0, };

int16_t ihSclCoefA[SCL_SEG_NUM][2] = {0, };
int16_t ihSclCoefB[SCL_SEG_NUM][2] = {0, };
int16_t ihSclCoefC[SCL_SEG_NUM][2] = {0, };


uint8_t ucAddrCh[COM_ADDR_CH_SIZE] = {0, };
uint8_t ucMainAddrCh[6] = {0, };
uint8_t ucSlaveAddrCh[6] = {0xFA, 0xAA, 0xEA, 0};
uint8_t ucQueryAddrCh[6] = {0, };
uint8_t ucShortID[2] = {0, };


void sortArray(uint32_t * numArray, uint32_t num)
{
	uint32_t i, j, k;
	for (j = 0; j < num; j++)
	{
		for (i = 0; i < num-j; i++)
		{
			if (numArray[i] > numArray[j])
			{
				k = numArray[i];
				numArray[i] = numArray[i+1];
				numArray[i+1] = k;
			}
		}
	}
}

void systemTask(void const * argument)
{
	osStatus osTempSta = osOK, osVoltSta = osOK;
	uint32_t uiTmpTemp, uiTmpVolt, i, iTemp, jTemp;
	uint8_t jVolt, iVolt;

	HAL_ADC_Start_IT(&hadc1);
	HAL_ADC_Start_IT(&hadc3);
	while (1)
	{
		if (osSemaphoreWait(tempADC_SemHandle, 0) == osOK )
		{
			HAL_ADC_Stop_IT(&hadc1);
			if (ucTempCnt < TEMP_READ_LENGTH)
			{
				uiTempA[ucTempCnt] = HAL_ADC_GetValue(&hadc1);
				ucTempCnt++;
			}
			else
			{
				sortArray(uiTempA, TEMP_READ_LENGTH);
				jTemp = TEMP_READ_LENGTH - LIMIT_RMV_NUM * 2;

				for (iTemp = LIMIT_RMV_NUM; iTemp < jTemp; iTemp++)
				{
					uiTmpTemp += uiTempA[i];
				}
				uiTemp = uiTmpTemp/jTemp;
				ucTempCnt = 0;
			}
			HAL_ADC_Start_IT(&hadc1);
		}

		if (osSemaphoreWait(voltADC_SemHandle, 0) == osOK )
		{
			HAL_ADC_Stop_IT(&hadc3);
			if (ucVoltCnt < VOLT_READ_LENGTH)
			{
				uiVoltA[ucVoltCnt] = HAL_ADC_GetValue(&hadc3);
				ucVoltCnt++;
			}
			else
			{
				sortArray(uiVoltA, VOLT_READ_LENGTH);
				jVolt = VOLT_READ_LENGTH - LIMIT_RMV_NUM * 2;

				for (iVolt = LIMIT_RMV_NUM; iVolt < jVolt; iVolt++)
				{
					uiTmpVolt += uiVoltA[i];
				}
				uiVolt = uiTmpVolt/jVolt;
				ucVoltCnt = 0;
			}
			HAL_ADC_Start_IT(&hadc3);
		}

		if (uiVolt < VOLT_LOW_THRESHOLD)
		{
			//goToSleep();
		}

	}
}

void systemInitialFromFlash (uint32_t uiData)
{
	uint32_t uiReturn, i, j;
	uiData = uiData;
	uiReturn = W25Qx_Read(ucVersionBlock, ADDR_VER_BASE, VER_BLOCK_SIZE);
	if (uiReturn != W25Qx_OK)
	{
		//error
	}

	uiReturn = W25Qx_Read(ucConfigBlock, ADDR_CFG, CFG_BLOCK_SIZE);
	if (uiReturn != W25Qx_OK)
	{
		//error
	}

	uiReturn = W25Qx_Read(ucAddrCh, ADDR_COM_ADDR_CH, COM_ADDR_CH_SIZE);
	if (uiReturn != W25Qx_OK)
	{
		//error
	}

	uiSystemVersion = *((uint32_t *) (ucVersionBlock));
	uiHwVersion = *((uint32_t *) (ucVersionBlock + 0x04));
	uiSwVersion = *((uint32_t *) (ucVersionBlock + 0x08));

	for (i = 0; i < 24; i++)
		ucBoardInfo[i] = ucVersionBlock[0x0C + i];

	for (i = 0; i < 16; i++)
		ucBoardID[i] = ucVersionBlock[0x24 + i];

	ucLockDly = ucConfigBlock[0];
	ucSclRngA = ucConfigBlock[1];
	ucSclRngB = ucConfigBlock[2];
	ucSclRngC = ucConfigBlock[3];
	ucRfidTyp = ucConfigBlock[4];
	ucSclDly = ucConfigBlock[5];
	ucSclTimeStep = ucConfigBlock[6];

	for (i = 0; i < SCL_SEG_NUM; i++)
		ihSclAY[i] = *((int16_t *) (ucConfigBlock + ADDR_SCLA_Y + 2*i));
	for (i = 0; i < SCL_SEG_NUM; i++)
	{
		for(j = 0; j < 2; j++)
		{
			ihSclCoefA[i][j] = *((int16_t *) (ucConfigBlock + ADDR_SCLA_COEF_OFFSET + i * 2 + j));
		}
	}

	for (i = 0; i < SCL_SEG_NUM; i++)
		ihSclBY[i] = *((int16_t *) (ucConfigBlock + ADDR_SCLB_Y + 2*i));
	for (i = 0; i < SCL_SEG_NUM; i++)
	{
		for(j = 0; j < 2; j++)
		{
			ihSclCoefB[i][j] = *((int16_t *) (ucConfigBlock + ADDR_SCLB_COEF_OFFSET + i * 2 + j));
		}
	}

	for (i = 0; i < SCL_SEG_NUM; i++)
		ihSclCY[i] = *((int16_t *) (ucConfigBlock + ADDR_SCLC_Y + 2*i));
	for (i = 0; i < SCL_SEG_NUM; i++)
	{
		for(j = 0; j < 2; j++)
		{
			ihSclCoefC[i][j] = *((int16_t *) (ucConfigBlock + ADDR_SCLC_COEF_OFFSET + i * 2 + j));
		}
	}

	for (i = 0; i < 6; i++)
	{
		ucMainAddrCh[i] = ucAddrCh[i];
		ucSlaveAddrCh[i] = ucAddrCh[6 + i];
		ucQueryAddrCh[i] = ucAddrCh[12 + i];
	}

	for (i = 0; i < 2; i++)
	{
		ucShortID[i] = ucAddrCh[18 + i];
	}
}

void systemToFlash (uint32_t uiData)
{
	uint32_t uiReturn, i, j;
	uiData = uiData;

	*((uint32_t *) (ucVersionBlock)) = uiSystemVersion;
	*((uint32_t *) (ucVersionBlock + 0x04)) = uiHwVersion;
	*((uint32_t *) (ucVersionBlock + 0x08)) = uiSwVersion;

	for (i = 0; i < 24; i++)
		ucVersionBlock[0x0C + i] = ucBoardInfo[i];

	for (i = 0; i < 16; i++)
		ucVersionBlock[0x24 + i] = ucBoardID[i];

	ucConfigBlock[0] = ucLockDly;
	ucConfigBlock[1] = ucSclRngA;
	ucConfigBlock[2] = ucSclRngB;
	ucConfigBlock[3] = ucSclRngC;
	ucConfigBlock[4] = ucRfidTyp;
	ucConfigBlock[5] = ucSclDly;
	ucConfigBlock[6] = ucSclTimeStep;

	for (i = 0; i < SCL_SEG_NUM; i++)
		*((int16_t *) (ucConfigBlock + ADDR_SCLA_Y + 2*i)) = ihSclAY[i];
	for (i = 0; i < SCL_SEG_NUM; i++)
	{
		for(j = 0; j < 2; j++)
		{
			*((int16_t *) (ucConfigBlock + ADDR_SCLA_COEF_OFFSET + i * 2 + j)) = ihSclCoefA[i][j];
		}
	}

	for (i = 0; i < SCL_SEG_NUM; i++)
		*((int16_t *) (ucConfigBlock + ADDR_SCLB_Y + 2*i)) = ihSclBY[i];
	for (i = 0; i < SCL_SEG_NUM; i++)
	{
		for(j = 0; j < 2; j++)
		{
			*((int16_t *) (ucConfigBlock + ADDR_SCLB_COEF_OFFSET + i * 2 + j)) = ihSclCoefB[i][j];
		}
	}

	for (i = 0; i < SCL_SEG_NUM; i++)
		*((int16_t *) (ucConfigBlock + ADDR_SCLC_Y + 2*i)) = ihSclCY[i];
	for (i = 0; i < SCL_SEG_NUM; i++)
	{
		for(j = 0; j < 2; j++)
		{
			*((int16_t *) (ucConfigBlock + ADDR_SCLC_COEF_OFFSET + i * 2 + j)) = ihSclCoefC[i][j];
		}
	}

	for (i = 0; i < 6; i++)
	{
		ucAddrCh[i] = ucMainAddrCh[i];
		ucAddrCh[6 + i] = ucSlaveAddrCh[i];
		ucAddrCh[12 + i] = ucQueryAddrCh[i];
	}

	for (i = 0; i < 2; i++)
	{
		ucAddrCh[18 + i] = ucShortID[i];
	}

	uiReturn = W25Qx_Write(ucVersionBlock, ADDR_VER_BASE, VER_BLOCK_SIZE);
	if (uiReturn != W25Qx_OK)
	{
		//error
	}

	uiReturn = W25Qx_Write(ucConfigBlock, ADDR_CFG, CFG_BLOCK_SIZE);
	if (uiReturn != W25Qx_OK)
	{
		//error
	}

	uiReturn = W25Qx_Write(ucAddrCh, ADDR_COM_ADDR_CH, COM_ADDR_CH_SIZE);
	if (uiReturn != W25Qx_OK)
	{
		//error
	}

}


