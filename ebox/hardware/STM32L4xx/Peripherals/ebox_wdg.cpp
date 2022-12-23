/**
  ******************************************************************************
  * @file    wdg.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   2018/10/15 ���ò��������ָ������
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

//�������Ź�ʱ��ԴΪ��LSI�������ڲ�ʱ��ԴƵ��32-60khz
void Iwdg::begin(uint16_t ms)
{
    uint8_t pr  = 0;	//��Ƶ�� 0-7
    uint16_t rlr = 0;	//���ؼĴ���ֵ��12λ��Чλ�����ֵ4095
    // ��������ֵ
    for (pr = 0; pr < 7; pr++)
    {
        rlr = ms * 32 / PR[pr];//��32khz����
        if (rlr <= 0x0fff) break;
    }
    // �������������ֵ����0xfffʱ��ȡ���ֵrlr = 4095, pr = 6;
    // ��ʱ��ʱTout=(256*4095)/32 (ms)
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
