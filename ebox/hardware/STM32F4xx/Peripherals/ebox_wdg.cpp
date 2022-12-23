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
#include "stm32f4xx_hal.h"

const uint16_t PR[7] = {4, 8, 16, 32, 64, 128, 256};
IWDG_HandleTypeDef IWDG_Handler; //

void Iwdg::begin(uint16_t ms)
{
    uint8_t pr  = 0;	//��Ƶ�� 0-7
    uint16_t rlr = 0;	//���ؼĴ���ֵ��12λ��Чλ�����ֵ4095
    // ��������ֵ
    for (pr = 0; pr < 7; pr++)
    {
        rlr = ms * 40 / PR[pr];
        if (rlr <= 0x0fff) break;
    }
    // �������������ֵ����0xfffʱ��ȡ���ֵrlr = 4095, pr = 6;
    // ��ʱ��ʱTout=(256*4095)/40 (ms)
    if ( rlr > 0x0fff || pr >= 7 )
    {
        pr = 6;
        rlr = 0xfff;
    }
    IWDG_Handler.Instance=IWDG; //
    IWDG_Handler.Init.Prescaler=pr; //
    IWDG_Handler.Init.Reload=rlr; //

    HAL_IWDG_Init(&IWDG_Handler);
}
void Iwdg::feed()
{
    HAL_IWDG_Refresh(&IWDG_Handler);
}
