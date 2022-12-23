#ifndef _stm32_hal_h
#define _stm32_hal_h

#if defined (STM32F1xx)
#include "stm32f1xx_hal.h"
#endif

#if defined (STM32F4xx)
#include "stm32f4xx_hal.h"
#endif

#if defined (STM32L412xx) || defined (STM32L422xx) ||defined (STM32L431xx) || defined (STM32L432xx) || defined (STM32L433xx) || defined (STM32L442xx) || defined (STM32L443xx) || defined (STM32L451xx) || defined (STM32L452xx) || defined (STM32L462xx)
#include "stm32l4xx_hal.h"
#endif

#endif
