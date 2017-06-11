/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define dohIndicatorA_Pin GPIO_PIN_2
#define dohIndicatorA_GPIO_Port GPIOF
#define dohIndicatorB_Pin GPIO_PIN_3
#define dohIndicatorB_GPIO_Port GPIOF
#define aiMainPwMonitor_Pin GPIO_PIN_6
#define aiMainPwMonitor_GPIO_Port GPIOF
#define coreLED1_Pin GPIO_PIN_0
#define coreLED1_GPIO_Port GPIOC
#define keyInput1_Pin GPIO_PIN_0
#define keyInput1_GPIO_Port GPIOA
#define dilFullDetectA_Pin GPIO_PIN_1
#define dilFullDetectA_GPIO_Port GPIOA
#define rfidA_Tx_Pin GPIO_PIN_2
#define rfidA_Tx_GPIO_Port GPIOA
#define rfidA_Rx_Pin GPIO_PIN_3
#define rfidA_Rx_GPIO_Port GPIOA
#define scalesA_CLK_Pin GPIO_PIN_4
#define scalesA_CLK_GPIO_Port GPIOC
#define scalesA_DIO_Pin GPIO_PIN_5
#define scalesA_DIO_GPIO_Port GPIOC
#define dohSoundA1_Pin GPIO_PIN_0
#define dohSoundA1_GPIO_Port GPIOB
#define dohSoundA2_Pin GPIO_PIN_1
#define dohSoundA2_GPIO_Port GPIOB
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define keyInput2_Pin GPIO_PIN_11
#define keyInput2_GPIO_Port GPIOF
#define dilScalesA_Act_Pin GPIO_PIN_12
#define dilScalesA_Act_GPIO_Port GPIOF
#define dohUnlockA_Pin GPIO_PIN_13
#define dohUnlockA_GPIO_Port GPIOF
#define dohDebugPwCtrl_Pin GPIO_PIN_0
#define dohDebugPwCtrl_GPIO_Port GPIOG
#define dohScalesPwCtrl_Pin GPIO_PIN_1
#define dohScalesPwCtrl_GPIO_Port GPIOG
#define dohRfidPwCtrl_Pin GPIO_PIN_7
#define dohRfidPwCtrl_GPIO_Port GPIOE
#define dohSoundPwCtrl_Pin GPIO_PIN_8
#define dohSoundPwCtrl_GPIO_Port GPIOE
#define flashCs_Pin GPIO_PIN_12
#define flashCs_GPIO_Port GPIOB
#define flashClk_Pin GPIO_PIN_13
#define flashClk_GPIO_Port GPIOB
#define flashMiso_Pin GPIO_PIN_14
#define flashMiso_GPIO_Port GPIOB
#define flashMosi_Pin GPIO_PIN_15
#define flashMosi_GPIO_Port GPIOB
#define rfidB_Tx_Pin GPIO_PIN_8
#define rfidB_Tx_GPIO_Port GPIOD
#define rfidB_Rx_Pin GPIO_PIN_9
#define rfidB_Rx_GPIO_Port GPIOD
#define scalesB_CLK_Pin GPIO_PIN_10
#define scalesB_CLK_GPIO_Port GPIOD
#define scalesB_DIO_Pin GPIO_PIN_11
#define scalesB_DIO_GPIO_Port GPIOD
#define dilFullDetectB_Pin GPIO_PIN_12
#define dilFullDetectB_GPIO_Port GPIOD
#define dohSoundB1_Pin GPIO_PIN_13
#define dohSoundB1_GPIO_Port GPIOD
#define dohSoundB2_Pin GPIO_PIN_14
#define dohSoundB2_GPIO_Port GPIOD
#define dohUnlockB_Pin GPIO_PIN_15
#define dohUnlockB_GPIO_Port GPIOD
#define dilScalesB_Act_Pin GPIO_PIN_2
#define dilScalesB_Act_GPIO_Port GPIOG
#define dilDebugAct_Pin GPIO_PIN_6
#define dilDebugAct_GPIO_Port GPIOG
#define doComMode1_Pin GPIO_PIN_7
#define doComMode1_GPIO_Port GPIOG
#define doComMode0_Pin GPIO_PIN_8
#define doComMode0_GPIO_Port GPIOG
#define exitlComAux_Pin GPIO_PIN_8
#define exitlComAux_GPIO_Port GPIOA
#define debugComTx_Pin GPIO_PIN_9
#define debugComTx_GPIO_Port GPIOA
#define debugComRx_Pin GPIO_PIN_10
#define debugComRx_GPIO_Port GPIOA
#define coreLED2_Pin GPIO_PIN_3
#define coreLED2_GPIO_Port GPIOD
#define dilScalesC_Act_Pin GPIO_PIN_7
#define dilScalesC_Act_GPIO_Port GPIOD
#define rfidC_Rx_Pin GPIO_PIN_9
#define rfidC_Rx_GPIO_Port GPIOG
#define scalesC_CLK_Pin GPIO_PIN_10
#define scalesC_CLK_GPIO_Port GPIOG
#define scalesC_DIO_Pin GPIO_PIN_11
#define scalesC_DIO_GPIO_Port GPIOG
#define dilFullDetectC_Pin GPIO_PIN_12
#define dilFullDetectC_GPIO_Port GPIOG
#define dohSoundC1_Pin GPIO_PIN_13
#define dohSoundC1_GPIO_Port GPIOG
#define rfidC_Tx_Pin GPIO_PIN_14
#define rfidC_Tx_GPIO_Port GPIOG
#define dohSoundC2_Pin GPIO_PIN_15
#define dohSoundC2_GPIO_Port GPIOG
#define dohUnlockC_Pin GPIO_PIN_5
#define dohUnlockC_GPIO_Port GPIOB
#define eepromSCL_Pin GPIO_PIN_6
#define eepromSCL_GPIO_Port GPIOB
#define eepromSDA_Pin GPIO_PIN_7
#define eepromSDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
