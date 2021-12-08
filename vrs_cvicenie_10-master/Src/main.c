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
#include "string.h"
#include "stdlib.h"

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
  uint8_t rx_data[10] = "";
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
void setDutyCyckle(uint8_t D);

void TimerCaptureCompare_Callback(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
float voltage = 0;
uint32_t uwMeasuredDutyCycle = 0;
static uint8_t rezim = 0;
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  // vyberanie funkcii
  USART2_RegisterCallback(processDmaData);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int deley_count = 0,turning_on = 1;

  while (1)
  {
    /* USER CODE END WHILE */
#if POLLING
	//Polling for new data, no interrupts
	USART2_CheckDmaReception();
	LL_mDelay(10);
#else
	USART2_CheckDmaReception();
	USART2_RegisterCallback(processDmaData);
	if(rezim == 0)
	{
		if(turning_on)
		{
			if(deley_count > 98)
			{
				turning_on = 0;
			}
			deley_count++;
			setDutyCycle(deley_count);
		}else
		{
			if(deley_count <2)
			{
				turning_on = 1;
			}
			deley_count--;
			setDutyCycle(deley_count);
		}
	}
	LL_mDelay(10);
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
  LL_Init1msTick(8000000); // 8 MHz
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


void setDutyCycle(uint8_t D)
{
	uint32_t P; // dlzka pulzu
	uint32_t T = LL_TIM_GetAutoReload(TIM2) + 1; // dlzka periody

	P = (D*T)/100;
	LL_TIM_OC_SetCompareCH1(TIM2,P);
}

void processDmaData(const uint8_t* data, uint16_t len)
{
	char manual[6] = "manual";
	char automat[4] = "auto";
	char pwm[5] = "PWM  ";
	char aux[6] = "";
	uint8_t xx;

	if(*(data) == '$')
	{
		for(uint8_t i = 0; (i <= sizeof(data)); i++)
		{
			if(*(data+i) == '$')
			{
				listen = 1;
				//len = i - 1; // ulozi na akej pozicii bol najdeny druhy $
				break;
			}
			if(i == 7) // najdlhsi mozny signal ma 7 znakov aj s ukoncovacim $
			{
				listen = 0;
			}
		}

		if(listen) // pocuva iba ak prijalo prikaz zacinajuci a konciaci $
		{
			int lenData;
			for(lenData = 0; data+1 && *(data + lenData + 1) != '$' && lenData < 6; lenData++)
			{
				aux[lenData] = *(data  +lenData+1);
			}

			if(lenData == 6) // porovnava s "manual"
			{
				int x = compareTwoStrings(aux,manual);
				if(x == 0) {
					rezim = 1; // nastavi rezim
				}
			}
			else if(lenData == 4) // porovnava s "auto"
			{
				if(compareTwoStrings(aux, automat) == 0) {
					rezim = 0;	// nastavi rezim
				}
			}
			else if((lenData == 5) && (rezim == 1))
			{
				if((aux[3] >= '0' && aux[3] <= '9') && (aux[4] >= '0' && aux[4] <= '9')) // skontroluje ci ide o cisla
				{
					xx = 10*(aux[3] - '0') + (aux[4] - '0'); // do premennej ulozi cislo z prikazu
					aux[3] = ' '; // na miesto cisel vlozi medzery pre porovnanie ci sedi prikaz
					aux[4] = ' ';
					float x = compareTwoStrings(aux, pwm);
					if(x == 0) {
						setDutyCycle(xx);
					}
				}
			}
		}
	}
}

int compareTwoStrings(char str1[],char str2[])
{
	for(int i = 0; i < sizeof(str1);i++)
	{
		if(i > sizeof(str2)) return 1;
		if(str1[i] != str2[i]) return 1;
	}
	return 0;
}
void TimerCaptureCompare_Callback(void)
{
	uint32_t CNT = LL_TIM_GetCounter(TIM2);
	uint32_t ARR = LL_TIM_GetAutoReload(TIM2);

	if (LL_TIM_OC_GetCompareCH1(TIM2) > ARR)
	{
		// osetrenie pretecenia pocitadla
		CNT = CNT + ARR + 1;
	}
	uwMeasuredDutyCycle = (CNT * 100)/(ARR + 1);
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
