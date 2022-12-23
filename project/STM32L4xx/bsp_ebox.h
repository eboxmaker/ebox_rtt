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
#include "bsp_rtc.h"
#include "rtt_utils.h"
#include "4g.h"
#include "sdio.h"
#include "ADS1120.h"
#include "ripple.h"
#include "interface/storage/filesystem/FileSystem.h"
#include "interface/storage/filesystem/FAT/FATFileSystem.h"
#include "mmc_sd.h"
#include "TinyGPSPlus.h"
#include "w25xxx.h"

#define	HARDWARE	"ebox_spark STM32F103C8T6"

#define LED PB4

extern Iwdg dog;

extern FATFileSystem fs;
extern SDIO sd;
extern SD  ssd;
extern UsrGM5 gm5;
extern Ads1120App adc;
extern TinyGPSPlus gps;
extern Adc adc1;
extern W25xxx w25;


void bat_analog_init();

extern void print_log(const char *name = "test", const char *date = __DATE__);
#define UART uart2

#define Serial UART

#endif

