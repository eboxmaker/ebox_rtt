#include "apps.h"
#include "bsp_ebox.h"
#include "log.h"

static void app_thread_entry(void* paremeter)
{
    while(1)
    {
        
//        uint32_t mark = millis();

//        String str = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
//                      1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
//        1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
//        1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
//        1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
//        mark = millis();
//        UART.printf("----------elog.add(str)------------\n",millis() - mark);
//        elog.add(str);
//        UART.printf("cost add:%d\n",millis() - mark);
//        UART.printf("------------------------------------\n",millis() - mark);


//        UART.printf("kick:%d\n",gm5.kick());
//        UART.printf("wkmod:%s\n",gm5.wkmod("NET").c_str());
//        uart1.printf("0X%02X\n",gnetif.link);
        delay_ms(2000);
    }
}

int app_create()
{
    rt_thread_t app1_thread = rt_thread_create( "app",     /*name*/
                                    app_thread_entry,/*入口*/
                                    RT_NULL,/*参数*/
                                    1024*4,    /*heap*/
                                    0 ,    /*优先级*/
                                     20);   /*时间片段*/
    if(app1_thread !=RT_NULL)
        rt_thread_startup (app1_thread);
    else 
        return -1;
    return 0;
    
}