#include "apps.h"
#include "analog_ch.h"
#include "runValue.h"
#include "modbus_addr.h"
#include "config.h"
#include "mqtt.h"
#include "low_power.h"

bool cont_task_state = false;
rt_thread_t cont_thread;
static void set_run_para(Relay::Mode_t ch,float val);
static int cont_round = 0;

void app_cont_delete()
{
    if(cont_task_state)
    {
        appDebug("app cont stoping(thread:0X%08X)\n",cont_thread);
        rt_thread_delete(cont_thread);
        cont_task_state = false;
        appDebug("app_cont stop by other one(thread:0X%08X)\n",cont_thread);
        lowpower_standby_unlock(PWR_BIT_INDEX_CONT);
    }
    else
    {
        appDebug("app cont was stoped.(thread:0X%08X)\n",cont_thread);
    }
}


void cont_thread_entry(void* paremeter)
{
    cont_task_state = true;
    lowpower_standby_lock(PWR_BIT_INDEX_CONT);
    delay_ms(1000);
    appDebug("!!!!!!!!!continue sample start[%d]\n",millis());
    uint32_t mark1,mark2;
    DateTime start = gdt;
    cont_round = 1;
    while(1)
    {
        int dc_ch_timeout = (1000*config.pipe_on_time/8.0) * 0.8;
        int ac_ch_timeout = (1000*config.pipe_on_time/8.0) * 1.2;
        int off_ch_timeout = 1000*config.pipe_off_time/4.0;
        
        appDebug("------------------\n");
        appDebug("all :%d\n",config.pipe_on_time);
        appDebug("dc_ch_timeout :%d\n",dc_ch_timeout);
        appDebug("ac_ch_timeout :%d\n",ac_ch_timeout);
        appDebug("off_ch_timeout :%d/%d\n",off_ch_timeout,config.pipe_off_time);
        RunSample *temp = new RunSample();
        mark1 = millis();
        for(int i = 0; i < 8; i++)
        {
            mark2 = millis();
            if(relay.isAcCh((Relay::Mode_t)(i+1)))
            {
                temp->val[i] = analog_get((Relay::Mode_t)(i+1),ac_ch_timeout);
            }
            else
            {
                temp->val[i] = analog_get((Relay::Mode_t)(i+1),dc_ch_timeout);
            }
            appDebug("[%d]time:%d,value:%d\n",i+1,millis() - mark2,(int)(temp->val[i]*1000));
        }
        for(int i = 8; i < 12; i++)
        {
            mark2 = millis();
            temp->val[i] = analog_get((Relay::Mode_t)(i+1),off_ch_timeout);
            appDebug("[%d]time:%d,value:%d\n",i+1,millis() - mark2,(int)(temp->val[i]*1000));
        }   
        for(int i = 0; i < 12; i++)
        {
            set_run_para((Relay::Mode_t)(i+1),temp->val[i]);
        }    
        run.dt = gdt;
        temp->dt = gdt;
        temp->round = cont_round;
        temp->type = Config::SampleContinuous;
        runList.add(temp);
        cont_round++;
        appDebug("sample time:-------------%d\n",millis() - mark1);
        delay_ms(config.sample_cont_interval*1000);
        //sample time end
        TimeSpan ts = gdt - start;
        appDebug("continue mode run total sec ----------------------------%d\n",(int)ts.total_seconds);
        if(ts.total_seconds > config.sample_cont_duration*60)
        {
            if( PA10.read() == 0)
            {
                break;
            }
            else
            {
                appDebug("PA10.read() == 1, cant enter standby mode ------------------- \n");
            }
        }
    }
    cont_task_state = false;
    while(runList.size())
    {
            delay_ms(1000);
    }
    uint32_t mark = millis();
    int ret = rt_sem_take(sem_save_over,5000);
    appDebug("rt_sem_take:%d\n",millis() - mark);
    lowpower_standby_enter(PWR_BIT_INDEX_CONT);
    rt_sem_release(sem_save_over);
    return ;
}

int app_cont_create()
{
    cont_thread = rt_thread_create( "cont",     /*name*/
                                    cont_thread_entry,/*入口*/
                                    RT_NULL,/*参数*/
                                    1024*8,    /*heap*/
                                    2 ,    /*优先级*/
                                     20);   /*时间片段*/
    if(cont_thread !=RT_NULL)
    {
        rt_thread_startup(cont_thread);
    }
    else 
    {
        appDebug("create failed\n");
        return -1;
    }
    return 0;
}




static void set_run_para(Relay::Mode_t ch,float val)
{
    switch(ch)
    {
        case 1:
            run.r6_adv = val;break;
        case 2:
            run.r8_aav = val;break;
        case 3:
            run.r10_adc = val;break;
        case 4:
            run.r12_aac = val;break;
        case 5:
            run.r14_bdv = val;break;
        case 6:
            run.r16_bav = val;break;
        case 7:
            run.r18_bdc = val;break;
        case 8:
            run.r20_bac = val;break;
        case 9:
            run.r22_ndv = val;break;
        case 10:
            run.r24_nadv = val;break;
        case 11:
            run.r26_nbdv = val;break;
        case 12:
            run.r28_ncuznv = val;break;
        default:
            break;
    }

}