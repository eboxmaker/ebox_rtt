#include "apps.h"
#include "bsp_ebox.h"
#include "log.h"
#include "mqtt.h"


rt_sem_t sem_save_over;
static void app_save_thread_entry(void* paremeter)
{
    
    sem_save_over = rt_sem_create("save", 1,RT_IPC_FLAG_FIFO );
    if(RT_NULL != sem_save_over)/*  */
    {
        appDebug("The semaphore was successfully created\r\n");
    }
    else/*  */
    {
        appDebug("Failed to create semaphore\r\n");
        return;
    }

    
    while(!gm5.is_stable)
    {
        appDebug("save:wait gm5.is_stable:%d\n",gm5.is_stable);
        delay_ms(1000);
    }
    while(1)
    {
        if(runList.size())
        {   
            int ret = rt_sem_take (sem_save_over, RT_WAITING_FOREVER);           
            RunSample *node = runList.get();
            ret = mqtt_property_post(node);
            if(ret)
            {
                elog.add(node);
            }
            delete node;
            rt_sem_release(sem_save_over);
        }
        delay_ms(100);
    }
}

int app_save_create()
{
    rt_thread_t app1_thread = rt_thread_create( "save",     /*name*/
                                    app_save_thread_entry,/*入口*/
                                    RT_NULL,/*参数*/
                                    1024*4,    /*heap*/
                                    5 ,    /*优先级*/
                                     20);   /*时间片段*/
    if(app1_thread !=RT_NULL)
        rt_thread_startup (app1_thread);
    else 
        return -1;
    return 0;
    
}