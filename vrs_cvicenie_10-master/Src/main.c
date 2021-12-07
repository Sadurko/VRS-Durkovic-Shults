/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "dma.h"
#include "usart.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"
#include "ctype.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */
  uint8_t tx_data[] = "Data to send over UART DMA!\n\r";
  uint8_t rx_data[10] = "          ";
  uint8_t count = 0;
  uint8_t countUpper = 0;
  uint8_t countLower = 0;
  uint8_t listen = 0; // moze byt potrebne zmenit
  char mem[35];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void receive_dma_data(const uint8_t* data, uint16_t len);
void processDmaData(const uint8_t* data, uint16_t len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
float voltage = 0;
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
  

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  // vyberanie funkcii
  USART2_RegisterCallback(processDmaData);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int deley_count = 0;
  while (1)
  {
    /* USER CODE END WHILE */
#if POLLING
	//Polling for new data, no interrupts
	USART2_CheckDmaReception();
	LL_mDelay(10);
#else
	if(deley_count == 30)
	{
		USART2_PutBuffer(rx_data, sizeof(rx_data));
		deley_count = 0;
	}
	LL_mDelay(100);
	deley_count++;
#endif
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
}

/* USER CODE BEGIN 4 */
void receive_dma_data(const uint8_t* data, uint16_t len)
{
    for(uint8_t i = 0; i < len; i++)
    {
    	if(*(data+i) == '1')
		{
			count++;
		}
    }
}

static uint8_t count = 0;
static uint8_t rezim = 0;

void processDmaData(const uint8_t* data, uint16_t len)
{
	char manual[6] = "manual";
	char automat[4] = "auto";
	char pwm[5] = "pwm  ";
	char aux[6];
	int xx;

	if(*(data) == '$')
	{
		for(uint8_t i = 0; (i <= 7); i++)
		{
			if(*(data+i) == '$')
			{
				listen = 1;
				len = i - 1; // ulozi na akej pozicii bol najdeny druhy $
				break;
			}
			if(i == 7) // najdlhsi mozny signal ma 7 znakov aj s ukoncovacim $
			{
				listen = 0;
			}
		}

		if(listen) // pocuva iba ak prijalo prikaz zacinajuci a konciaci $
		{
			for(uint8_t i = 1; i < len; i++)
			{
				aux[i-1] = *(data+i);
			}

			if(len == 6) // porovnava s "manual"
			{
				if(strcmp(aux, manual) == 0) {
					rezim = 0; // nastavi rezim
				}
			}
			else if(len == 4) // porovnava s "auto"
			{
				if(strcmp(aux, automat) == 0) {
					rezim = 1;	// nastavi rezim
				}
			}
			else if(len == 5)
			{
				if(isdigit(aux[3]) && isdifit(aux[4])) // skontroluje ci ide o cisla
				{
					x1 = 10*atoi(aux[3]) + atoi(aux[4]); // do premennej ulozi cislo z prikazu
					aux[3] = ' '; // na miesto cisel vlozi medzery pre porovnanie ci sedi prikaz
					aux[4] = ' ';
					if(strcmp(aux, pwm) == 0) {
						rezim = 2;
					}
				}
			}
		}
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
