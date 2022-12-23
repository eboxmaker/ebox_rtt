#include "rcc.h"

/**
 *@name     void rcc_clock_cmd(uint32_t dev, FunctionalState state)
 *@brief    根据设备地址开启或关闭时钟
 *@param    dev  :  设备地址，如(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1等等
            state:  ENABLE(开启时钟) 或者 DISABLE(关闭时钟)
 *@retval   NONE
*/
void rcc_clock_cmd(uint32_t dev, FunctionalState state)
{
    switch(dev)
    {
        case GPIOA_BASE:
            if(state==ENABLE){__HAL_RCC_GPIOA_CLK_ENABLE();}else{__HAL_RCC_GPIOA_CLK_DISABLE();}break;
        #if defined(GPIOB)
        case GPIOB_BASE:
            if(state==ENABLE){__HAL_RCC_GPIOB_CLK_ENABLE();}else{__HAL_RCC_GPIOB_CLK_DISABLE();}break;
        #endif
        #if defined(GPIOC)
        case GPIOC_BASE:
            if(state==ENABLE){__HAL_RCC_GPIOC_CLK_ENABLE();}else{__HAL_RCC_GPIOC_CLK_DISABLE();}break;
        #endif
        #if defined(GPIOD)
        case GPIOD_BASE:
            if(state==ENABLE){__HAL_RCC_GPIOD_CLK_ENABLE();}else{__HAL_RCC_GPIOD_CLK_DISABLE();}break;
        #endif
        #if defined(GPIOE)
        case GPIOE_BASE:
            if(state==ENABLE){__HAL_RCC_GPIOE_CLK_ENABLE();}else{__HAL_RCC_GPIOE_CLK_DISABLE();}break;
        #endif
        #if defined(GPIOF)
        case GPIOF_BASE:
            if(state==ENABLE){__HAL_RCC_GPIOF_CLK_ENABLE();}else{__HAL_RCC_GPIOF_CLK_DISABLE();}break;
        #endif
        #if defined(GPIOG)
        case GPIOG_BASE:
            if(state==ENABLE){__HAL_RCC_GPIOG_CLK_ENABLE();}else{__HAL_RCC_GPIOG_CLK_DISABLE();}break;
        #endif
        #if defined(GPIOH)
        case GPIOH_BASE:
            if(state==ENABLE){__HAL_RCC_GPIOH_CLK_ENABLE();}else{__HAL_RCC_GPIOH_CLK_DISABLE();}break;
        #endif
        case USART1_BASE:
            if(state==ENABLE){__HAL_RCC_USART1_CLK_ENABLE();}else{__HAL_RCC_USART1_CLK_DISABLE();}break;
        case USART2_BASE:
            if(state==ENABLE){__HAL_RCC_USART2_CLK_ENABLE();}else{__HAL_RCC_USART2_CLK_DISABLE();}break;
        #if defined(USART3)
        case USART3_BASE:
            if(state==ENABLE){__HAL_RCC_USART3_CLK_ENABLE();}else{__HAL_RCC_USART3_CLK_DISABLE();}break;
        #endif
        #if defined(USART4)
        case USART4_BASE:
            if(state==ENABLE){__HAL_RCC_USART4_CLK_ENABLE();}else{__HAL_RCC_USART4_CLK_DISABLE();}break;
        #endif
        #if defined(USART5_BASE)
        case USART5_BASE:
            if(state==ENABLE){__HAL_RCC_USART5_CLK_ENABLE();}else{__HAL_RCC_USART5_CLK_DISABLE();}break;
        #endif
        #if defined(DMA1)
        case DMA1_BASE:
        case DMA1_Stream0_BASE:
        case DMA1_Stream1_BASE:
        case DMA1_Stream2_BASE:
        case DMA1_Stream3_BASE:
        case DMA1_Stream4_BASE:
        case DMA1_Stream5_BASE:
        case DMA1_Stream6_BASE:
        case DMA1_Stream7_BASE:
            if(state==ENABLE){__HAL_RCC_DMA1_CLK_ENABLE();}else{__HAL_RCC_DMA1_CLK_DISABLE();}break;
        #endif
        #if defined(DMA2)
        case DMA2_BASE:
        case DMA2_Stream0_BASE:
        case DMA2_Stream1_BASE:
        case DMA2_Stream2_BASE:
        case DMA2_Stream3_BASE:
        case DMA2_Stream4_BASE:
        case DMA2_Stream5_BASE:
        case DMA2_Stream6_BASE:
        case DMA2_Stream7_BASE:
            if(state==ENABLE){__HAL_RCC_DMA2_CLK_ENABLE();}else{__HAL_RCC_DMA2_CLK_DISABLE();}break;
        #endif
        case TIM1_BASE:
            if(state==ENABLE){__HAL_RCC_TIM1_CLK_ENABLE();}else{__HAL_RCC_TIM1_CLK_DISABLE();}break;
        case TIM2_BASE:
            if(state==ENABLE){__HAL_RCC_TIM2_CLK_ENABLE();}else{__HAL_RCC_TIM2_CLK_DISABLE();}break;
        case TIM3_BASE:
            if(state==ENABLE){__HAL_RCC_TIM3_CLK_ENABLE();}else{__HAL_RCC_TIM3_CLK_DISABLE();}break;
        #if defined(TIM4_BASE)
        case TIM4_BASE:
            if(state==ENABLE){__HAL_RCC_TIM4_CLK_ENABLE();}else{__HAL_RCC_TIM4_CLK_DISABLE();}break;
        #endif
        #if defined(TIM5_BASE)
        case TIM5_BASE:
            if(state==ENABLE){__HAL_RCC_TIM5_CLK_ENABLE();}else{__HAL_RCC_TIM5_CLK_DISABLE();}break;
        #endif
        #if defined(TIM6_BASE)
        case TIM6_BASE:
            if(state==ENABLE){__HAL_RCC_TIM6_CLK_ENABLE();}else{__HAL_RCC_TIM6_CLK_DISABLE();}break;
        #endif
        #if defined(TIM7_BASE)
        case TIM7_BASE:
            if(state==ENABLE){__HAL_RCC_TIM7_CLK_ENABLE();}else{__HAL_RCC_TIM7_CLK_DISABLE();}break;
        #endif
        #if defined(TIM8_BASE)
        case TIM8_BASE:
            if(state==ENABLE){__HAL_RCC_TIM8_CLK_ENABLE();}else{__HAL_RCC_TIM8_CLK_DISABLE();}break;
        #endif

        case SPI1_BASE:
            if(state==ENABLE){__HAL_RCC_SPI1_CLK_ENABLE();}else{__HAL_RCC_SPI1_CLK_DISABLE();}break;
        case SPI2_BASE:
            if(state==ENABLE){__HAL_RCC_SPI2_CLK_ENABLE();}else{__HAL_RCC_SPI2_CLK_DISABLE();}break;
        case ADC1_BASE:
            if(state==ENABLE){__HAL_RCC_ADC1_CLK_ENABLE();}else{__HAL_RCC_ADC1_CLK_DISABLE();}break;
        case ADC2_BASE:
            if(state==ENABLE){__HAL_RCC_ADC2_CLK_ENABLE();}else{__HAL_RCC_ADC2_CLK_DISABLE();}break;

        case I2C1_BASE:
            if(state==ENABLE){__HAL_RCC_I2C1_CLK_ENABLE();}else{__HAL_RCC_I2C1_CLK_DISABLE();}break;
        case I2C2_BASE:
            if(state==ENABLE){__HAL_RCC_I2C2_CLK_ENABLE();}else{__HAL_RCC_I2C2_CLK_DISABLE();}break;

        default:
            break;
    }
}
