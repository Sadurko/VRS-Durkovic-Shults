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


int main(void)
{
  /*
   * DO NOT WRITE TO THE WHOLE REGISTER!!!
   *
   * Write to bits, that are meant for change.
   */

  /*Enables clock for GPIO port A*/
  *((volatile uint32_t *) (uint32_t)(0x40021000 + 0x00000014U)) |= (uint32_t)(1 << 17);

  /*GPIOA pin 3 and 4 setup*/
  // *GPIOA pin 3 - switch
  // *GPIOA pin 4 - LED
  /*GPIO MODER register*/
  //Set mode for pin 4
  *((volatile uint32_t *)((uint32_t)0x48000000)) &= ~(uint32_t)(0x3 << 8);
  *((volatile uint32_t *)((uint32_t)0x48000000)) |= (uint32_t)(1 << 8);
  //Set mode for pin 3
  *((volatile uint32_t *)((uint32_t)0x48000000)) &= ~(uint32_t)(0x3 << 6);

  /*GPIO OTYPER register*/
  *((volatile uint32_t *)((uint32_t)(0x48000000 + 0x04U))) &= ~(1 << 4);

  /*GPIO OSPEEDR register*/
  //Set Low speed for GPIOB pin 4
  *((volatile uint32_t *)((uint32_t)(0x48000000 + 0x08U))) &= ~(0x3 << 8);

  /*GPIO PUPDR register, reset*/
  //Set pull up for GPIOB pin 3 (input)
  *((volatile uint32_t *)((uint32_t)(0x48000000 + 0x0CU))) |= (1 << 6);
  //Set no pull for GPIOB pin 4
  *((volatile uint32_t *)((uint32_t)(0x48000000 + 0x0CU))) &= ~(0x3 << 8);

  uint8_t samples = 5;
  uint8_t pin_state = 0;
  while (1)
  {
	  //for(uint16_t i = 0; i < 0xFF00; i++){}
	  if(!(*((volatile uint32_t *)((uint32_t)(0x48000000 + 0x10U))) & (1 << 3)))
	  {
		  pin_state = 1;
	  }else{
		  pin_state = 0;
	  }
	  enum EDGE_TYPE edge = edgeDetect(pin_state,samples);
	  switch(edge)
	  {
	  case NONE: LED_OFF; break;
	  case RISE: LED_ON; break;
	  case FALL: LED_OFF; break;
	  }

  }

}

enum EDGE_TYPE samplesEdgeDetect(uint8_t pin_state_list, uint8_t samples)
{
	// kontroluju sa predchadzajuce stavy vstupneho pinu
	if(samples == 1)
		{
			return ((pin_state_list & 1) == 1) ? RISE : FALL;
	}else{
		if((pin_state_list & 1) == ((pin_state_list >> 1) & 1))
		{
			return samplesEdgeDetect(pin_state_list >> 1, samples - 1);
		}else{
			return NONE;
		}
	}
}

uint8_t pin_state_list = 0;


enum EDGE_TYPE edgeDetect(uint8_t pin_state, uint8_t samples)
{
	// ukladanie stavu na vstupnom pine do premennej
	pin_state_list = pin_state_list << 1;
	pin_state_list |= pin_state;

	return samplesEdgeDetect(pin_state_list, samples);
}


/* USER CODE BEGIN 4 */

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
