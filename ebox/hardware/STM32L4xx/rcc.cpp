#include "rcc.h"
#include "stm32l4xx.h"

void rcc_clock_cmd(uint32_t dev, FunctionalState enable)
{
    switch(dev)
    {
        case GPIOA_BASE:
            if(enable == ENABLE )__HAL_RCC_GPIOA_CLK_ENABLE(); else __HAL_RCC_GPIOA_CLK_DISABLE();break;
        case GPIOB_BASE:
            if(enable == ENABLE )__HAL_RCC_GPIOB_CLK_ENABLE(); else __HAL_RCC_GPIOB_CLK_DISABLE();break;
        case GPIOC_BASE:
            if(enable == ENABLE )__HAL_RCC_GPIOC_CLK_ENABLE(); else __HAL_RCC_GPIOC_CLK_DISABLE();break;
        case GPIOD_BASE:
            if(enable == ENABLE )__HAL_RCC_GPIOD_CLK_ENABLE(); else __HAL_RCC_GPIOD_CLK_DISABLE();break;
        case GPIOE_BASE:
            if(enable == ENABLE )__HAL_RCC_GPIOE_CLK_ENABLE(); else __HAL_RCC_GPIOE_CLK_DISABLE();break;
        
        case USART1_BASE:
            if(enable == ENABLE )__HAL_RCC_USART1_CLK_ENABLE(); else __HAL_RCC_USART1_CLK_DISABLE();break;
        case USART2_BASE:
            if(enable == ENABLE )__HAL_RCC_USART2_CLK_ENABLE(); else __HAL_RCC_USART2_CLK_DISABLE();break;
        case USART3_BASE:
            if(enable == ENABLE )__HAL_RCC_USART3_CLK_ENABLE(); else __HAL_RCC_USART3_CLK_DISABLE();break;
        case UART4_BASE:
            if(enable == ENABLE )__HAL_RCC_UART4_CLK_ENABLE(); else __HAL_RCC_UART4_CLK_DISABLE();break;
        case LPUART1_BASE:
            if(enable == ENABLE )__HAL_RCC_LPUART1_CLK_ENABLE(); else __HAL_RCC_LPUART1_CLK_DISABLE();break;
        
        case DMA1_BASE:
        case DMA1_Channel1_BASE:
        case DMA1_Channel2_BASE:
        case DMA1_Channel3_BASE:
        case DMA1_Channel4_BASE:
        case DMA1_Channel5_BASE:
        case DMA1_Channel6_BASE:
        case DMA1_Channel7_BASE:
            if(enable == ENABLE )__HAL_RCC_DMA1_CLK_ENABLE(); else __HAL_RCC_DMA1_CLK_DISABLE();break;
        case DMA2_BASE:
        case DMA2_Channel1_BASE:
        case DMA2_Channel2_BASE:
        case DMA2_Channel3_BASE:
        case DMA2_Channel4_BASE:
        case DMA2_Channel5_BASE:
        case DMA2_Channel6_BASE:
        case DMA2_Channel7_BASE:
            if(enable == ENABLE )__HAL_RCC_DMA2_CLK_ENABLE(); else __HAL_RCC_DMA2_CLK_DISABLE();break;
    }
}


