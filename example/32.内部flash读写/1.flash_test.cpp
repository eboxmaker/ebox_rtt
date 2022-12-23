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


Flash flash;

void setup()
{
    print_log();
    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP); 
    flash.begin();
}
void flash_test();
int main(void)
{

    setup();
    while (1)
    {
        flash_test();
    }
}
void flash_test()
{
    int ret = 0;
    int err = 0;
    uint8_t wbuf[64];
    uint8_t rbuf[64];
    #define MAX_LEN 32
    
    UART.printf("=================wbuf================\r\n");
    for(uint16_t i = 0; i < MAX_LEN; i++)
    {
        wbuf[i] = random()%255;
        rbuf[i] = 0;
    }
    for(uint16_t i = 0; i < MAX_LEN; i++)
    {
        UART.printf(" %02x ", wbuf[i ]);
    }
    UART.printf("\r\n ");
    UART.flush();

    ret = flash.write(1020, wbuf, MAX_LEN);
    if(ret)
        UART.printf("write:%d\r\n",ret);

    UART.printf("==================rbuf==============\r\n");
    UART.flush();
    flash.read(1020, rbuf, MAX_LEN);
    for(uint16_t i = 0; i < MAX_LEN; i++)
    {
        UART.printf(" %02x ", rbuf[i]);
    }
    UART.printf("\r\n ");
        UART.flush();

    
    for(int i = 0; i < MAX_LEN; i++)
    {
        if(wbuf[i] != rbuf[i])
        {
            err = 1;
            break;
        }
    }
    if(err == 1)
    {
        UART.printf("flash check ...[err]");
    }
    else
        UART.printf("flash check ...[OK]");

    UART.printf("\r\n================================\r\n");
    delay_ms(1000);

}
