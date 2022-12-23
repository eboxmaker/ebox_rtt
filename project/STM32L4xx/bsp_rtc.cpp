#include "bsp_rtc.h"
#include "bsp_ebox.h"
#include "log.h"

RTC_HandleTypeDef hrtc;

//RTC_DateTypeDef rtc_date;
//RTC_TimeTypeDef rtc_time;
DateTime gdt;
bool rtc_get_dt(DateTime *dt);
void rtc_set_dt(DateTime &dt);
void MX_RTC_Init(void);


void rtc_init()
{
    gdt.parse(__DATE__,__TIME__); 
    gdt.print(UART);
    MX_RTC_Init();
}
void rtc_loop()
{
    static uint32_t last = millis();
    if(millis() - last > 999)
    {
        rtc_get_dt(&gdt);
    }
}
void stop_iwdg()
{
    uint32_t flag_IWDG;  

    flag_IWDG=READ_BIT(FLASH->OPTR, FLASH_OPTR_IWDG_STOP);//????,??????flash????
    if(flag_IWDG!=0)
    {
       UART.printf("enabled IWDG,then try to close\r\n");
      
      HAL_FLASH_Unlock();
      HAL_FLASH_OB_Unlock();//??FLASH

      CLEAR_BIT(FLASH->OPTR, FLASH_OPTR_IWDG_STOP);//????????:??????????
      /* Set OPTSTRT Bit */
      SET_BIT(FLASH->CR, FLASH_CR_OPTSTRT);//??3?????,??????FLASH->OPTR
      /* Wait for last operation to be completed */
      FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);
       /* If the option byte program operation is completed, disable the OPTSTRT Bit */
      CLEAR_BIT(FLASH->CR, FLASH_CR_OPTSTRT);
      HAL_FLASH_OB_Launch();//??flash,?????
       
      HAL_FLASH_OB_Lock();
      HAL_FLASH_Lock();//??????flash,?????????????
    }
    else
    {
      UART.printf("STOP IWDG no enabled\r\n");
    }
    UART.flush();
}

void get_date (RTC_DateTypeDef *sDateStruc,RTC_TimeTypeDef *sTimeStruc)
{
    
    if(HAL_RTC_GetTime(&hrtc,sTimeStruc,RTC_FORMAT_BIN) != HAL_OK)
    {
        UART.printf("HAL_RTC_GetTime ERR \r\n");
    }
    
    if(HAL_RTC_GetDate(&hrtc,sDateStruc,RTC_FORMAT_BIN) != HAL_OK)
    {
        UART.printf("HAL_RTC_GetDate ERR \r\n");
    }
}
void rtc_set_dt(DateTime &dt)
{

    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    sTime.Hours = dt.hour;
    sTime.Minutes = dt.minute;
    sTime.Seconds = dt.second;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
    sDate.WeekDay = dt.dayOfWeek();
    sDate.Month = dt.month;
    sDate.Date = dt.day;
    sDate.Year = dt.year - 2000;

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }

}

bool rtc_get_dt(DateTime *dt)
{
    RTC_DateTypeDef date;
    RTC_TimeTypeDef time;
    if(HAL_RTC_GetTime(&hrtc,&time,RTC_FORMAT_BIN) != HAL_OK)
    {
        UART.printf("HAL_RTC_GetTime ERR \r\n");
        return false;
    }
    if(HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN) != HAL_OK)
    {
        UART.printf("HAL_RTC_GetDate ERR \r\n");
        return false;
    }
    dt->parse(date.Year + 2000,date.Month,date.Date,time.Hours,time.Minutes,time.Seconds);
    return true;
}

void rtc_set_alarm(uint32_t sec)
{
    rtc_get_dt(&gdt);
    TimeSpan ts(sec);
    DateTime ndt = gdt + ts;
    
    UART.println("------next rtc alarm time-----");
    ndt.print(UART);
    
    RTC_AlarmTypeDef sAlarm = {0};
    
    sAlarm.AlarmTime.Hours = ndt.hour;
    sAlarm.AlarmTime.Minutes = ndt.minute;
    sAlarm.AlarmTime.Seconds = ndt.second;
    sAlarm.AlarmTime.SubSeconds = 0;
    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
//    sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_HOURS
//                              |RTC_ALARMMASK_MINUTES;
    sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
    
    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    sAlarm.AlarmDateWeekDay = ndt.day;
    sAlarm.Alarm = RTC_ALARM_A;
    if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    } 
}

void rtc_set_alarm_minute(uint32_t minute)
{
    limit(minute,(uint32_t)1,(uint32_t)20000);
    uint32_t sec = minute * 60;
    rtc_set_alarm(sec);
}

void MX_RTC_Init(void)
{


  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
    if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0)!=0X5A5A)//
    { 
        rtc_set_dt(gdt);
        HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0X5A5A);//
    }
    else
    {
        rtc_get_dt(&gdt);
    }
    //do not set alarm 
//    TimeSpan ts(35); 
//    rtc_set_alarm(ts);
    gdt.print(UART);



}

extern "C"
{

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(hrtc->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_RTC_ENABLE();
    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }

}
    
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
//    get_date(&rtc_date,&rtc_time);
//    UART.printf("RTC A alram\r\n");
//    rtc_set_alarm(rtc_interval);
}
void RTC_Alarm_IRQHandler(void)
{
    rt_enter_critical();
    HAL_RTC_AlarmIRQHandler(&hrtc);
    rt_exit_critical();
}
void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
//    
//    get_date(&gDateType,&gTimeType);
//    UART.printf("RTC %02d:%02d:%02d \r\n",gTimeType.Hours,gTimeType.Minutes,gTimeType.Seconds);
}
}
