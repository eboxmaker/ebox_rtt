/**
  ******************************************************************************
  * @file    bsp_ebox.h
  * @author  cat_li
  * @version V1.0
  * @date    2018/07/31
  * @brief   硬件相关信息声明
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
#ifndef __BSP_EBOX_H
#define __BSP_EBOX_H
#include "ebox.h"
#include "rtt_utils.h"
#include "soft_i2c.h"
#include "soft_spi.h"
#include "ebox_gpio.h"
#include "ebox_uart.h"
#include "ebox_mcu_spi.h"
#include "ebox_iflash.h"
#include "at24x.h"
#include "w25xxx.h"
#include "mmc_sd.h"
#include "at24x.h"


#define	HARDWARE	"ebox_spark，STM32F103C8T6"
extern SoftI2c bsp_i2c;


extern W25xxx w25;
extern SD sd;
extern At24x ee;



#define LED_R PB7
#define LED_G PB8
#define LED_B PB9

#define LED1 PB8
#define LED2 PB9
#define LED3 PB10
#define I2C  i2c2

extern const unsigned char JpegData1[];


extern void print_log(const char *name = "test", const char *date = __DATE__);
#define UART uart1

#define Serial UART

#endif

