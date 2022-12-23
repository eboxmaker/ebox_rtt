#include "apps.h"
#include "log.h"
#include "low_power.h"
#include "mqtt.h"

#include "./storage/filesystem/FileSystem.h"
#include "./storage/filesystem/FAT/FATFileSystem.h"
#include "./storage/filesystem/file.h"
#include "./storage/filesystem/dir.h"

static bool history_ctr,history_state;

static char buf[1024];
bool over = false;
void dir_tree(const char *path)
{
}

static void history_thread_entry(void* paremeter)
{
    appDebug("=====history_thread_entry started=====\n");
    history_state = true;
    lowpower_standby_lock(PWR_BIT_INDEX_HISTORY);
    while(history_ctr)
    {
        if(gm5.mqtt_state)
        {
            String str = elog.get();
            if(str != "")
            {
                int trytimes = 3;
                while(trytimes)
                {
                    if(gm5.mqttpub(event_property_post,str))
                    {
                        appDebug("|||history gm5.mqttpub failed\n");
                        delay_ms(1000);
                        trytimes--;                   
                    }
                    else
                    {
                        break;
                    }
                }
                if(trytimes == 0)
                    break;
            }
            else
            {
                history_ctr = 0;
            }
        }
        delay_ms(1000);
    }
    history_state = false;
    appDebug("=======history_thread_entry stoped=============\n");

    lowpower_standby_enter(PWR_BIT_INDEX_HISTORY);
}

int app_history_create()
{
    rt_thread_t app1_thread = rt_thread_create( "history",     /*name*/
                                    history_thread_entry,/*入口*/
                                    RT_NULL,/*参数*/
                                    1024*5,    /*heap*/
                                    5 ,    /*优先级*/
                                     20);   /*时间片段*/
    if(app1_thread !=RT_NULL)
    {
        history_ctr = true;
        rt_thread_startup (app1_thread);
    }
    else 
        return -1;
    return 0;
}
void wait_stop()
{
    while(history_state)
    {
        delay_ms(100);
    }
}

void history_stop()
{
    history_ctr = false;
    wait_stop();
}
