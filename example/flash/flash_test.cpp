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
#include "apps.h"

/**
    *	1	此例程为IIC扫描程序
	*	2	此例程演示了扫描总线上所有地址有响应的设备
    *   3   通过begin可以设置i2c速率为10k，100k，200k，400k，默认100k
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"IIC Scan example"
#define EXAMPLE_DATE	"2021-02-27"


extern void rt_hw_board_init(void);


#include "ebox_flash.h"


PFlash fl;
uint8_t wbuf[256],rbuf[256];
void setup() {
    print_log();
    fl.begin();
    for(int i = 0; i < 18; i++)
    {
        wbuf[i] = i;
    }



}



int main(void)
{
    setup();
    while(1)
    {
        fl.put(wbuf,18);
        fl.get(rbuf,18);
        print_buf(rbuf,18);
        delay_ms(5000);
    }
}


