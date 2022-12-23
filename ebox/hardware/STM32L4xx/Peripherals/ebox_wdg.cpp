/**
  ******************************************************************************
  * @file    wdg.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   2018/10/15 改用查表法，减少指数运算
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
#include "ebox_wdg.h"
#include "stm32l4xx_hal.h"
#include "stm32_hal.h"



const uint16_t PR[7] = {4, 8, 16, 32, 64, 128, 256};
IWDG_HandleTypeDef hiwdg;

//独立看门狗时钟源为：LSI，低速内部时钟源频率32-60khz
void Iwdg::begin(uint16_t ms)
{
    uint8_t pr  = 0;	//分频数 0-7
    uint16_t rlr = 0;	//重载寄存器值，12位有效位，最大值4095
    // 计算重载值
    for (pr = 0; pr < 7; pr++)
    {
        rlr = ms * 32 / PR[pr];//按32khz计算
        if (rlr <= 0x0fff) break;
    }
    // 当计算出的重载值大于0xfff时，取最大值rlr = 4095, pr = 6;
    // 此时延时Tout=(256*4095)/32 (ms)
    if ( rlr > 0x0fff || pr >= 7 )
    {
        pr = 6;
        rlr = 0xfff;
    }
    
    
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = pr;
  hiwdg.Init.Window = rlr;
  hiwdg.Init.Reload = rlr;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }    
}
void Iwdg::feed()
{
    HAL_IWDG_Refresh(&hiwdg);
}
void Iwdg::end()
{
}
