#include "apps.h"
#include "rtt_utils.h"

    
void app_utils_thread_entry(void* paremeter)
{
    while(1)
    {
//        list_thread();
        sys.cpu = cpu_usage_get_float();
        get_memory_info();
//        UART.printf("cpu:%0.2f\n",usage);
//        RTTUtils::thread_info(rt_thread_self());       
        delay_ms(1000);
    }
    return ;
}

int app_utils_create()
{
    rt_thread_t app = rt_thread_create( "utils",     /*name*/
                                    app_utils_thread_entry,/*���*/
                                    RT_NULL,/*����*/
                                    1024*2,    /*heap*/
                                    7 ,    /*���ȼ�*/
                                    20);   /*ʱ��Ƭ��*/
    if(app !=RT_NULL)
    {
        rt_thread_startup (app);
    }
    else 
    {
        appDebug("create failed\n");
        return -1;
    }
    return 0;
}
