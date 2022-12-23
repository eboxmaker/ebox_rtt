/**
  ******************************************************************************
  * @file    bsp_ebox.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2018/07/31
  * @brief   硬件相关信息声明
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
#include "bsp_ebox.h"


SoftI2c bsp_i2c(&PB8, &PB9);
mcuSpi	bsp_spi(SPI1, &PB3, &PB4, &PB5);
SoftSpi	bsp_sspi(&PB3, &PB4, &PB5);
SoftSpi	bsp_sd_sspi(&PD2, &PC8, &PC12);
//SoftSpi	bsp_sd_sspi(&PC12, &PC8, &PD2);

W25xxx w25(&PD7,&bsp_spi);

SD sd(&PC11,&bsp_sd_sspi);
At24x ee(&bsp_i2c);


/*
*********************************************************************************************************
*	函 数 名: PrintfLog
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void print_progress(float percent,int len = 10)
{
    limit(len,10,30);
    int cnt = (percent*len);
    UART.print("[");
    for(int i = 0; i < len;i++)
    {
        if(i < cnt)
            UART.print("*");
        else
            UART.print("_");
    }
    UART.print("]");
}
void print_log(const char *name, const char *date)
{
    UART.print("\n");
    UART.printf("*******************************************\n");
    UART.printf("* \r\n");	                /* 打印一行空格 */
    UART.printf("* name  : %s\n",name);	    /* 打印例程名称 */
    UART.printf("* hard  : %s\n",HARDWARE);       /* 硬件平台 */
    UART.printf("* eBox  : V%s,build %s\n" ,EBOX_VERSION,date);/* 版本信息 */
    UART.printf("* HAL   : V%d.%d.%d.%d\n" ,bits_get(HAL_GetHalVersion(),24,31)
                                  ,bits_get(HAL_GetHalVersion(),16,23)
                                  ,bits_get(HAL_GetHalVersion(),8,15)
                                  ,bits_get(HAL_GetHalVersion(),0,7));/* 版本信息 */



    UART.printf("* cpuID  : ");       /* 打印CPU唯一ID */
    for(int i = 0 ; i <11; i++)
    {
        UART.printf("%02X-",cpu.chip_id[i],HEX);
        
    }
    UART.printf("%02X\n",cpu.chip_id[11],HEX);

    


    UART.print("* core  : ");
    UART.print(cpu.clock.core / 1000000.0);	/* 打印时钟树信息 */
    UART.println("Mhz");
    UART.print("* hclk  : ");
    UART.print(cpu.clock.hclk / 1000000.0);
    UART.println("Mhz");
    UART.print("* pclk1 : ");
    UART.print(cpu.clock.pclk1 / 1000000.0);
    UART.println("Mhz");
    UART.print("* pclk2 : ");
    UART.print(cpu.clock.pclk2 / 1000000.0);
    UART.println("Mhz");
    

    UART.print("* flash : ");	    /* 打印flash使用了多少KB */
    print_progress((float)(cpu.flash.used)/(cpu.flash.size ),20);
    UART.printf("%0.1fKB/%0.1fKB\n",cpu.flash.used/1024.0, cpu.flash.size/1024.0);

    UART.print("* mem   : ");	    /* 打印SRAM剩余了多少KB */
    print_progress((float)(MCU_SRAM1_USED)/(MCU_SRAM1_SIZE ),20);
    UART.printf("%0.2fKB/%0.2fKB\r\n",MCU_SRAM1_USED/1024.0, MCU_SRAM1_SIZE/1024.0);
    UART.printf("**********************************************\n");
    UART.flush();
}
extern "C"{
void user_uart_init()
{
    uart1.begin(115200);
}

void user_uart_out(const char *str)
{
    uart1.write(str,rt_strlen(str));
    uart1.flush();
}
}


