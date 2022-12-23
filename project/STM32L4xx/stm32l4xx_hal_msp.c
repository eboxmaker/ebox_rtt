/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32l4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx.h"
DMA_HandleTypeDef hdma_sdmmc1;

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

//void HAL_SD_MspInit(SD_HandleTypeDef* hsd)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//  if(hsd->Instance==SDMMC1)
//  {
//  /* USER CODE BEGIN SDMMC1_MspInit 0 */

//  /* USER CODE END SDMMC1_MspInit 0 */

//  /** Initializes the peripherals clock
//  */
//    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SDMMC1;
//    PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
//    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
//    PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
//    PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
//    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
//    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV8;
//    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
//    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//    {
////      Error_Handler();
//    }

//    /* Peripheral clock enable */
//    __HAL_RCC_SDMMC1_CLK_ENABLE();

//    __HAL_RCC_GPIOC_CLK_ENABLE();
//    __HAL_RCC_GPIOD_CLK_ENABLE();
//    /**SDMMC1 GPIO Configuration
//    PC8     ------> SDMMC1_D0
//    PC9     ------> SDMMC1_D1
//    PC10     ------> SDMMC1_D2
//    PC11     ------> SDMMC1_D3
//    PC12     ------> SDMMC1_CK
//    PD2     ------> SDMMC1_CMD
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
//                          |GPIO_PIN_12;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
//    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = GPIO_PIN_2;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
//    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

//    /* SDMMC1 interrupt Init */
//    HAL_NVIC_SetPriority(SDMMC1_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
//  /* USER CODE BEGIN SDMMC1_MspInit 1 */

//  /* USER CODE END SDMMC1_MspInit 1 */
//  }

//}


/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
