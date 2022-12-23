/**
  ******************************************************************************
  * @file    object.cpp
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


/* Includes ------------------------------------------------------------------*/
#include "ebox.h"


mcuGpio PA0(PA0_ID);
mcuGpio PA1(PA1_ID);
mcuGpio PA2(PA2_ID);
mcuGpio PA3(PA3_ID);
mcuGpio PA4(PA4_ID);
mcuGpio PA5(PA5_ID);
mcuGpio PA6(PA6_ID);
mcuGpio PA7(PA7_ID);
mcuGpio PA8(PA8_ID);
mcuGpio PA9(PA9_ID);
mcuGpio PA10(PA10_ID);
mcuGpio PA11(PA11_ID);
mcuGpio PA12(PA12_ID);
mcuGpio PA13(PA13_ID);
mcuGpio PA14(PA14_ID);
mcuGpio PA15(PA15_ID);

mcuGpio PB0(PB0_ID);
mcuGpio PB1(PB1_ID);
mcuGpio PB2(PB2_ID);
mcuGpio PB3(PB3_ID);
mcuGpio PB4(PB4_ID);
mcuGpio PB5(PB5_ID);
mcuGpio PB6(PB6_ID);
mcuGpio PB7(PB7_ID);
mcuGpio PB8(PB8_ID);
mcuGpio PB9(PB9_ID);
mcuGpio PB10(PB10_ID);
mcuGpio PB11(PB11_ID);
mcuGpio PB12(PB12_ID);
mcuGpio PB13(PB13_ID);
mcuGpio PB14(PB14_ID);
mcuGpio PB15(PB15_ID);

mcuGpio PC13(PC13_ID);
mcuGpio PC14(PC14_ID);
mcuGpio PC15(PC15_ID);


SoftI2c sI2c1(&PB6, &PB7);
SoftI2c sI2c2(&PB10, &PB11);
SoftSpi	sspi1(&PA5, &PA6, &PA7);
SoftSpi	sspi2(&PB13, &PB14, &PB15);
mcuSpi	mcuSpi1(SPI1,&PA5, &PA6, &PA7);
mcuSpi	mcuSpi2(SPI2,&PB13, &PB14, &PB15);


//´®¿Ú
#if USE_UART1
Uart uart1(USART1, &PA9, &PA10);
#endif
#if USE_UART2
Uart uart2(USART2, &PA2, &PA3);
#endif
#if USE_UART3
Uart uart3(USART3, &PB10, &PB11);
#endif

#if (STM32F10X_HD)
#if USE_UART4
Uart uart4(UART4, &PC10, &PC11);
#endif
#if USE_UART5
Uart uart5(UART5, &PC12, &PD2);
#endif
#endif




