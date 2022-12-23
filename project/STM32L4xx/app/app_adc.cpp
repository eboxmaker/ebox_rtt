#include "apps.h"
#include "rtt_utils.h"
#include "filters.h"
#include "runValue.h"
#include "modbus_addr.h"
#include "relay.h"
#include "ripple.h"


void rpl_stable(float rpl_value)
{

}

void app_adc_thread_entry(void* paremeter)
{
    adc.begin();
    run.begin();
    adc.rpl->attach(rpl_stable);
    appDebug("adc thread is started!\n");
    while(1)
    {
        adc.loop();
        if(adc.is_update())
        {
//            appDebug("ripple.get():%0.6f,adc.voltage:%0.6fv\n",adc_ripple.get(),adc.read_voltage());
        }
        delay_ms(10);
    }
    return ;
}

int app_adc_create()
{
    rt_thread_t app = rt_thread_create( "adc",     /*name*/
                                    app_adc_thread_entry,/*入口*/
                                    RT_NULL,/*参数*/
                                    1024*4,    /*heap*/
                                    1 ,    /*优先级*/
                                     200);   /*时间片段*/
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
