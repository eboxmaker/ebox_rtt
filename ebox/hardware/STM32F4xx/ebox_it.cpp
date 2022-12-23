/**
  ******************************************************************************
  * @file    ebox_it.cpp
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

#include "ebox_core.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_tim.h"

#include "ebox_timer_it.h"
#include "ebox.h"
/*
��ʱ���жϼ�飺
    ���ļ��ṩ�˶�ʱ�����ж���ڡ����������ж�ʱ������thisָ�����tim_irq_ids[TIM_IRQ_ID_NUM]��
�û�����ͨ��Tim_It_Index����ѯÿһ����ʱ������thisָ����ڵ�indexֵ����ͨ��tim_irq_handler���͵�
������ڰ���ľ�̬������ڡ�ʵ����Ѱ�Ҷ��󲢶�λ�������������衣
*/


uint32_t master_count[4]  = {0};



static tim_irq_handler irq_handler;
static uint32_t tim_irq_ids[TIM_IRQ_ID_NUM];//�������thisָ��

//index:��ʱ������thisָ�������б��е�����ֵ��ʵ�ֽ��ض��Ķ���󶨵��ض������
//handler����ľ�̬����
//id:�����thisָ��
int tim_irq_init(uint8_t index, tim_irq_handler handler, uint32_t id)
{
    tim_irq_ids[index] = id;//������thisָ�뱣�����б���
    irq_handler =  handler;//��ľ�̬����
    return 0;
}

void tim_irq_callback(uint8_t index)
{
    irq_handler(tim_irq_ids[index]);//Ѱ�ҵ�����Ļص��������
}


