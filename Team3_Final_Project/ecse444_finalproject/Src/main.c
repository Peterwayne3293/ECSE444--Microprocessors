/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32l4xx_hal.h"

#include "stdlib.h"
#include "arm_math.h"
#include "stm32l475e_iot01_qspi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac1;

QSPI_HandleTypeDef hqspi;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
int playing_mode = Unmixed;
extern int sineWaveIdx;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DAC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_QUADSPI_Init(void);
/* USER CODE BEGIN PFP */
int showMenuOptions(void);
void showGenerateWavesMenu(void);
void showMixWavesMenu(void);
void showUnmixWavesMenu(void);
void showPlayMenu(PlayingMode);
void store_sine(float frequency, int duration, int sampling_freq, uint32_t qspiStartAddress);
void mix_waves(uint32_t input_addr_1, uint32_t input_addr_2, uint32_t output_addr_1, uint32_t output_addr_2, int size, float a11, float a12, float a21, float a22);
void unmix_waves(int size, uint32_t input_addr_1, uint32_t input_addr_2, uint32_t output_addr_1, uint32_t output_addr_2);

float32_t vec_norm_mat(arm_matrix_instance_f32 vec);
float32_t vec_norm_arr(float32_t* vec, int size);

int fputc(int ch, FILE *f) {
  while (HAL_OK != HAL_UART_Transmit(&huart1, (uint8_t *) &ch, 1, 30000));
  return ch;
}
int fgetc(FILE *f) {
  uint8_t ch = 0;
  while (HAL_OK != HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 30000));
  return ch;
}
void uart_receive(char output[], int size, int reply) {
	uint8_t ch = 0;
	int idx = 0;
	
  while (idx < size) {
		HAL_UART_Receive(&huart1, &ch, 1, 30000);
		
		// If no character receive, try again - infinite timeout
		if (!ch)
			continue;
		
		if(ch == '\n' || ch == '\r')
			break;
		
		// Transmit back 
		if (reply)
			HAL_UART_Transmit(&huart1, &ch, 1, 30000);
		
		output[idx++] = ch;
	}
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t tempBuffer1[TEMP_BUFFER_SIZE];
uint8_t tempBuffer2[TEMP_BUFFER_SIZE];
uint8_t tempBufferOut1[TEMP_BUFFER_SIZE];
uint8_t tempBufferOut2[TEMP_BUFFER_SIZE];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_QUADSPI_Init();
  /* USER CODE BEGIN 2 */
	BSP_QSPI_Init();
	
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	// Show banner
	printf("##########################################\r\n"
				 "### ECSE 444 - Final Project - Group 3 ###\r\n"
				 "##########################################\r\n");
	
	// Erase previously saved waves
	BSP_QSPI_Erase_Block(WAVE_1_ADDRESS);
	BSP_QSPI_Erase_Block(WAVE_2_ADDRESS);
	BSP_QSPI_Erase_Block(WAVE_MIXED_1_ADDRESS);
	BSP_QSPI_Erase_Block(WAVE_MIXED_2_ADDRESS);
	BSP_QSPI_Erase_Block(WAVE_UNMIXED_1_ADDRESS);
	BSP_QSPI_Erase_Block(WAVE_UNMIXED_2_ADDRESS);
				
  while (1)
  {
		switch(showMenuOptions()) {
			case 0:
				// Timeout - simply reshow menu 
				break;
			case GenerateWaves:
				// Erase previously generated waves
				BSP_QSPI_Erase_Block(WAVE_1_ADDRESS);
				BSP_QSPI_Erase_Block(WAVE_2_ADDRESS);
			
				showGenerateWavesMenu();
				break;
			case MixWaves:
				// Erase previously mixed waves
				BSP_QSPI_Erase_Block(WAVE_MIXED_1_ADDRESS);
				BSP_QSPI_Erase_Block(WAVE_MIXED_2_ADDRESS);
			
				showMixWavesMenu();
				break;
			case UnmixWaves:
				// Erase previously unmixed waves
				BSP_QSPI_Erase_Block(WAVE_UNMIXED_1_ADDRESS);
				BSP_QSPI_Erase_Block(WAVE_UNMIXED_2_ADDRESS);
			
				showUnmixWavesMenu();
				break;
			case PlayGenerated:
				showPlayMenu(Generated);
				break;
			case PlayMixed:
				showPlayMenu(Mixed);
				break;
			case PlayUnmixed:
				showPlayMenu(Unmixed);
				break;
			case PlayDiff:
				showPlayMenu(Diff);
				break;
			default:
				printf("Invalid option.\n");
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

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */
  /**DAC Initialization 
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }
  /**DAC channel OUT1 config 
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /**DAC channel OUT2 config 
  */
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 255;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 1;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 5000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
int showMenuOptions() {
	printf("\r\n=== Main menu ===\r\n\r\n"
				 "  1) Generate signals\r\n"
				 "  2) Mix signals\r\n"
				 "  3) Unmix signals\r\n"
				 "  4) Play generated signal\r\n"
				 "  5) Play mixed signal\r\n"
				 "  6) Play unmixed signal\r\n"
				 "  7) Play generated-unmixed difference signal\r\n\r\n"
				 "Enter option: ");
	
	char option;
	uart_receive(&option, 1, 0);
	
	return option - '0';
}

void showGenerateWavesMenu() {
	char okay = 0;
	char freq1arr[7] = {0}, freq2arr[7] = {0};
	float freq1, freq2;
	
	while(okay != 'Y' && okay != 'y') {
		// Get frequencies
		printf("\r\n=== Wave generation ===\r\n\r\n"
					 "Wave 1 frequency: ");
		uart_receive(freq1arr, 6, 1);
		
		printf("\r\nWave 2 frequency: ");
		uart_receive(freq2arr, 6, 1);
		
		// Convert them to float
		freq1 = atof(freq1arr);
		freq2 = atof(freq2arr);
		
		// Make sure they are ok
		printf("\r\n\r\nFrequency 1: %.2f\r\n", freq1);
		printf("Frequency 2: %.2f\r\n\r\n", freq2);
		
		printf("(Y/N) Is this okay? ");
		uart_receive(&okay, 1, 0);
	}
	
	// We got a confirmation, so let's generate them
	printf("\r\nGenerating sine waves ...\r\n");
	store_sine(freq1, DURATION_IN_SEC, SAMPLING_FREQUENCY, WAVE_1_ADDRESS);
	store_sine(freq2, DURATION_IN_SEC, SAMPLING_FREQUENCY, WAVE_2_ADDRESS); 
}

void showMixWavesMenu() {
	char okay = 0;
	char a11arr[6] = {0}, a12arr[6] = {0}, a21arr[6] = {0}, a22arr[6] = {0};
	float a11, a12, a21, a22;
	
	while(okay != 'Y' && okay != 'y') {
		// Get frequencies
		printf("\r\n=== Wave mixing ===\r\n\r\n"
					 "a11: ");
		uart_receive(a11arr, 5, 1);
		
		printf("\r\na12: ");
		uart_receive(a12arr, 5, 1);
		
		printf("\r\na21: ");
		uart_receive(a21arr, 5, 1);
		
		printf("\r\na22: ");
		uart_receive(a22arr, 5, 1);
		
		// Convert them to float
		a11 = atof(a11arr);
		a12 = atof(a12arr);
		a21 = atof(a21arr);
		a22 = atof(a22arr);
		
		// Make sure they are ok
		printf("\r\n\r\na11: %.2f\r\n", a11);
		printf("a12: %.2f\r\n", a12);
		printf("a21: %.2f\r\n", a21);
		printf("a22: %.2f\r\n\r\n", a22);
		
		printf("(Y/N) Is this okay? ");
		uart_receive(&okay, 1, 0);
	}
	
	// We got a confirmation, so let's mix them
	printf("\r\nMixing sine waves ...\r\n");
	mix_waves(WAVE_1_ADDRESS, WAVE_2_ADDRESS, WAVE_MIXED_1_ADDRESS, WAVE_MIXED_2_ADDRESS, NUMBER_OF_SAMPLES, a11, a12, a21, a22);
}

void showUnmixWavesMenu(){
	printf("\r\n=== Wave unmixing ===\r\n\r\n"
						 "Unmixing waves...\r\n");

	unmix_waves(NUMBER_OF_SAMPLES, WAVE_MIXED_1_ADDRESS, WAVE_MIXED_2_ADDRESS, WAVE_UNMIXED_1_ADDRESS, WAVE_UNMIXED_2_ADDRESS);
}

void showPlayMenu(PlayingMode mode) {
	playing_mode = mode;
	switch(mode){
		case Generated:
			printf("\r\n=== Generated waves playing ===\r\n\r\n");
			printf("Left channel: Wave 1 (s1)\r\n"
				 "Right channel: Wave 2 (s2)\r\n");
			break;
		case Mixed:
			printf("\r\n=== Mixed waves playing ===\r\n\r\n");
			printf("Left channel: Wave 1 (x1)\r\n"
				 "Right channel: Wave 2 (x2)\r\n");
			break;
		case Unmixed:
			printf("\r\n=== Unmixed waves playing ===\r\n\r\n");
			printf("Left channel: Wave 1 (s1')\r\n"
				 "Right channel: Wave 2 (s2')\r\n");
			break;
		case Diff:
			printf("\r\n=== Unmixed waves playing ===\r\n\r\n");
			printf("Left channel: Wave 1 (s1 - s1')\r\n"
				 "Right channel: Wave 2 (s2 - s2')\r\n");
			break;
	}
	
	printf("\r\nPress any character to return to main menu ...");

	HAL_TIM_Base_Start_IT(&htim2);
	
	char c = 0;
	while(c == 0) {
		uart_receive(&c, 1, 0); 
	}
	
	HAL_TIM_Base_Stop_IT(&htim2);
	
	sineWaveIdx = 0;
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_8B_R, 0);
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_8B_R, 0);
}

void store_sine(float frequency, int duration, int sampling_freq, uint32_t qspiStartAddress)
{
	int t;
	float invSamplingFreq = 1.0f / sampling_freq;
	
	int bufferIdx = 0;
	for(t = 0; t < duration * sampling_freq; t++) {
		float32_t val = arm_sin_f32(2.0f * PI * frequency * t * invSamplingFreq); 
		uint8_t data = (val + 1.0f) * 127.5f; // convert to 8 bits
		tempBuffer1[bufferIdx++] = data;
		
		if (bufferIdx == TEMP_BUFFER_SIZE) {
			bufferIdx = 0;
			BSP_QSPI_Write(tempBuffer1, qspiStartAddress + t + 1 - TEMP_BUFFER_SIZE, TEMP_BUFFER_SIZE);
		}
	}
}

void mix_waves(uint32_t input_addr_1, uint32_t input_addr_2, uint32_t output_addr_1, uint32_t output_addr_2, int size, float a11, float a12, float a21, float a22)
{
	// Make sure vectors stay normalized in new basis to avoid clipping
	float a1sum = a11 + a12;
	float a2sum = a21 + a22; 
	
	a11 = a11 / a1sum;
	a12 = a12 / a1sum;
	a21 = a21 / a2sum;
	a22 = a22 / a2sum;
	
	int i;
	for(i = 0; i < size; i += TEMP_BUFFER_SIZE)
	{
		// Get unmixed waves
		int bytesToRead = (i + TEMP_BUFFER_SIZE > size) ? (i + TEMP_BUFFER_SIZE - size) : TEMP_BUFFER_SIZE;
		
		BSP_QSPI_Read(tempBuffer1, input_addr_1 + i, bytesToRead);
		BSP_QSPI_Read(tempBuffer2, input_addr_2 + i, bytesToRead);
		
		int j;
		for(j = 0; j < bytesToRead; j++) {
			tempBufferOut1[j] = a11 * tempBuffer1[j] + a12 * tempBuffer2[j];
			tempBufferOut2[j] = a21 * tempBuffer1[j] + a22 * tempBuffer2[j];
		}
		
		BSP_QSPI_Write(tempBufferOut1, output_addr_1 + i, bytesToRead);
		BSP_QSPI_Write(tempBufferOut2, output_addr_2 + i, bytesToRead);
	}
}

float32_t vec_norm_mat(arm_matrix_instance_f32 vec) {
	int size = (vec.numCols > vec.numRows) ? vec.numCols : vec.numRows;
	
	float32_t* data = vec.pData;
	int i;
	float32_t sum = 0.0;
	for(i = 0; i < size; i++, data++) {
		sum += (*data) * (*data);
	}
	
	float32_t res;
	arm_sqrt_f32(sum, &res);
	
	return res;
}

float32_t vec_norm_arr(float32_t* vec, int size) {
	int i;
	float32_t sum = 0.0;
	for(i = 0; i < size; i++, vec++) {
		sum += (*vec) * (*vec);
	}
	
	float32_t res;
	arm_sqrt_f32(sum, &res);
	
	return res;
}

void unmix_waves(int size, uint32_t input_addr_1, uint32_t input_addr_2, uint32_t output_addr_1, uint32_t output_addr_2){
	// 1) Get mean for x_1 and x_2
	float32_t mean1 = 0;
	float32_t mean2 = 0;
	int i;
	for(i = 0; i < size; i += TEMP_BUFFER_SIZE){
		int bytesToRead = (i + TEMP_BUFFER_SIZE > size) ? (i + TEMP_BUFFER_SIZE - size) : TEMP_BUFFER_SIZE;
		
		BSP_QSPI_Read(tempBuffer1, input_addr_1 + i, bytesToRead);
		BSP_QSPI_Read(tempBuffer2, input_addr_2 + i, bytesToRead);
		
		int j;
		for(j = 0; j < bytesToRead; j++){
			mean1 += tempBuffer1[j];
			mean2 += tempBuffer2[j];
		}
	}
	
	mean1 /= size;
	mean2 /= size;
	
	// 2) Compute variance-covariance matrix
	float32_t cv_mat_values[] = {0, 0, 0, 0};
	for(i = 0; i < size; i++) {
		uint8_t val1;
		uint8_t val2;
		
		// Get values of both mixed signals
		BSP_QSPI_Read(&val1, input_addr_1 + i, 1);
		BSP_QSPI_Read(&val2, input_addr_2 + i, 1);
		
		cv_mat_values[0] += (val1 - mean1) * (val1 - mean1);
		cv_mat_values[1] += (val1 - mean1) * (val2 - mean2);
		cv_mat_values[2] += (val1 - mean1) * (val2 - mean2);
		cv_mat_values[3] += (val2 - mean2) * (val2 - mean2);
	}
	cv_mat_values[0] /= (size - 1);
	cv_mat_values[1] /= (size - 1);
	cv_mat_values[2] /= (size - 1);
	cv_mat_values[3] /= (size - 1);
	
	float32_t trace = cv_mat_values[0] + cv_mat_values[3];
	float32_t det = cv_mat_values[0] * cv_mat_values[3] - cv_mat_values[1] * cv_mat_values[2];
		
	// 3) Compute eigenvalues and eigenvectors
	float32_t sqrtDet;
	arm_sqrt_f32(trace * trace - 4 * det, &sqrtDet);
		
	float32_t eigval1 = (trace + sqrtDet) / 2.0f;
	float32_t eigval2 = (trace - sqrtDet) / 2.0f;
	
	float32_t eigvec1[] = {eigval1 - cv_mat_values[3], cv_mat_values[2]};
	float32_t eigvec2[] = {eigval2 - cv_mat_values[3], cv_mat_values[2]};
	
	float32_t eigvec1_norm = vec_norm_arr(eigvec1, 2);
	float32_t eigvec2_norm = vec_norm_arr(eigvec2, 2);
	
	// Normalize eigenvectors
	eigvec1[0] = eigvec1[0] / eigvec1_norm; eigvec1[1] = eigvec1[1] / eigvec1_norm;
	eigvec2[0] = eigvec2[0] / eigvec2_norm; eigvec2[1] = eigvec2[1] / eigvec2_norm;
	
	float32_t mat_E_values[] = {eigvec2[0], eigvec2[1], eigvec1[0], eigvec1[1]};
	arm_matrix_instance_f32 mat_E = {2, 2, mat_E_values};
	
	// 4) Compute whitening and de-whitening matrices
	float32_t mat_sqrt_eig_values[] = {0, 0, 0, 0};
	arm_sqrt_f32(eigval1, &mat_sqrt_eig_values[3]);
	arm_sqrt_f32(eigval2, &mat_sqrt_eig_values[0]);
	arm_matrix_instance_f32 mat_sqrt_eig = {2, 2, mat_sqrt_eig_values};
	
	float32_t mat_whitening_values[] = {0, 0, 0, 0};
	float32_t mat_dewhitening_values[] = {0, 0, 0, 0};
	float32_t trans_mat_E_values[] = {0, 0, 0, 0};
	float32_t inv_mat_sqrt_eig_values[] = {0, 0, 0, 0};
	arm_matrix_instance_f32 mat_whitening = {2, 2, mat_whitening_values};
	arm_matrix_instance_f32 mat_dewhitening = {2, 2, mat_dewhitening_values};
	arm_matrix_instance_f32 trans_mat_E = {2, 2, trans_mat_E_values};
	arm_matrix_instance_f32 inv_mat_sqrt_eig = {2, 2, inv_mat_sqrt_eig_values};
	arm_mat_trans_f32(&mat_E, &trans_mat_E);
	arm_mat_inverse_f32(&mat_sqrt_eig, &inv_mat_sqrt_eig);
	
	arm_mat_mult_f32(&inv_mat_sqrt_eig, &trans_mat_E, &mat_whitening);
	arm_mat_mult_f32(&mat_E, &mat_sqrt_eig, &mat_dewhitening);
	
	// 5) Initialize "random" weight vector
	float32_t vec_weight_values[] = {0.5, 0.5};
	float32_t vec_weight_norm = vec_norm_arr(vec_weight_values, 2);
	vec_weight_values[0] = vec_weight_values[0] / vec_weight_norm; vec_weight_values[1] = vec_weight_values[1] / vec_weight_norm;
	arm_matrix_instance_f32 vec_weight = {2, 1, vec_weight_values};
	float32_t vec_weight_values_old[] = {0.0, 0.0};
	arm_matrix_instance_f32 vec_weight_old = {2, 1, vec_weight_values_old};
	
	int iteration = 0;
	int max_iterations = 100;
	float epsilon = 0.0001f;
	
	float32_t vec_weight_tmp_values[] = {0, 0};
	arm_matrix_instance_f32 vec_weight_tmp = {2, 1, vec_weight_tmp_values};
	
	// 6) Run through ICA until convergence
	while(iteration++ < max_iterations) {
		// Test for convergence
		arm_mat_sub_f32(&vec_weight, &vec_weight_old, &vec_weight_tmp);
		if(vec_norm_mat(vec_weight_tmp) < epsilon)
			break;
		
		arm_mat_add_f32(&vec_weight, &vec_weight_old, &vec_weight_tmp);
		if(vec_norm_mat(vec_weight_tmp) < epsilon)
			break;
		
		// Update weight
		vec_weight_values_old[0] = vec_weight_values[0]; vec_weight_values_old[1] = vec_weight_values[1]; 
		
		// Whiten input matrix and compute new weights
		// Note: matrix has to be whiten for every iteration because it "cannot" be
		// stored in ram since it's too big and nor in flash since it uses 32bit values 
		vec_weight_values[0] = 0.0;
		vec_weight_values[1] = 0.0;
		for(i = 0; i < size; i++) {
			uint8_t in_val1, in_val2;
			
			BSP_QSPI_Read(&in_val1, input_addr_1 + i, 1);
			BSP_QSPI_Read(&in_val2, input_addr_2 + i, 1);
			
			float32_t whitenval1, whitenval2;
			
			whitenval1 = mat_whitening_values[0] * (in_val1 - mean1) + mat_whitening_values[1] * (in_val2 - mean2);
			whitenval2 = mat_whitening_values[2] * (in_val1 - mean1) + mat_whitening_values[3] * (in_val2 - mean2);
			
			float32_t tmp_w1 = whitenval1 * vec_weight_values_old[0] + whitenval2 * vec_weight_values_old[1];
			float32_t tmp_w2 = whitenval1 * vec_weight_values_old[0] + whitenval2 * vec_weight_values_old[1];
			
			vec_weight_values[0] += whitenval1 * tmp_w1 * tmp_w1 * tmp_w1;
			vec_weight_values[1] += whitenval2 * tmp_w2 * tmp_w2 * tmp_w2;
		}
		
		vec_weight_values[0] /= size;
		vec_weight_values[1] /= size;
		
		vec_weight_values[0] = vec_weight_values[0] - 3 * vec_weight_values_old[0];
		vec_weight_values[1] = vec_weight_values[1] - 3 * vec_weight_values_old[1];
		
		// Normalize weight
		vec_weight_norm = vec_norm_arr(vec_weight_values, 2);
		vec_weight_values[0] = vec_weight_values[0] / vec_weight_norm; vec_weight_values[1] = vec_weight_values[1] / vec_weight_norm;
	}
	
	// 7) Construct basis set
	float32_t mat_basis_set_values[] = {vec_weight_values[0], -vec_weight_values[1], vec_weight_values[1], vec_weight_values[0]};
	float32_t mat_basis_set_trans_values[] = {0, 0, 0, 0};
	arm_matrix_instance_f32 mat_basis_set = {2, 2, mat_basis_set_values};
	arm_matrix_instance_f32 mat_basis_set_trans = {2, 2, mat_basis_set_trans_values};
	arm_mat_trans_f32(&mat_basis_set, &mat_basis_set_trans);
	
	// 8) Construct ICA filter
	float32_t mat_ica_values[] = {0, 0, 0, 0};
	arm_matrix_instance_f32 mat_ica = {2, 2, mat_ica_values};
	arm_mat_mult_f32(&mat_basis_set_trans, &mat_whitening, &mat_ica);

	// 9) Compute BSS signals statistics
	float32_t min1 = 1000.0, min2 = 1000.0, max1 = -1000.0, max2 = -1000.0;
	for(i = 0; i < size; i++) {
		uint8_t in_val1, in_val2;
			
		BSP_QSPI_Read(&in_val1, input_addr_1 + i, 1);
		BSP_QSPI_Read(&in_val2, input_addr_2 + i, 1);
		
		float32_t outval1, outval2;
		outval1 = mat_ica_values[2] * in_val1 + mat_ica_values[3] * in_val2;
		outval2 = mat_ica_values[0] * in_val1 + mat_ica_values[1] * in_val2;
		
		if (outval1 < min1)
			min1 = outval1;
		if (outval1 > max1)
			max1 = outval1;
		if (outval2 < min2)
			min2 = outval2;
		if (outval2 > max2)
			max2 = outval2;
	}
	
	// 10) Approximate original signal
	for(i = 0; i < size; i++) {
		uint8_t in_val1, in_val2;
			
		BSP_QSPI_Read(&in_val1, input_addr_1 + i, 1);
		BSP_QSPI_Read(&in_val2, input_addr_2 + i, 1);
		
		float32_t outval1, outval2;
		outval1 = mat_ica_values[2] * in_val1 + mat_ica_values[3] * in_val2;
		outval2 = mat_ica_values[0] * in_val1 + mat_ica_values[1] * in_val2;
		
		// Normalize to unsigned 8 bits
		uint8_t outval1_u8 = 255.0f * (outval1 + min1) / (max1 - min1);
		uint8_t outval2_u8 = 255.0f * (outval2 + min2) / (max2 - min2);
		
		BSP_QSPI_Write(&outval1_u8, output_addr_1 + i, 1);
		BSP_QSPI_Write(&outval2_u8, output_addr_2 + i, 1);
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
