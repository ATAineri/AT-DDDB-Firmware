/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f0xx_it.c
 * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern uint8_t volume;
extern uint32_t progtime;
extern uint8_t ak4493eq_fir;
extern uint8_t dsd1794a_fir;
extern uint8_t bd34301ekv_fir;

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

extern SPI_HandleTypeDef hspi1;

extern uint8_t tx_buf[];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void itoa(long i, char *string)
{
	int power = 0, j = 0;

	j = i;
	for (power = 1; j>10; j /= 10)
		power *= 10;

	for (; power>0; power /= 10)
	{
		*string++ = '0' + i / power;
		i %= power;
	}
	*string = '\0';
}

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
	while (1)
	{
	}
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line 0 and 1 interrupts.
  */
void EXTI0_1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_1_IRQn 0 */
	if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))
		if (volume < MAX_VOLUME)
		{
			volume++;

			select_font(&FreeMono12pt7b);
			char volume_string[2]; itoa(volume, volume_string);
			draw_text(tx_buf, volume_string, 192, 38, 15);
			send_buffer_to_OLED(tx_buf, 0, 0);
			HAL_Delay(2000);

			progtime = HAL_GetTick();
		}

  /* USER CODE END EXTI0_1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_1_IRQn 1 */

  /* USER CODE END EXTI0_1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line 2 and 3 interrupts.
  */
void EXTI2_3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_3_IRQn 0 */
	if ((HAL_GetTick() - progtime) > 50)
		if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))
			if (volume > 0)
			{
				volume--;

				select_font(&FreeMono12pt7b);
				char volume_string[2]; itoa(volume, volume_string);
				draw_text(tx_buf, volume_string, 192, 38, 15);
				send_buffer_to_OLED(tx_buf, 0, 0);
				HAL_Delay(2000);

				progtime = HAL_GetTick();
			}
  /* USER CODE END EXTI2_3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_3_IRQn 1 */

  /* USER CODE END EXTI2_3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line 4 to 15 interrupts.
  */
