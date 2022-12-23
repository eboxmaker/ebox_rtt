#include "app1.h"

#include "lwip.h"

#include "tcpclient.h"
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"TCP client example"
#define EXAMPLE_DATE	"2018-08-11"

extern struct netif gnetif;



static void app1_thread_entry(void* paremeter)
{
    MX_LWIP_Init();
    delay_ms(10000);
    TCP_Client_Init();
    while(1)
    {
       MX_LWIP_Process();
//        uart1.printf("0X%02X\n",gnetif.link);
        delay_ms(1);
    }
}

int app1_create()
{
    rt_thread_t app1_thread = rt_thread_create( "app1",     /*name*/
                                    app1_thread_entry,/*入口*/
                                    RT_NULL,/*参数*/
                                    1024,    /*heap*/
                                    0 ,    /*优先级*/
                                     20);   /*时间片段*/
    if(app1_thread !=RT_NULL)
        rt_thread_startup (app1_thread);
    else 
        return -1;
    return 0;
    
}