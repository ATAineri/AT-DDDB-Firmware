/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define AK4403EQ_FIRS_NUM 6

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

uint8_t volume;
uint32_t progtime;
uint8_t ak4493eq_fir;
uint8_t dsd1794a_fir;
uint8_t bd34301ekv_fir;

uint8_t tx_buf[256 * 64 / 2];

const char AK4493EQ_FIRS_NAMES[6][16]   = {"Sharp", "Slow", "SD Sharp", "SD Slow", "Super Slow", "Low Dispersion"};
const char DSD1794A_FIRS_NAMES[2][16]   = {"Sharp", "Slow"};
const char BD34301EKV_FIRS_NAMES[2][16] = {"Sharp", "Slow"};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void Set_Clock(uint8_t clock);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	volume = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

	SSD1322_API_init();
	set_buffer_size(256, 64);
	fill_buffer(tx_buf, 0);
	select_font(&FreeMono12pt7b);
	draw_text(tx_buf, "VOLUME:", 64, 38, 15);
	send_buffer_to_OLED(tx_buf, 0, 0);
	HAL_Delay(2000);


  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

	progtime = HAL_GetTick();
	Set_Clock(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10));

	SystemClock_Config();

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
	select_font(&FreeSansOblique9pt7b);
	draw_text(tx_buf, "AK4493EQ", 9, 5, 15);
	send_buffer_to_OLED(tx_buf, 0, 0);
	HAL_Delay(2000);

	// Control 1 Reg: Reset
	frame[0] = 0x0; frame[1] = 0xE;
	HAL_I2C_Master_Transmit(&hi2c1, 0x10, frame, size, I2C_TIMEOUT);
	HAL_I2C_Master_Transmit(&hi2c2, 0x10, frame, size, I2C_TIMEOUT);

	// Control 2 Reg
	frame[0] = 0x1; frame[1] = 0x3 | ((dfs & 0x3) << 3);
	HAL_I2C_Master_Transmit(&hi2c1, 0x10, frame, size, I2C_TIMEOUT);
	HAL_I2C_Master_Transmit(&hi2c2, 0x10, frame, size, I2C_TIMEOUT);

	// Control 3 Reg
	frame[0] = 0x2; frame[1] = 0x9 | ((!pcm_dsd) << 7);
	HAL_I2C_Master_Transmit(&hi2c1, 0x10, frame, size, I2C_TIMEOUT);
	frame[1] = 0xB | ((!pcm_dsd) << 7);
	HAL_I2C_Master_Transmit(&hi2c2, 0x10, frame, size, I2C_TIMEOUT);

	// Control 4 Reg
	frame[0] = 0x5; frame[1] = ((dfs & 0x4) >> 1);
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
		select_font(&FreeSansOblique9pt7b);
		draw_text(tx_buf, "DSD1794A", 9, 5, 15);
		send_buffer_to_OLED(tx_buf, 0, 0);
		HAL_Delay(2000);

		// Set MUTE
		frame[0] = 0x18; frame[1] = 0x51;
		HAL_I2C_Master_Transmit(&hi2c1, 0x4C, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x4C, frame, size, I2C_TIMEOUT);

		// OFF the DACs
		frame[0] = 0x19; frame[1] = 0x12;
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
		frame[0] = 0x19; frame[1] = 0x2;
		HAL_I2C_Master_Transmit(&hi2c1, 0x4C, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x4C, frame, size, I2C_TIMEOUT);

		// Release MUTE
		frame[0] = 0x18; frame[1] = 0x50;
		HAL_I2C_Master_Transmit(&hi2c1, 0x4C, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x4C, frame, size, I2C_TIMEOUT);

		break;
	case BD34301EKV:  // addrs: 0x38, 0x3A
		select_font(&FreeSansOblique9pt7b);
		draw_text(tx_buf, "BD34301EKV", 9, 5, 15);
		send_buffer_to_OLED(tx_buf, 0, 0);
		HAL_Delay(2000);

		// DSD ON/OFF
		frame[0] = 0x10; frame[1] = 0xB | (!pcm_dsd << 7);
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// DUAL MONO enabling
		frame[0] = 0x12; frame[1] = 0x2;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		frame[1] = 0x3;
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
			frame[1] = 0x3;
			break;
		case SS_96:
			frame[1] = 0x4;
			break;
		case SS_192:
			frame[1] = 0x5;
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

		// Power ON Sequence
		frame[0] = 0x41; frame[1] = 0x0;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		frame[0] = 0x42; frame[1] = 0x34;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		frame[0] = 0x43; frame[1] = 0xB8;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		frame[0] = 0x48; frame[1] = 0x0D;
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

		// Pop Noise Prevention
		frame[0] = 0xD0; frame[1] = 0x6A;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		frame[0] = 0xD3; frame[1] = 0x10;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		frame[0] = 0xD3; frame[1] = 0x0;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		frame[0] = 0xD0; frame[1] = 0x0;
		HAL_I2C_Master_Transmit(&hi2c1, 0x38, frame, size, I2C_TIMEOUT);
		HAL_I2C_Master_Transmit(&hi2c2, 0x38, frame, size, I2C_TIMEOUT);

		// Analog Power On
		frame[0] = 0x3; frame[1] = 0x1;
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
	}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	uint8_t fir = 1; ak4493eq_fir = fir; dsd1794a_fir = fir; bd34301ekv_fir = fir;

	select_font(&FreeSansOblique9pt7b);
	switch (dacv)
	{
	case AK4493EQ:
		draw_text(tx_buf, AK4493EQ_FIRS_NAMES[fir], 9, 53, 15);
		break;
	case DSD1794A:
		draw_text(tx_buf, DSD1794A_FIRS_NAMES[fir], 9, 53, 15);
		break;
	case BD34301EKV:
		draw_text(tx_buf, BD34301EKV_FIRS_NAMES[fir], 9, 53, 15);
		break;
	}
	send_buffer_to_OLED(tx_buf, 0, 0);
	HAL_Delay(2000);

	while (1)
	{
		if ((HAL_GetTick() - progtime) > 500)
			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))
			{
				select_font(&FreeSansOblique9pt7b);

				switch (dacv)
				{
				case AK4493EQ:
					fir = fir == 5 ? 0 : fir + 1;
					ak4493eq_fir = fir == 5 ? 6 : fir;

					pcm_dsd = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);
					boleroSampleSpeed = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)) | (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) << 1);
					switch (boleroSampleSpeed)
					{
					case SS_48:
						dfs = 0x0;
						break;
					case SS_96:
						dfs = 0x1;
						break;
					case SS_192:
						dfs = 0x2;
						break;
					case SS_384:
						dfs = 0x4;
						break;
					}

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

					draw_text(tx_buf, AK4493EQ_FIRS_NAMES[fir], 9, 53, 15);
					break;
					case DSD1794A:
						fir = !fir;
						dsd1794a_fir = fir;

						frame[0] = 0x19; frame[1] = dsd1794a_fir << 1;
						HAL_I2C_Master_Transmit(&hi2c1, 0x4C, frame, size, I2C_TIMEOUT);
						HAL_I2C_Master_Transmit(&hi2c2, 0x4C, frame, size, I2C_TIMEOUT);

						draw_text(tx_buf, DSD1794A_FIRS_NAMES[fir], 9, 53, 15);
						break;
					case BD34301EKV:
						fir = !fir;
						bd34301ekv_fir = fir;

						boleroSampleSpeed = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)) | (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) << 1);
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

						draw_text(tx_buf, BD34301EKV_FIRS_NAMES[fir], 9, 53, 15);
						break;
				}
				send_buffer_to_OLED(tx_buf, 0, 0);
				HAL_Delay(2000);

				progtime = HAL_GetTick();
			}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x0000020B;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x0000020B;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, SPI1_RESET_Pin|SPI1_CS_Pin|SPI1_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Y1_Pin|Y2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SMUTE_Pin|DAC_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENC_CH_A_Pin ENC_CH_B_Pin */
  GPIO_InitStruct.Pin = ENC_CH_A_Pin|ENC_CH_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ENC_BUT_Pin */
  GPIO_InitStruct.Pin = ENC_BUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(ENC_BUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_RESET_Pin SPI1_CS_Pin SPI1_DC_Pin */
  GPIO_InitStruct.Pin = SPI1_RESET_Pin|SPI1_CS_Pin|SPI1_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DACV0_Pin DACV1_Pin DACV2_Pin DACV3_Pin */
  GPIO_InitStruct.Pin = DACV0_Pin|DACV1_Pin|DACV2_Pin|DACV3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BLOERO_MUX1_Pin BOLERO_MUX0_Pin BOLERO_Y1_Y2_Pin BOLERO_PCM_DSD_Pin
                           BOLERO_DAC_RESET_Pin */
  GPIO_InitStruct.Pin = BLOERO_MUX1_Pin|BOLERO_MUX0_Pin|BOLERO_Y1_Y2_Pin|BOLERO_PCM_DSD_Pin
                          |BOLERO_DAC_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Y1_Pin Y2_Pin SMUTE_Pin DAC_RESET_Pin */
  GPIO_InitStruct.Pin = Y1_Pin|Y2_Pin|SMUTE_Pin|DAC_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 4 */
void Set_Clock(uint8_t clock)
{
	switch (clock)
	{
	case C_44_1:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
		break;
	case C_48:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
		break;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