extern "C" {


    void TIM1_UP_IRQHandler(void)
    {

        if(LL_TIM_IsActiveFlag_UPDATE(TIM1) == SET)
        {
            master_count[0] += 0x10000;
            tim_irq_callback(TIM1_IT_Update);
            LL_TIM_ClearFlag_UPDATE(TIM1);
        }
        
//        if(TIM_GetITStatus(TIM1, TIM_IT_CC1) == SET)
//        {
//            tim_irq_callback(TIM1_IT_CC1);
//            TIM_ClearITPendingBit(TIM1, TIM_FLAG_CC1);
//        }
//        if(TIM_GetITStatus(TIM1, TIM_IT_CC2) == SET)
//        {
//            tim_irq_callback(TIM1_IT_CC2);
//            TIM_ClearITPendingBit(TIM1, TIM_FLAG_CC2);
//        }
//        if(TIM_GetITStatus(TIM1, TIM_IT_CC3) == SET)
//        {
//            tim_irq_callback(TIM1_IT_CC3);
//            TIM_ClearITPendingBit(TIM1, TIM_FLAG_CC3);
//        }
//        if(TIM_GetITStatus(TIM1, TIM_IT_CC4) == SET)
//        {
//            tim_irq_callback(TIM1_IT_CC4);
//            TIM_ClearITPendingBit(TIM1, TIM_FLAG_CC4);
//        }
    }
    void TIM2_IRQHandler(void)
    {
        if(LL_TIM_IsActiveFlag_UPDATE(TIM2) == SET)
        {
            master_count[1] += 0x10000;
            tim_irq_callback(TIM2_IT_Update);
            LL_TIM_ClearFlag_UPDATE(TIM2);
        }
//        if((TIM2->SR & TIM_IT_CC1) && (TIM2->DIER & TIM_IT_CC1))
//        {
//            tim_irq_callback(TIM2_IT_CC1);
//            TIM2->SR = (uint16_t)~TIM_IT_CC1;
//        }
//        if((TIM2->SR & TIM_IT_CC2) && (TIM2->DIER & TIM_IT_CC2))
//        {
//            tim_irq_callback(TIM2_IT_CC2);
//            TIM2->SR = (uint16_t)~TIM_IT_CC2;
//        }
//        if((TIM2->SR & TIM_IT_CC3) && (TIM2->DIER & TIM_IT_CC3))
//        {
//            tim_irq_callback(TIM2_IT_CC3);
//            TIM2->SR = (uint16_t)~TIM_IT_CC3;
//        }
//        if((TIM2->SR & TIM_IT_CC4) && (TIM2->DIER & TIM_IT_CC4))
//        {
//            tim_irq_callback(TIM2_IT_CC4);
//            TIM2->SR = (uint16_t)~TIM_IT_CC4;
//        }

    }
    void TIM3_IRQHandler(void)
    {
//        if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
//        {
//            master_count[2] += 0x10000;
//            t3_overflow_times++;
//            tim_irq_callback(TIM3_IT_Update);
//            TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
//        }
        if(LL_TIM_IsActiveFlag_UPDATE(TIM3) == SET)
        {
            master_count[2] += 0x10000;
            tim_irq_callback(TIM3_IT_Update);
            LL_TIM_ClearFlag_UPDATE(TIM3);
        }

//        if(TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)
//        {
//            tim_irq_callback(TIM3_IT_CC1);
//            TIM_ClearITPendingBit(TIM2, TIM_FLAG_CC1);
//        }
//        if(TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET)
//        {
//            tim_irq_callback(TIM3_IT_CC2);
//            TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC2);
//        }
//        if(TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET)
//        {
//            tim_irq_callback(TIM3_IT_CC3);
//            TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC3);
//        }
//        if(TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET)
//        {
//            tim_irq_callback(TIM3_IT_CC4);
//            TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC4);
//        }
    }
    void TIM4_IRQHandler(void)
    {
//        if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
//        {
//            master_count[3] += 0x10000;
//            t4_overflow_times++;
//            tim_irq_callback(TIM4_IT_Update);
//            TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
//        }
        if(LL_TIM_IsActiveFlag_UPDATE(TIM4) == SET)
        {
            master_count[3] += 0x10000;
            tim_irq_callback(TIM4_IT_Update);
            LL_TIM_ClearFlag_UPDATE(TIM4);
        }
        
//        if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)
//        {
//            tim_irq_callback(TIM4_IT_CC1);
//            TIM_ClearITPendingBit(TIM4, TIM_FLAG_CC1);
//        }
//        if(TIM_GetITStatus(TIM4, TIM_IT_CC2) == SET)
//        {
//            tim_irq_callback(TIM4_IT_CC2);
//            TIM_ClearITPendingBit(TIM4, TIM_FLAG_CC2);
//        }
//        if(TIM_GetITStatus(TIM4, TIM_IT_CC3) == SET)
//        {
//            tim_irq_callback(TIM4_IT_CC3);
//            TIM_ClearITPendingBit(TIM4, TIM_FLAG_CC3);
//        }
//        if(TIM_GetITStatus(TIM4, TIM_IT_CC4) == SET)
//        {
//            tim_irq_callback(TIM4_IT_CC4);
//            TIM_ClearITPendingBit(TIM4, TIM_FLAG_CC4);
//        }
    }
#if defined(TIM5)
    void TIM5_IRQHandler(void)
    {
//        if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
//        {
//            tim_irq_callback(TIM5_IT_Update);
//            TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
//        }
        if(LL_TIM_IsActiveFlag_UPDATE(TIM5) == SET)
        {
            master_count[4] += 0x10000;
            tim_irq_callback(TIM5_IT_Update);
            LL_TIM_ClearFlag_UPDATE(TIM5);
        }
        
//        if(TIM_GetITStatus(TIM5, TIM_IT_CC1) == SET)
//        {
//            tim_irq_callback(TIM5_IT_CC1);
//            TIM_ClearITPendingBit(TIM5, TIM_FLAG_CC1);
//        }
//        if(TIM_GetITStatus(TIM5, TIM_IT_CC2) == SET)
//        {
//            tim_irq_callback(TIM5_IT_CC2);
//            TIM_ClearITPendingBit(TIM5, TIM_FLAG_CC2);
//        }
//        if(TIM_GetITStatus(TIM5, TIM_IT_CC3) == SET)
//        {
//            tim_irq_callback(TIM5_IT_CC3);
//            TIM_ClearITPendingBit(TIM5, TIM_FLAG_CC3);
//        }
//        if(TIM_GetITStatus(TIM5, TIM_IT_CC4) == SET)
//        {
//            tim_irq_callback(TIM5_IT_CC4);
//            TIM_ClearITPendingBit(TIM5, TIM_FLAG_CC4);
//        }
    }
#endif
#if defined(TIM6)
    void TIM6_IRQHandler(void)
    {
//        if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
//        {
//            tim_irq_callback(TIM6_IT_Update);
//            TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
//        }
        if(LL_TIM_IsActiveFlag_UPDATE(TIM6) == SET)
        {
            master_count[5] += 0x10000;
            tim_irq_callback(TIM6_IT_Update);
            LL_TIM_ClearFlag_UPDATE(TIM6);
        }

    }
#endif
#if defined(TIM7)
    void TIM7_IRQHandler(void)
    {
//        if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
//        {
//            tim_irq_callback(TIM7_IT_Update);
//            TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
//        }
        if(LL_TIM_IsActiveFlag_UPDATE(TIM7) == SET)
        {
            master_count[6] += 0x10000;
            tim_irq_callback(TIM7_IT_Update);
            LL_TIM_ClearFlag_UPDATE(TIM7);
        }

    }
#endif

/**
  * @brief This function handles ADC1, ADC2 and ADC3 global interrupts.
  */
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */

  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&(adc1.hadc));
  /* USER CODE BEGIN ADC_IRQn 1 */

  /* USER CODE END ADC_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&(adc1.hdma));
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}




























}

