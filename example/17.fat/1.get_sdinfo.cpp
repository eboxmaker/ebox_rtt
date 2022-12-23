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
#include "mmc_sd.h"



ebox::SD sd(&PB12, &mcuSpi2);

void getSDCardInfo()
{
    uint8_t ret;
    uint64_t rl;
    uint8_t buf[1024];

    ret = sd.get_CID(buf);
    uart1.printf("\r\n========================");
    uart1.printf("\r\nget CID Info,ret = %d", ret);
    uart1.printf("\r\n");
    uart1.printf((const char *)buf);

    rl = sd.size();
    uart1.printf("\r\n========================");
    uart1.printf("\r\ncapacity = %dMB", rl / 1024 / 1024);
    uart1.printf("\r\ncapacity = %0.1fGB", rl / 1024 / 1024 / 1024.0);

    uart1.printf("\r\nWaiting...");


}
void setup()
{
    uint8_t ret;
    ebox_init();
    uart1.begin(115200);
    sd.init();
  

}
int main(void)
{
    setup();
    getSDCardInfo();
    while(1)
    {

        delay_ms(1000);
    }


}

