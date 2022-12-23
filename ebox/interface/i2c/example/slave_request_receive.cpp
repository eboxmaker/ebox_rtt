/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-05     yangjie      First edition
 */

#include "bsp_ebox.h"
#include "app1.h"
#include "app2.h"
#include "ebox_timer.h"
#include "ebox_adc.h"
#include "Wire.h"
#include "ebox_mcu_i2c.h"
    
mcuI2c mI2c2(I2C2,&PB10,&PB11);
#define tw mI2c2

void requestEvent() {
    tw.write("123");
}
void receiveEvent(int howMany)
{
    rt_kprintf("slave:I2C receive:%d[",howMany);         
    while( tw.available()>0) 
    {
        char c = tw.read(); 
        Serial.print(c);         
    }
    Serial.printf("]\n");         
    PB8.toggle();
}
void setup()
{
    print_log();
    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP); 
//    app1_create();
    app2_create();

    tw.begin(8);
    tw.onRequest(requestEvent); // register event
    tw.onReceive(receiveEvent);
}
void flash_test();
int main(void)
{

    setup();
    while (1)
    {
        PB8.toggle();
        delay_ms(500);
    }
}
