/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_hal_conf.h
  * @brief   HAL configuration file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_CONF_CUSTOM_H
#define __HAL_CONF_CUSTOM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the HAL driver
  */

#define HAL_ADC_MODULE_ENABLED   
//#define HAL_CRYP_MODULE_ENABLED   
//#define HAL_CAN_MODULE_ENABLED   
//#define HAL_CAN_LEGACY_MODULE_ENABLED   
//#define HAL_CEC_MODULE_ENABLED   
//#define HAL_CORTEX_MODULE_ENABLED   
//#define HAL_CRC_MODULE_ENABLED   
//#define HAL_DAC_MODULE_ENABLED   
//#define HAL_DMA_MODULE_ENABLED   
//#define HAL_ETH_MODULE_ENABLED   
#define HAL_FLASH_MODULE_ENABLED   
//#define HAL_GPIO_MODULE_ENABLED  
#define HAL_I2C_MODULE_ENABLED   
//#define HAL_I2S_MODULE_ENABLED   
//#define HAL_IRDA_MODULE_ENABLED   
#define HAL_IWDG_MODULE_ENABLED   
//#define HAL_NOR_MODULE_ENABLED   
//#define HAL_NAND_MODULE_ENABLED   
//#define HAL_PCCARD_MODULE_ENABLED   
//#define HAL_PCD_MODULE_ENABLED   
//#define HAL_HCD_MODULE_ENABLED   
//#define HAL_PWR_MODULE_ENABLED   
//#define HAL_RCC_MODULE_ENABLED   
//#define HAL_RTC_MODULE_ENABLED   
//#define HAL_SD_MODULE_ENABLED   
//#define HAL_MMC_MODULE_ENABLED   
//#define HAL_SDRAM_MODULE_ENABLED   
//#define HAL_SMARTCARD_MODULE_ENABLED   
#define HAL_SPI_MODULE_ENABLED   
//#define HAL_SRAM_MODULE_ENABLED   
#define HAL_TIM_MODULE_ENABLED   
#define HAL_UART_MODULE_ENABLED   
//#define HAL_USART_MODULE_ENABLED   
//#define HAL_WWDG_MODULE_ENABLED   

/* ########################## Oscillator Values adaptation ####################*/
/**
  * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSE is used as system clock source, directly or through the PLL).
  */
#if !defined  (HSE_VALUE)
  #define HSE_VALUE    8000000U /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT    100U   /*!< Time out for HSE start up, in ms */
#endif /* HSE_STARTUP_TIMEOUT */

/**
  * @brief Internal High Speed oscillator (HSI) value.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSI is used as system clock source, directly or through the PLL).
  */
#if !defined  (HSI_VALUE)
  #define HSI_VALUE    8000000U /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @brief Internal Low Speed oscillator (LSI) value.
  */
#if !defined  (LSI_VALUE)
 #define LSI_VALUE               40000U    /*!< LSI Typical Value in Hz */
#endif /* LSI_VALUE */                     /*!< Value of the Internal Low Speed oscillator in Hz
                                                The real value may vary depending on the variations
                                                in voltage and temperature. */

/**
  * @brief External Low Speed oscillator (LSE) value.
  *        This value is used by the UART, RTC HAL module to compute the system frequency
  */
#if !defined  (LSE_VALUE)
  #define LSE_VALUE    32768U /*!< Value of the External oscillator in Hz*/
#endif /* LSE_VALUE */

#if !defined  (LSE_STARTUP_TIMEOUT)
  #define LSE_STARTUP_TIMEOUT    5000U   /*!< Time out for LSE start up, in ms */
#endif /* LSE_STARTUP_TIMEOUT */

/* Tip: To avoid modifying this file each time you need to use different HSE,
   ===  you can define the HSE value in your toolchain compiler preprocessor. */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_CONF_H */

