/**
 ******************************************************************************
 * @file    main.cpp
 * @author  shentq
 * @version V2.2
 * @date    2018/08/6
 * @brief   ebox application example .
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

#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1	������ʵ����ģ��ת����
	*       a ����һ��adc����
    *       b �����Ҫ��adcͨ��
    *       c begin(),����adc
    *       d ͨ��ͨ����(&PA0)��ͨ������(1)��ȡת�����
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"Adc example"
#define EXAMPLE_DATE	"2018-08-06"



//Adc adc(ADC1);
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);


    adc1.add_ch(&PA0);
    adc1.add_ch(&PA1);
    adc1.add_ch(&PA2);
    adc1.add_temp();
    adc1.add_vbat();
    adc1.add_vref();
    adc1.begin();
}

int main(void)
{
    setup();
    while(1)
    {
        UART.printf("========����IO��ѯ====================\r\n");
        UART.printf("adc0:0x%x\t(%0.1fmv)\r\n", adc1.read(&PA0), adc1.read_voltage(&PA0));
        UART.printf("adc11:0x%x\t(%0.1fmv)\r\n", adc1.read(&PA1), adc1.read_voltage(&PA1));
        UART.printf("adc12:0x%x\t(%0.1fmv)\r\n", adc1.read(&PA2), adc1.read_voltage(&PA2));

        UART.printf("========��������˳��====================\r\n");
        UART.printf("adc10:0x%x\t(%0.1fmv)\r\n", adc1.read(0), adc1.read_voltage(0));
        UART.printf("adc11:0x%x\t(%0.1fmv)\r\n", adc1.read(1), adc1.read_voltage(1));
        UART.printf("adc12:0x%x\t(%0.1fmv)\r\n", adc1.read(2), adc1.read_voltage(2));
        
        UART.printf("========internal src====================\r\n");
        UART.printf("read_temp:\t%0.1f��\r\n", adc1.read_temp());
        UART.printf("read_vbat:\t%0.1fmV\r\n", adc1.read_vbat());
        UART.printf("read_vref:\t%0.1fmV\r\n", adc1.read_vref());
        delay_ms(2000);
    }
}



