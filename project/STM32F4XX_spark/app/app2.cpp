#include "app2.h"
#include "rtt_utils.h"

    
void app2_thread_entry(void* paremeter)
{
    while(1)
    {
        list_thread();
        float usage = cpu_usage_get_float();
        uart1.printf("cpu:%0.2f\n",usage);
//        RTTUtils::thread_info(rt_thread_self());
        delay_ms(1000);
    }
    return ;
}

int app2_create()
{
    rt_thread_t app2_thread = rt_thread_create( "app2",     /*name*/
                                    app2_thread_entry,/*���*/
                                    RT_NULL,/*����*/
                                    1024,    /*heap*/
                                  3 ,    /*���ȼ�*/
                                     20);   /*ʱ��Ƭ��*/
    if(app2_thread !=RT_NULL)
    {
        rt_thread_startup (app2_thread);
    }
    else 
    {
        rt_kprintf("create failed\n");
        return -1;
    }
    return 0;
}
