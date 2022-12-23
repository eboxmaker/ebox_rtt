#include "apps.h"
#include "modbus_addr.h"





    
static void app_mb_entry(void* paremeter)
{
      
    mb_init();

    
    while(1)
    {
        mb_task();
        delay_ms(10);
    }
    return ;
}

int app_mb_create()
{
    rt_thread_t thread = rt_thread_create( "mb",     /*name*/
                                    app_mb_entry,/*���*/
                                    RT_NULL,/*����*/
                                    1024*4,    /*heap*/
                                    0 ,    /*���ȼ�*/
                                     20);   /*ʱ��Ƭ��*/
    if(thread !=RT_NULL)
    {
        rt_thread_startup (thread);
    }
    else 
    {
        appDebug("app3 create failed\n");
        return -1;
    }
    return 0;
}

