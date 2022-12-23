#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#include "ebox_core.h"
#include "stm32_hal.h"
#include "DateTime.h"


extern DateTime gdt;

void rtc_init();
void rtc_loop();
void rtc_set_alarm_minute(uint32_t minute);
void rtc_set_dt(DateTime &dt);





class eBoxRtc
{
    public:
        eBoxRtc();
        void begin();
        void loop();
    public:
        DateTime dt;

};

#endif
