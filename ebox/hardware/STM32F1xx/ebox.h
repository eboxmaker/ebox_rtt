/**
  ******************************************************************************
  * @file    ebox.h
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EBOX_H
#define __EBOX_H


#include "ebox_core.h"


#if __cplusplus
extern "C" {
#endif

    void ebox_init(void);
    size_t ebox_printf(const char *fmt, ...);

#if __cplusplus
} // extern "C"
#endif


#ifdef __cplusplus
#include "ebox_gpio.h"
//#include "ebox_adc.h"
//#include "ebox_exti.h"
//#include "ebox_timer.h"
//#include "ebox_pwm.h"
//#include "ebox_pulse.h"
//#include "ebox_in_capture.h"
//#include "ebox_encoder.h"
#include "ebox_uart.h"
//#include "ebox_rtc.h"
#include "ebox_iflash.h"
//#include "ebox_dac.h"
//#include "ebox_wdg.h"
//#include "ebox_can.h"
//#include "ebox_step_motor.h"
//#include "ebox_3steper.h"

#include "soft_i2c.h"
#include "soft_spi.h"
#include "ebox_mcu_i2c.h"
#include "ebox_mcu_spi.h"


extern mcuGpio PA0;
extern mcuGpio PA1;
extern mcuGpio PA2;
extern mcuGpio PA3;
extern mcuGpio PA4;
extern mcuGpio PA5;
extern mcuGpio PA6;
extern mcuGpio PA7;
extern mcuGpio PA8;
extern mcuGpio PA9;
extern mcuGpio PA10;
extern mcuGpio PA11;
extern mcuGpio PA12;
extern mcuGpio PA13;
extern mcuGpio PA14;
extern mcuGpio PA15;

extern mcuGpio PB0;
extern mcuGpio PB1;
extern mcuGpio PB2;
extern mcuGpio PB3;
extern mcuGpio PB4;
extern mcuGpio PB5;
extern mcuGpio PB5;
extern mcuGpio PB6;
extern mcuGpio PB7;
extern mcuGpio PB8;
extern mcuGpio PB9;
extern mcuGpio PB10;
extern mcuGpio PB11;
extern mcuGpio PB12;
extern mcuGpio PB13;
extern mcuGpio PB14;
extern mcuGpio PB15;


extern mcuGpio PC13;
extern mcuGpio PC14;
extern mcuGpio PC15;


extern SoftI2c sI2c1;
extern SoftI2c sI2c2;
extern SoftSpi	sspi1;
extern SoftSpi	sspi2;
extern mcuSpi	mcuSpi2;
extern mcuSpi	mcuSpi1;

//创建所有引脚对象和宏定义其指针
#if USE_UART1
    extern Uart uart1;
#endif
#if USE_UART2
    extern Uart uart2;
#endif
#if USE_UART3
    extern Uart uart3;
#endif

#else
#include "bsp_ebox.h"
#endif

#endif

