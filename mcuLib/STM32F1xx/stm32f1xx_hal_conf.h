#ifndef __STM32F1xx_HAL_CONF_H
#define __STM32F1xx_HAL_CONF_H

#if defined(__clang__)
    /* STM32F1xx specific HAL configuration options. */
    #if __has_include("hal_conf_custom.h")
        #include "hal_conf_custom.h"
    #else
        #if __has_include("hal_conf_extra.h")
        #include "hal_conf_extra.h"
        #endif
        #include "stm32f1xx_hal_conf_default.h"
    #endif
#else
    #include "hal_conf_extra.h"
    #include "stm32f1xx_hal_conf_default.h"
#endif

#endif /* __STM32F1xx_HAL_CONF_H */
