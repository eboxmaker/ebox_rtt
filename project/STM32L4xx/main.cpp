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
#include "log.h"
#include "low_power.h"


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"pipeline"
#define EXAMPLE_DATE	"2021-02-27"

/*******************************
dev     uart    ctr     pwr     
pc      uart1   PC13
bat     uart2   PE3
4g      lpuart1       
gps     uart3           PB1
*******************************/


void setup() {
    
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  { 
    /* Clear Standby flag */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB); 
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF1);
//    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF3);
  }
  
    PE3.mode(OUTPUT_PP_PN);
    PE3.set();    
    uart2.begin(115200,RxIt);
//    PC13.mode(OUTPUT_PP_PN);
//    PC13.set();    
//    lpuart1.begin(115200,RxIt);
    print_log();
      
    bat_analog_init();
    rtc_init();
  
  //必须要启动的项目
    lowpower_init();
    power_init();
    power_on(PwrAnalog);
    delay_ms(10);
    power_on(PwrDigital);
    delay_ms(10);
    power_on(PwrBatCheck);
//    power_on(PwrAll);

  
//    w25.begin();
    LED.mode(OUTPUT_PP_PD);
    config.begin();
    config.print(UART);
    
    relay.begin();
    relay.mode(Relay::Mode0DEFAULT);
    delay_ms(10);
    relay.mode(Relay::Mode13OPEN);

    if(!config.gps_ctr)
    {
        power_off(PwrGps);
    }
    else
    {
//        power_on(PwrGps);
    }
    rtc_set_alarm_minute(config.sample_regu_interval);
    
    elog.begin();
    run.begin();
    
    gm5.begin();
    mqtt_init();
    
    app_create();
    app_utils_create();
    app_mb_create();
    delay_ms(100);
    app_adc_create();
    app_gps_create();
    app_mqtt_create();
    app_save_create();
    runList.begin();
    if(config.sampleMode == 0)
    {
        app_regu_create();
    }
    else
    {
        app_cont_create();
    }

//    dog.begin(3000);
}

int index = 0;
uint32_t last = 0;
int main(void)
{
    setup();

    while(1)
    {
        index++;
        if(index == 10)
        {
            
        }
        relay.loop();
        rtc_loop();
        if(millis() - last > 500)
        {
            last = millis();
            PB4.toggle();;
            
//            UART.printf("==========================\r\n");
//            UART.printf("adc0:0x%x\t(%0.1fmv)\r\n", adc1.read(0), adc1.read_voltage(0));
//            UART.printf("adc temp:\t(%0.1f℃)\r\n", adc1.read_temp());
//            UART.printf("adc vbat:\t(%0.1fmv)\r\n", adc1.read_vbat());
//            UART.printf("adc vref:\t(%0.1fmv)\r\n", adc1.read_vref());
            run.bat_vol =  adc1.read_voltage(0) *(4.95 / 1.96) / 1000;
//            dog.feed();
//            print_memory_info();
//            UART.printf("main %02d:%02d:%02d \r\n",gdt.hour,gdt.minute,gdt.second);
        }
        gm5.loop();
        delay_ms(10);
        
        

    }
}


extern "C"
{



}
