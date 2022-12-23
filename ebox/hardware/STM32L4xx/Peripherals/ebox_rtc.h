/**
  ******************************************************************************
  * @file    rtc.h
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  *          2018/11/3  LQM
  *          �޸Ľӿڣ���f0����һֱ������f1��rtc���������ṩ �꣬�£��գ�ʱ���֣���
  *          �������ṩһ������
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

#ifndef __RTC_H
#define __RTC_H
#include "ebox_core.h"
#include "FunctionPointer.h"
#include "time.h"
/**
  *1.�ṩһ��32λ��ѭ������,ÿ���1.
  *2.һ���ж�Դ�������ж��¼�
  *3.֧��һ������ʱ��
  *4.֧��һ������
  */
typedef struct
{
    uint32_t Format12_24;	//
    uint8_t Hours;			// 0-12 or 0-23
    uint8_t Minutes;		// 0-59
    uint8_t Seconds;		// 0-59
} Time_T;

typedef struct
{
    Time_T time;
    uint32_t Day;
    uint32_t WeekDaySel;
} Alarm_T;

typedef struct
{
    uint8_t WeekDay;	// ��һ������ 1-7
    uint8_t Day;			// 1-31
    uint8_t Month;		// 1-12
    uint8_t Year;			// 0-99
} Date_T;

typedef struct
{
    uint8_t Year;
    uint8_t Month;
    uint8_t Day;
    uint8_t Hours;
    uint8_t Minutes;
    uint8_t Seconds;
    uint8_t WeekDay;
} date_time_t;

typedef enum
{
    clock_lsi = 0,
    clock_lse
} ClockS;

enum Rtc_IrqType
{
    Sec_Irq = 0,
    Alr_Irq = 1,
    Ow_Irq = 2
};

#define eRtc_NoMask                         0//LL_RTC_ALMA_MASK_NONE          // ������,date,hour,minutes,seconds������ƥ��
#define eRtc_MaskDateWeekDay                0//LL_RTC_ALMA_MASK_DATEWEEKDAY   // ��ƥ��������
#define eRtc_MaskHours                      0//LL_RTC_ALMA_MASK_HOURS         // ��ƥ��Сʱ
#define eRtc_MaskMinutes                    0//LL_RTC_ALMA_MASK_MINUTES       // ��ƥ�����
#define eRtc_MaskSeconds                    0//LL_RTC_ALMA_MASK_SECONDS       // ��ƥ����
#define eRtc_MaskAll                        0//(RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1) //��������λ,��ʱ����ÿ�봥��һ��

typedef void (*rtc_irq_handler)(uint32_t id, Rtc_IrqType type);

class Rtc
{
public:
    /**
    	* @brief  ��ʼ��RTC,Ĭ��LSE,���LSE����ʧ��,���Զ�תΪLSI
    	* @param  ClockS clock ʱ��Դ��clock_lse,clock_lsi
    	* @retval E_TIMEOUT ʱ������ʧ��,E_OK ʱ�����óɹ�,��������ʱ�ӣ�E_NG ʱ�����óɹ�����Ҫ����ʱ��
    	*/
    int begin(ClockS clock = clock_lse);


    /** ��������
     *  @param time ��ʱʱ��
     *  @param mask ����������Ӧλ(��ϸ��define)��Ĭ�ϲ�ƥ�����ڡ�f1ϵ���иò�����Ч��f0����Ч��
     */
    void setAlarm(Time_T time, uint32_t mask = eRtc_MaskDateWeekDay);
    void setInterrupt(FunctionalState state, Rtc_IrqType type = Alr_Irq);

    // �������ڣ�ʱ��
    void setDate(Date_T date);
    void setTime(Time_T time);
    // ��ȡ���ڣ�ʱ��
    void getDateTime(date_time_t *datetime);
    void getTime(Time_T *time);
    void getDate(Date_T *date);


    /** �󶨻ص�����
     *  @param fptr ����ָ�룬ָ��һ���޷���ֵ���Ӻ���, �� 0 ָ��պ���
     *  @param type �ж����ͣ�Ĭ�� alarm�жϡ�f1ϵ���иò�����Ч��f0����Ч
     */
    void attach(void (*fptr)(void), Rtc_IrqType type = Alr_Irq);

    /** �󶨳�Ա�ص�����
     *  @param tptr ����ָ�룬һ��Ϊ����
     *  @param mptr ��Աָ�룬��ʽһ��Ϊ &����::��Ա��
     *  @param type �ж����ͣ�Ĭ�� alarm�жϡ�f1ϵ���иò�����Ч��f0����Ч
     */
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void), Rtc_IrqType type = Sec_Irq)
    {
        if ((mptr != NULL) && (tptr != NULL))
        {
            _irq[type].attach(tptr, mptr);
        }
    }
private:
    int         _config(ClockS clock);
    void        _setTimeFlag(uint16_t configFlag);
    uint8_t     _getTimeFlag(void);
    void        _nvic(FunctionalState state);
    void        _setTimeCounter(uint32_t count);
    uint32_t    _getTimeCounter(void);
    uint32_t    _getAlarmCounter(void);
    void        _updateDate(uint32_t dayElapsed);
    uint8_t     _isLeapYear(uint16_t nYear);
protected:
    FunctionPointer _irq[3];
    static void _irq_handler(uint32_t id, Rtc_IrqType irq_type);
};

#endif