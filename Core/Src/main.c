/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
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
#include <string.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TRUE 1
#define FALSE 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile uint32_t count = 0;
volatile uint32_t speed = 1000;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
TIM_HandleTypeDef htimer2;
void TIMER2_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define zero  0x40 // 0
#define one   0x79 // 1
#define two   0x24 // 2
#define three 0x30 // 3
#define four  0x19 // 4
#define five  0x12 // 5
#define six   0x02 // 6
#define seven 0x78 // 7
#define eight 0x00 // 8
#define nine  0x10 // 9

uint8_t numbers[] = {
    zero, one, two, three, four, five, six, seven, eight, nine
};

void displaynumbers(uint8_t number)
{
    if (number > 9) return;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
    GPIOA->ODR |= (0x7F << 4);
    GPIOA->ODR &= ~((~numbers[number] & 0x7F) << 4);
}



typedef enum
{

  BUTTON_START = 1u,
  BUTTON_PAUSE,
  BUTTON_RESUME,
  BUTTON_RESET
} Button1_State;
int state_of_button1 = 0;
int state_of_button2 = 0;




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
  TIMER2_Init();



  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (state_of_button1 == BUTTON_PAUSE)
	  {
		  HAL_TIM_Base_Stop_IT(&htimer2);
	     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, SET);
	      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, RESET);
	  }
	  if (state_of_button1 == BUTTON_START)
	  {
		  HAL_TIM_Base_Start_IT(&htimer2);
		      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, RESET);
		      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, RESET);

	  }

	  if (state_of_button1 == BUTTON_RESUME)
	  {
		  HAL_TIM_Base_Start_IT(&htimer2);
		     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, RESET);
		      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, SET);

	  }
	  if (state_of_button1 == BUTTON_RESET)
	  {
		  displaynumbers(count);
		  	  count = 0;
		  displaynumbers(count);
      	state_of_button1 = BUTTON_PAUSE;

	  }


  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, A_Pin|B_Pin|C_Pin|D_Pin
                          |E_Pin|F_Pin|G_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : A_Pin B_Pin C_Pin D_Pin
                           E_Pin F_Pin G_Pin PA11
                           PA12 PA15 */
  GPIO_InitStruct.Pin = A_Pin|B_Pin|C_Pin|D_Pin
                          |E_Pin|F_Pin|G_Pin|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : button___Pin button_2_Pin */
  GPIO_InitStruct.Pin = button___Pin|button_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : button_3_Pin */
  GPIO_InitStruct.Pin = button_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(button_3_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static uint32_t last_time1 = 0;
	static uint32_t last_time2 = 0;
	static uint32_t last_time3 = 0;

	uint32_t current_time = HAL_GetTick();

	//  cho button 1
	 if (GPIO_Pin == GPIO_PIN_12)
	    {
	        if (current_time - last_time1 > 200)
	        {
	        	if(state_of_button1 == BUTTON_RESUME)
	        	{
	        		state_of_button1 = BUTTON_START;
	        	}
	        	++state_of_button1;
	            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	            last_time1 = current_time;
	        }
	    }
	 // cho button 2
		 if (GPIO_Pin == GPIO_PIN_13)
		    {
		        if (current_time - last_time2 > 200)
		        {
		             state_of_button1 = BUTTON_RESET;

		            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
		            last_time2 = current_time;
		        }
		    }

		 // cho button 2
			 if (GPIO_Pin == GPIO_PIN_14)
			    {
			        if (current_time - last_time3 > 200)
			        {
			        		speed -= 200;
			        	        if (speed <= 100)
			        	        {
			        	            speed = 1000;
			        	        }

			        	        __HAL_TIM_SET_AUTORELOAD(&htimer2, speed - 1);
			        	        __HAL_TIM_SET_COUNTER(&htimer2, 0);
			        }
			    }
}
// config timer
void TIMER2_Init(void)
{
		htimer2.Instance = TIM2;
		htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
		htimer2.Init.Prescaler = 8000 - 1;
		htimer2.Init.Period = speed;
	if (HAL_TIM_Base_Init(&htimer2) != HAL_OK)
	{
		while(1);
	}
}

// timer callbackkk
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	displaynumbers(count);
    count++;
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
    if (count > 9) count = 0;
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