void EXTI4_15_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_15_IRQn 0 */
	uint8_t dacv = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)) | (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) << 1) | (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) << 2) | (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) << 3);

	uint8_t frame[2], size = 2;
	uint8_t pcm_dsd = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);

	uint8_t boleroSampleSpeed = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)) | (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) << 1);
	uint8_t dfs;

	if (pcm_dsd)
	{
		select_font(&FreeMono12pt7b);

		switch (boleroSampleSpeed)
		{
		case SS_48:
			dfs = 0x0;

			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == C_44_1)
				draw_text(tx_buf, "PCM  44.1KHz", 64, 32, 15);
			else
				draw_text(tx_buf, "PCM    48KHz", 64, 20, 15);
			send_buffer_to_OLED(tx_buf, 0, 0);
			HAL_Delay(2000);

			break;
		case SS_96:
			dfs = 0x1;

			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == C_44_1)
				draw_text(tx_buf, "PCM  88.2KHz", 64, 32, 15);
			else
				draw_text(tx_buf, "PCM    96KHz", 64, 20, 15);
			send_buffer_to_OLED(tx_buf, 0, 0);
			HAL_Delay(2000);

			break;
		case SS_192:
			dfs = 0x2;

			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == C_44_1)
				draw_text(tx_buf, "PCM 176.4KHz", 64, 32, 15);
			else
				draw_text(tx_buf, "PCM   192KHz", 64, 20, 15);
			send_buffer_to_OLED(tx_buf, 0, 0);
			HAL_Delay(2000);

			break;
		case SS_384:
			dfs = 0x4;

			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == C_44_1)
				draw_text(tx_buf, "PCM 352.8KHz", 64, 32, 15);
			else
				draw_text(tx_buf, "PCM   384KHz", 64, 20, 15);
			send_buffer_to_OLED(tx_buf, 0, 0);
			HAL_Delay(2000);

			break;
		}
	}
	else
	{
		dfs = boleroSampleSpeed;

		select_font(&FreeMono12pt7b);
		switch(boleroSampleSpeed)
		{
		case DSD_64:
			draw_text(tx_buf, "DSD 64", 64, 32, 15);
			break;
		case DSD_128:
			draw_text(tx_buf, "DSD 128", 64, 32, 15);
			break;
		case DSD_256:
			draw_text(tx_buf, "DSD 256", 64, 32, 15);
			break;
		case DSD_512:
			draw_text(tx_buf, "DSD 512", 64, 32, 15);
			break;
		}
		send_buffer_to_OLED(tx_buf, 0, 0);
		HAL_Delay(2000);
	}

	switch (dacv)
	{
	case AK4493EQ:  	// addrs: 0x10, 0x11
		// Control 1 Reg: Reset
		frame[0] = 0x0; frame[1] = 0xE;
		HAL_I2C_Master_Transmit(&hi2c1, 0x10, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x10, frame, size, I2C_TIMEOUT);

		// Control 2 Reg
		frame[0] = 0x1; frame[1] = 0x3 | ((dfs & 0x3) << 3) | ((ak4493eq_fir & 0x2) << 5);
		HAL_I2C_Master_Transmit(&hi2c1, 0x10, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x10, frame, size, I2C_TIMEOUT);

		// Control 3 Reg
		frame[0] = 0x2; frame[1] = 0x8 | (ak4493eq_fir & 0x1) | ((!pcm_dsd) << 7);
		HAL_I2C_Master_Transmit(&hi2c1, 0x10, frame, size, I2C_TIMEOUT);
		frame[1] = 0xA | (ak4493eq_fir & 0x1) | ((!pcm_dsd) << 7);
		HAL_I2C_Master_Transmit(&hi2c2, 0x10, frame, size, I2C_TIMEOUT);

		// Control 4 Reg
		frame[0] = 0x5; frame[1] = ((ak4493eq_fir & 0x4) >> 2) | ((dfs & 0x4) >> 1);
		HAL_I2C_Master_Transmit(&hi2c1, 0x10, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x10, frame, size, I2C_TIMEOUT);

		// DSD1 Reg
		frame[0] = 0x6; frame[1] = 0x2 | (dfs & 0x1);
		HAL_I2C_Master_Transmit(&hi2c1, 0x10, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x10, frame, size, I2C_TIMEOUT);

		// DSD2 Reg
		frame[0] = 0x9; frame[1] = dfs >> 1;
		HAL_I2C_Master_Transmit(&hi2c1, 0x10, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x10, frame, size, I2C_TIMEOUT);

		// Control 2 Reg
		frame[0] = 0x1; frame[1] = 0x2 | ((dfs & 0x3) << 3);
		HAL_I2C_Master_Transmit(&hi2c1, 0x10, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x10, frame, size, I2C_TIMEOUT);

		// Control 1 Reg: Release Reset
		frame[0] = 0x0; frame[1] = 0xF;
		HAL_I2C_Master_Transmit(&hi2c1, 0x10, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x10, frame, size, I2C_TIMEOUT);

		break;
	case DSD1794A:  	// addrs: 0x4C, 0x4D
		// Set MUTE
		frame[0] = 0x18; frame[1] = 0x51;
		HAL_I2C_Master_Transmit(&hi2c1, 0x4C, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x4C, frame, size, I2C_TIMEOUT);

		// OFF the DACs
		frame[0] = 0x19; frame[1] = 0x10 | (dsd1794a_fir << 1);
		HAL_I2C_Master_Transmit(&hi2c1, 0x4C, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x4C, frame, size, I2C_TIMEOUT);

		// Setting DUAL MONO, setting
		if (!dfs)
			dfs = 0x2;
		else
			if (dfs == 1)
				dfs = 0x0;
			else
				dfs = 0x1;
		frame[0] = 0x20; frame[1] = 0x8 | (!pcm_dsd << 5) | dfs;
		HAL_I2C_Master_Transmit(&hi2c1, 0x4C, frame, size, I2C_TIMEOUT);
		frame[1] = 0xC | (!pcm_dsd << 5) | dfs;
		HAL_I2C_Master_Transmit(&hi2c2, 0x4C, frame, size, I2C_TIMEOUT);

		// ON the DACs
		frame[0] = 0x19; frame[1] = dsd1794a_fir << 1;
		HAL_I2C_Master_Transmit(&hi2c1, 0x4C, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x4C, frame, size, I2C_TIMEOUT);

		// Release MUTE
		frame[0] = 0x18; frame[1] = 0x50;
		HAL_I2C_Master_Transmit(&hi2c1, 0x4C, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x4C, frame, size, I2C_TIMEOUT);

		break;
	case BD34301EKV:  	// addrs: 0x38, 0x3A
		// MUTE ON
		frame[0] = 0x2A; frame[1] = 0x0;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// Digital Power OFF
		frame[0] = 0x2; frame[1] = 0x0;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// Software Reset ON
		frame[0] = 0x0; frame[1] = 0x0;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// DSD ON/OFF
		frame[0] = 0x10; frame[1] = 0xB | (!pcm_dsd << 7);
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// DSD filter selecting
		if (!dfs)
			frame[1] = 0x2;
		else if (dfs == 1)
			frame[1] = 0x1;
		else
			frame[1] = 0x0;
		frame[0] = 0x16;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// Sample Rate configuring
		dfs <<= 1;
		frame[0] = 0x30; frame[1] = 0x8;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);
		dfs >>= 1;

		switch (boleroSampleSpeed)
		{
		case SS_48:
			frame[1] = bd34301ekv_fir ? 0x3 : 0x0;
			break;
		case SS_96:
			frame[1] = bd34301ekv_fir ? 0x4 : 0x1;
			break;
		case SS_192:
			frame[1] = bd34301ekv_fir ? 0x5 : 0x2;
			break;
		case SS_384:
			frame[1] = 0x0;
			break;
		}
		frame[0] = 0x31;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		if (pcm_dsd)
			frame[1] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) ? 0x11 : 0x10;
		else
			frame[1] = 0x2;
		frame[0] = 0x40;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// PCM / DSD configuring
		frame[0] = 0x60;
		frame[1] = pcm_dsd ? 0x16 : 0x9E;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		frame[0] = 0x61;
		frame[1] = pcm_dsd ? 0x16 : 0x1E;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// Software Reset OFF
		frame[0] = 0x0; frame[1] = 0x1;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// Digital Power ON
		frame[0] = 0x2; frame[1] = 0x1;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// RAM clear ON
		frame[0] = 0x2F; frame[1] = 0x80;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// RAM clear release
		frame[0] = 0x2F; frame[1] = 0x0;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// MUTE release
		frame[0] = 0x2A; frame[1] = 0x3;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		break;
	case AK4499EX:	// ADDRS: 0x20, 0x21
		// Control 1 Reg: Reset
		frame[0] = 0x0; frame[1] = 0xE;
		HAL_I2C_Master_Transmit(&hi2c1, 0x20, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x20, frame, size, I2C_TIMEOUT);

		// Control 2 Reg: filters
		frame[0] = 0x1; frame[1] = (ak4493eq_fir << 3) | 0x3;
		HAL_I2C_Master_Transmit(&hi2c1, 0x20, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x20, frame, size, I2C_TIMEOUT);

		// Control 3 Reg: DSD/PCM
		frame[0] = 0x2; frame[1] = (!pcm_dsd) << 7;
		HAL_I2C_Master_Transmit(&hi2c1, 0x20, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x20, frame, size, I2C_TIMEOUT);

		// Control 4 Reg: Sample Speed
		frame[0] = 0x5; frame[1] = dfs << 1;
		HAL_I2C_Master_Transmit(&hi2c1, 0x20, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x20, frame, size, I2C_TIMEOUT);

		// DSD1 Reg: DSD Sample SPeed
		frame[0] = 0x6; frame[1] = 0x8 | dfs;
		HAL_I2C_Master_Transmit(&hi2c1, 0x20, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x20, frame, size, I2C_TIMEOUT);

		// Control 2 Reg: filters
		frame[0] = 0x1; frame[1] = (ak4493eq_fir << 3) | 0x2;
		HAL_I2C_Master_Transmit(&hi2c1, 0x20, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x20, frame, size, I2C_TIMEOUT);

		// Control 1 Reg: Reset
		frame[0] = 0x0; frame[1] = 0xF;
		HAL_I2C_Master_Transmit(&hi2c1, 0x20, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x20, frame, size, I2C_TIMEOUT);

		break;
	case CS43198:		// addrs: 0x30, 0x31
		uint8_t *cs_frame[5];
		const uint8_t cs_size = 5;

		// 1. Power down amplifier
		frame[0] = 0x2; frame[1] = 0x00; frame[2] = 0x00;
		frame[3] = 0x0;
		frame[4] = 0xB6;
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		HAL_Delay(300);

		// 2. Power down ASP
		frame[0] = 0x2; frame[1] = 0x00; frame[2] = 0x00;
		frame[3] = 0x0;
		frame[4] = 0xF6;
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		// 3. Set MCLK Source to RCO
		frame[0] = 0x1; frame[1] = 0x00; frame[2] = 0x06;
		frame[3] = 0x0;
		frame[4] = 0x6;
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		HAL_Delay(2);

		// 6. Change MCLK_INT frequency
		frame[0] = 0x1; frame[1] = 0x00; frame[2] = 0x06;
		frame[3] = 0x0;
		frame[4] = ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == C_44_1 ? 0x1 : 0x0) << 2) | 0x2;
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		// 8. Switch MCLK source to direct MCLK mode
		frame[0] = 0x1; frame[1] = 0x00; frame[2] = 0x06;
		frame[3] = 0x0;
		frame[4] = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == C_44_1 ? 0x1 : 0x0) << 2;
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		frame[0] = 0x1; frame[1] = 0x00; frame[2] = 0x0B;
		frame[3] = 0x0;
		switch (boleroSampleSpeed)
		{
		case SS_48:
			frame[4] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == C_44_1 ? 0x1 : 0x2;
			break;
		case SS_96:
			frame[4] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == C_44_1 ? 0x3 : 0x4;
			break;
		case SS_192:
			frame[4] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == C_44_1 ? 0x5 : 0x6;
			break;
		case SS_384:
			frame[4] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == C_44_1 ? 0x7 : 0x8;
			break;
		}
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		// Configure DSD path Signal Control 2
		frame[0] = 0x7; frame[1] = 0x00; frame[2] = 0x04;
		frame[3] = 0x0;
		switch (boleroSampleSpeed)
		{
		case DSD_64:
			frame[4] = 0x82;
			break;
		case DSD_128:
			frame[4] = 0x86;
			break;
		case DSD_256:
			frame[4] = 0x0A;
			break;
		case DSD_512:
			break;
		}
		frame[4] |= !pcm_dsd << 0x4;
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		HAL_Delay(2);

		// 38. Power up DAC
		frame[0] = 0x1; frame[1] = 0x00; frame[2] = 0x10;
		frame[3] = 0x0;
		frame[4] = 0x99;
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		frame[0] = 0x8; frame[1] = 0x00; frame[2] = 0x32;
		frame[3] = 0x0;
		frame[4] = 0x20;
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		frame[0] = 0x2; frame[1] = 0x00; frame[2] = 0x00;
		frame[3] = 0x0;
		frame[4] = 0xF6 & ~(pcm_dsd << 6) & ~(!pcm_dsd << 5);
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		frame[0] = 0x2; frame[1] = 0x00; frame[2] = 0x00;
		frame[3] = 0x0;
		frame[4] = 0xE6 & ~(pcm_dsd << 6) & ~(!pcm_dsd << 5);;
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		frame[0] = 0x8; frame[1] = 0x00; frame[2] = 0x32;
		frame[3] = 0x0;
		frame[4] = 0x0;
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		frame[0] = 0x1; frame[1] = 0x00; frame[2] = 0x10;
		frame[3] = 0x0;
		frame[4] = 0x99;
		HAL_I2C_Master_Transmit(&hi2c1, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c1, 0x31, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x30, cs_frame, cs_size);
		HAL_I2C_Master_Transmit(&hi2c2, 0x31, cs_frame, cs_size);

		break;
	}

  /* USER CODE END EXTI4_15_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
  /* USER CODE BEGIN EXTI4_15_IRQn 1 */

  /* USER CODE END EXTI4_15_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
