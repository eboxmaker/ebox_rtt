/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "bsp_ebox.h"
#include "relay.h"
#include <stdlib.h>
#include "apps.h"
#include "sdio.h"
#include "power.h"

#include "ebox_flash.h"
#include "config.h"

#include "bsp_rtc.h"

#include "mqtt.h"
#include "runValue.h"
#include "modbus_addr.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"pipeline"
#define EXAMPLE_DATE	"2021-02-27"
PFlash pflash;


/*******************************
dev     uart        pwr     ctr
pc      lpuart1     PE2     PC13
bat     uart2       PA4     PA0
gps     uart3       PB1
4g      uart1       PA8
*******************************/
  uint8_t wbuf[512],rbuf[512];

void setup() {
   
    uart2.begin(115200,RxIt);
//    lpuart1.begin(115200,RxIt);
    print_log();
    pflash.begin();
    

}

int index = 0;
uint32_t last = 0;
int main(void)
{
    setup();

    while(1)
    {
        
        for(int i =0 ;i <512; i++)
        {
            wbuf[i] = i+index;
        }
        last = millis();
        pflash.put(wbuf,512);
        uart2.printf("time :%d\n",millis() - last);
        last = millis();
        pflash.get(rbuf,512);
        uart2.printf("time :%d\n",millis() - last);
        print_buf(rbuf,512);
        index++;
        delay_ms(200);
    }
}


extern "C"
{



}
