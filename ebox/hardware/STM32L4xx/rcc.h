#ifndef __RCC_H
#define __RCC_H


#ifdef __cplusplus
extern "C" {
#endif
#include "stm32l4xx_hal.h"

void rcc_clock_cmd(uint32_t dev, FunctionalState enable);

#ifdef __cplusplus
}
#endif

#endif
