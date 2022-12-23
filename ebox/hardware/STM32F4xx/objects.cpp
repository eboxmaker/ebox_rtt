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

mcuGpio PC0(PC0_ID);
mcuGpio PC1(PC1_ID);
mcuGpio PC2(PC2_ID);
mcuGpio PC3(PC3_ID);
mcuGpio PC4(PC4_ID);
mcuGpio PC5(PC5_ID);
mcuGpio PC6(PC6_ID);
mcuGpio PC7(PC7_ID);
mcuGpio PC8(PC8_ID);
mcuGpio PC9(PC9_ID);
mcuGpio PC10(PC10_ID);
mcuGpio PC11(PC11_ID);
mcuGpio PC12(PC12_ID);
mcuGpio PC13(PC13_ID);
mcuGpio PC14(PC14_ID);
mcuGpio PC15(PC15_ID);

mcuGpio PD0(PD0_ID);
mcuGpio PD1(PD1_ID);
mcuGpio PD2(PD2_ID);
mcuGpio PD3(PD3_ID);
mcuGpio PD4(PD4_ID);
mcuGpio PD5(PD5_ID);
mcuGpio PD6(PD6_ID);
mcuGpio PD7(PD7_ID);
mcuGpio PD8(PD8_ID);
mcuGpio PD9(PD9_ID);
mcuGpio PD10(PD10_ID);
mcuGpio PD11(PD11_ID);
mcuGpio PD12(PD12_ID);
mcuGpio PD13(PD13_ID);
mcuGpio PD14(PD14_ID);
mcuGpio PD15(PD15_ID);

mcuGpio PE0(PE0_ID);
mcuGpio PE1(PE1_ID);
mcuGpio PE2(PE2_ID);
mcuGpio PE3(PE3_ID);
mcuGpio PE4(PE4_ID);
mcuGpio PE5(PE5_ID);
mcuGpio PE6(PE6_ID);
mcuGpio PE7(PE7_ID);
mcuGpio PE8(PE8_ID);
mcuGpio PE9(PE9_ID);
mcuGpio PE10(PE10_ID);
mcuGpio PE11(PE11_ID);
mcuGpio PE12(PE12_ID);
mcuGpio PE13(PE13_ID);
mcuGpio PE14(PE14_ID);
mcuGpio PE15(PE15_ID);

mcuGpio PF9(PF9_ID);


SoftI2c sI2c1(&PB6, &PB7);
SoftI2c sI2c2(&PB10, &PB11);
SoftSpi	sspi2(&PB13, &PB14, &PB15);


SoftSpi	sspi1(&PA1, &PA6, &PA7);
mcuSpi	spi1(SPI1, &PA1, &PA6, &PA7);
mcuSpi	mcuSpi2(SPI2,&PB13, &PB14, &PB15);

//´®¿Ú
Uart uart1(USART1, &PA9, &PA10);
Uart uart2(USART2, &PA2, &PA3);
Uart uart3(USART3, &PC4, &PC5);

Adc adc1(ADC1);
