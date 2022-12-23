#include "rcc.h"
#include "stm32f1xx_hal.h"
#include "stm32f103xb.h"
typedef struct
{
    uint32_t dev;
    uint32_t rcc;

} DevToRcc_t;

//外设->时钟源查询表
static const DevToRcc_t dev_to_rcc_table[] =
{
    {GPIOA_BASE, RCC_APB2ENR_IOPAEN},
    {GPIOB_BASE, RCC_APB2ENR_IOPBEN},
    {GPIOC_BASE, RCC_APB2ENR_IOPCEN},
    {GPIOD_BASE, RCC_APB2ENR_IOPDEN},
    {GPIOE_BASE, RCC_APB2ENR_IOPEEN},
#if defined(GPIOF)    
    {GPIOF_BASE, RCC_APB2ENR_IOPFEN},
#endif
#if defined(GPIOG)
    {GPIOG_BASE, RCC_APB2ENR_IOPGEN},
#endif
    //    {GPIOH_BASE,RCC_APB2Periph_GPIOH},
    //    {GPIOI_BASE,RCC_APB2Periph_GPIOI},

    {ADC1_BASE, RCC_APB2ENR_ADC1EN},
    {ADC2_BASE, RCC_APB2ENR_ADC2EN},

    {TIM1_BASE, RCC_APB2ENR_TIM1EN},
    {TIM2_BASE, RCC_APB1ENR_TIM2EN},
    {TIM3_BASE, RCC_APB1ENR_TIM3EN},
//    {TIM4_BASE, RCC_APB1Periph_TIM4},
//    {TIM5_BASE, RCC_APB1Periph_TIM5},
//    {TIM6_BASE, RCC_APB1Periph_TIM6},
//    {TIM7_BASE, RCC_APB1Periph_TIM7},
//    {TIM8_BASE, RCC_APB2Periph_TIM8},
//    {TIM9_BASE, RCC_APB2Periph_TIM9},
//    {TIM10_BASE, RCC_APB2Periph_TIM10},
//    {TIM11_BASE, RCC_APB2Periph_TIM11},

    {SPI1_BASE, RCC_APB2ENR_SPI1EN},
    {SPI2_BASE, RCC_APB1ENR_SPI2EN},
//    {SPI3_BASE, RCC_APB1Periph_SPI3},
//    //    {SPI4_BASE,RCC_APB2Periph_SPI4},
//    //    {SPI5_BASE,RCC_APB2Periph_SPI5},
    {I2C1_BASE, RCC_APB1ENR_I2C1EN},
    {I2C2_BASE, RCC_APB1ENR_I2C2EN},
//    //    {I2C3_BASE,RCC_APB1Periph_I2C3},

    {USART1_BASE, RCC_APB2ENR_USART1EN},
    {USART2_BASE, RCC_APB1ENR_USART2EN},
    {USART3_BASE, RCC_APB1ENR_USART3EN},
//    {UART4_BASE, RCC_APB1Periph_UART4},
//    {UART5_BASE, RCC_APB1Periph_UART5}
    
    {DMA1_BASE, RCC_AHBENR_DMA1EN},
    {DMA1_Channel1_BASE, RCC_AHBENR_DMA1EN},
    {DMA1_Channel2_BASE, RCC_AHBENR_DMA1EN},
    {DMA1_Channel3_BASE, RCC_AHBENR_DMA1EN},
    {DMA1_Channel4_BASE, RCC_AHBENR_DMA1EN},
    {DMA1_Channel5_BASE, RCC_AHBENR_DMA1EN},
    {DMA1_Channel6_BASE, RCC_AHBENR_DMA1EN},
    {DMA1_Channel7_BASE, RCC_AHBENR_DMA1EN},
    
    {ADC1_BASE, RCC_AHBENR_DMA1EN},
  
};

#if 1
/**
 *@name     void rcc_clock_cmd(uint32_t dev, FunctionalState state)
 *@brief    根据设备地址开启或关闭时钟
 *@param    dev  :  设备地址，如(uint32_t)TIM1,(uint32_t)USART1,(uint32_t)SPI1等等
            state:  ENABLE(开启时钟) 或者 DISABLE(关闭时钟)
 *@retval   NONE
*/
void rcc_clock_cmd(uint32_t dev, FunctionalState state)
{
    uint32_t rcc;
    for(int i = 0; i < sizeof(dev_to_rcc_table) / sizeof(DevToRcc_t); i++)
    {
        if(dev_to_rcc_table[i].dev == dev)
            rcc  = dev_to_rcc_table[i].rcc;

    }
    if(dev >= AHBPERIPH_BASE)
    {
        if (state != DISABLE){RCC->AHBENR |= rcc;}else{RCC->AHBENR &= ~rcc;} 
    }
    else if(dev >= APB2PERIPH_BASE)
    {
        if (state != DISABLE){RCC->APB2ENR |= rcc;}else{RCC->APB2ENR &= ~rcc;}
    }
    else if(dev >= APB1PERIPH_BASE)
    {
        if (state != DISABLE){RCC->APB1ENR |= rcc;}else{RCC->APB1ENR &= ~rcc;}
    }

}
#else
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
        case DMA1_Channel1_BASE:
        case DMA1_Channel2_BASE:
        case DMA1_Channel3_BASE:
        case DMA1_Channel4_BASE:
        case DMA1_Channel5_BASE:
        case DMA1_Channel6_BASE:
        case DMA1_Channel7_BASE:
            if(state==ENABLE){__HAL_RCC_DMA1_CLK_ENABLE();}else{__HAL_RCC_DMA1_CLK_DISABLE();}break;
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
#endif
