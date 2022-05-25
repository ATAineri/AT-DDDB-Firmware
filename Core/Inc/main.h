/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SSD1322_API.h"
#include "SSD1322_GFX.h"
#include "FreeMono12pt7b.h"
#include "FreeSansOblique9pt7b.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
enum DACs
{
	AK4493EQ,
	DSD1794A,
	BD34301EKV,
	CS43198
};

enum SamplingSpeed
{
	SS_48,
	SS_96,
	SS_192,
	SS_384
};

enum DsdSamplingSpeed
{
	DSD_64,
	DSD_128,
	DSD_256,
	DSD_512
};

enum AK4493EQ_FIRS
{
	SharpRollOff,
	SlowRollOff,
	ShortDelaySharpRollOff,
	ShortDelaySlowRollOff,
	SuperSlowRollOff,
	LowDispersionShortDelay = 0x6
};

enum CLOCKS
{
	C_44_1,
	C_48
};

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENC_CH_A_Pin GPIO_PIN_0
#define ENC_CH_A_GPIO_Port GPIOA
#define ENC_CH_A_EXTI_IRQn EXTI0_1_IRQn
#define ENC_BUT_Pin GPIO_PIN_1
#define ENC_BUT_GPIO_Port GPIOA
#define ENC_CH_B_Pin GPIO_PIN_2
#define ENC_CH_B_GPIO_Port GPIOA
#define ENC_CH_B_EXTI_IRQn EXTI2_3_IRQn
#define SPI1_RESET_Pin GPIO_PIN_3
#define SPI1_RESET_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define SPI1_SCK_Pin GPIO_PIN_5
#define SPI1_SCK_GPIO_Port GPIOA
#define SPI1_DC_Pin GPIO_PIN_6
#define SPI1_DC_GPIO_Port GPIOA
#define DISPLAY_MOSI_Pin GPIO_PIN_7
#define DISPLAY_MOSI_GPIO_Port GPIOA
#define DAC2_SCL_Pin GPIO_PIN_10
#define DAC2_SCL_GPIO_Port GPIOB
#define DAC2_SDA_Pin GPIO_PIN_11
#define DAC2_SDA_GPIO_Port GPIOB
#define DACV0_Pin GPIO_PIN_12
#define DACV0_GPIO_Port GPIOB
#define DACV1_Pin GPIO_PIN_13
#define DACV1_GPIO_Port GPIOB
#define DACV2_Pin GPIO_PIN_14
#define DACV2_GPIO_Port GPIOB
#define DACV3_Pin GPIO_PIN_15
#define DACV3_GPIO_Port GPIOB
#define BLOERO_MUX1_Pin GPIO_PIN_8
#define BLOERO_MUX1_GPIO_Port GPIOA
#define BLOERO_MUX1_EXTI_IRQn EXTI4_15_IRQn
#define BOLERO_MUX0_Pin GPIO_PIN_9
#define BOLERO_MUX0_GPIO_Port GPIOA
#define BOLERO_MUX0_EXTI_IRQn EXTI4_15_IRQn
#define BOLERO_Y1_Y2_Pin GPIO_PIN_10
#define BOLERO_Y1_Y2_GPIO_Port GPIOA
#define BOLERO_Y1_Y2_EXTI_IRQn EXTI4_15_IRQn
#define BOLERO_PCM_DSD_Pin GPIO_PIN_11
#define BOLERO_PCM_DSD_GPIO_Port GPIOA
#define BOLERO_PCM_DSD_EXTI_IRQn EXTI4_15_IRQn
#define BOLERO_DAC_RESET_Pin GPIO_PIN_12
#define BOLERO_DAC_RESET_GPIO_Port GPIOA
#define BOLERO_DAC_RESET_EXTI_IRQn EXTI4_15_IRQn
#define PROG_TX_Pin GPIO_PIN_14
#define PROG_TX_GPIO_Port GPIOA
#define PROG_RX_Pin GPIO_PIN_15
#define PROG_RX_GPIO_Port GPIOA
#define Y1_Pin GPIO_PIN_3
#define Y1_GPIO_Port GPIOB
#define Y2_Pin GPIO_PIN_4
#define Y2_GPIO_Port GPIOB
#define SMUTE_Pin GPIO_PIN_6
#define SMUTE_GPIO_Port GPIOB
#define DAC_RESET_Pin GPIO_PIN_7
#define DAC_RESET_GPIO_Port GPIOB
#define DAC_SCL_Pin GPIO_PIN_8
#define DAC_SCL_GPIO_Port GPIOB
#define DAC_SDA_Pin GPIO_PIN_9
#define DAC_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define MAX_VOLUME 63
#define I2C_TIMEOUT 1000

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
