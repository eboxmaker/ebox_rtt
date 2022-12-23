/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24                  the first version
 */

#include <rthw.h>
#include <rtthread.h>

#include "stm32l451xx.h"
#include "stm32l4xx_hal.h"

#include "ebox_core.h"

extern void user_uart_init();
extern void user_uart_out(const char *str);
extern void cpu_usage_init(void);

//#define _SCB_BASE       (0xE000E010UL)
//#define _SYSTICK_CTRL   (*(rt_uint32_t *)(_SCB_BASE + 0x0))
//#define _SYSTICK_LOAD   (*(rt_uint32_t *)(_SCB_BASE + 0x4))
//#define _SYSTICK_VAL    (*(rt_uint32_t *)(_SCB_BASE + 0x8))
//#define _SYSTICK_CALIB  (*(rt_uint32_t *)(_SCB_BASE + 0xC))
//#define _SYSTICK_PRI    (*(rt_uint8_t  *)(0xE000ED23UL))

// Updates the variable SystemCoreClock and must be called
// whenever the core clock is changed during program execution.
extern void SystemCoreClockUpdate(void);

// Holds the system core clock, which is the system clock
// frequency supplied to the SysTick timer and the processor
// core clock.
extern uint32_t SystemCoreClock;

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
}

static uint32_t _SysTick_Config(rt_uint32_t ticks)
{ 
    if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
    {
        return (1UL);                                                   /* Reload value impossible */
    }

    SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
    NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
    SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
               SysTick_CTRL_TICKINT_Msk   |
               SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
    return (0UL);  
    }

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 2048 means 2048*4 bytes
 */
//#define RT_HEAP_SIZE 1024*3

//static uint32_t rt_heap[RT_HEAP_SIZE];
//RT_WEAK void *rt_heap_begin_get(void)
//{
//    return rt_heap;
//}

//RT_WEAK void *rt_heap_end_get(void)
//{
//    return rt_heap + RT_HEAP_SIZE;
//}
#endif

/**
 * This function will initial your board.
 */

void rt_hw_board_init(void)
{
    HAL_Init();
    SystemClock_Config();     //Configure the system clock
    SystemCoreClockUpdate();
    /* TODO: OS Tick Configuration */
    _SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    //rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
    rt_system_heap_init((void *)MCU_HEAP_BEGIN, (void *)MCU_HEAP_END);

#endif
    ebox_init();
    user_uart_init();
#ifdef RT_USING_IDLE_HOOK
    cpu_usage_init();
#endif
}

/* TODO: OS Tick increase */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();
    HAL_IncTick();
    /* leave interrupt */
    rt_interrupt_leave();
}

/* TODO: uart init and console output */
#ifdef RT_USING_CONSOLE


static int uart_init(void)
{
    user_uart_init();
//#error "Please implement the code according to your chip"
    return 0;
}
INIT_BOARD_EXPORT(uart_init);

/* For console print */
void rt_hw_console_output(const char *str)
{
    user_uart_out(str);
//#error "Please implement the code according to your chip"
}

#endif

