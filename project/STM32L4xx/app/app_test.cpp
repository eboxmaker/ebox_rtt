#include "apps.h"
#include "analog_ch.h"
#include "runValue.h"
#include "modbus_addr.h"
#include "config.h"
#include "low_power.h"

bool test_task_state = false;
rt_thread_t test_thread;

Relay::Mode_t test_ch = Relay::Mode0DEFAULT;
bool test_ch_changed = false;

static void set_run_para(Relay::Mode_t ch,float val);

#define TEST_DELAY_SAMPLE 1000

void app_test_delete()
{
    if(test_task_state)
    {
        appDebug("app test stoping(thread:0X%08X)\n",test_thread);
        rt_thread_delete(test_thread);
        test_task_state = false;
        mb.Hreg(REG_TEST_CH,0);
        appDebug("app_test stop by other one(thread:0X%08X)\n",test_thread);
        lowpower_standby_unlock(PWR_BIT_INDEX_TEST);
    }
    else
    {
        appDebug("app test was stoped.(thread:0X%08X)\n",test_thread);
    }
}


void test_thread_entry(void* paremeter)
{
    test_task_state = true;
    lowpower_standby_lock(PWR_BIT_INDEX_TEST);
    test_set_ch(1);
    set_relay_ch(test_ch);
    delay_ms(TEST_DELAY_SAMPLE);   

    appDebug("loop_test start\n");
    while(1)
    {
        if(test_ch_changed)
        {
            appDebug("test ch:%d\n",test_ch);
            set_relay_ch((Relay::Mode_t)(test_ch));
            delay_ms(TEST_DELAY_SAMPLE);  
            test_ch_changed = false;
        }
        float val = analog_get(test_ch);
        set_run_para(test_ch,val);

        run.r0_hex = adc.read_hex();;
        run.r1_gain = adc.read_gain();
        run.r2_voltage = adc.read_voltage();
        run.r4_rpl = adc.rpl->get();
        run.para_to_buf();
        mb_run_to_ireg();

        run.para_to_buf();
        mb_run_to_ireg();
        delay_ms(500);

    }
    test_task_state = false;
    mb.Hreg(REG_TEST_CH,0);
    appDebug("loop_test end\n");
    lowpower_standby_unlock(PWR_BIT_INDEX_TEST);
    return ;
}

int app_test_create()
{
    test_thread = rt_thread_create( "test",     /*name*/
                                    test_thread_entry,/*入口*/
                                    RT_NULL,/*参数*/
                                    1024*4,    /*heap*/
                                    2 ,    /*优先级*/
                                     20);   /*时间片段*/
    if(test_thread !=RT_NULL)
    {
        rt_thread_startup(test_thread);
    }
    else 
    {
        appDebug("create failed\n");
        return -1;
    }
    return 0;
}



void test_set_ch(uint8_t ch)
{
    if(sample_mode != WORK_TEST)    
    {
        mb.Hreg(REG_TEST_CH,0);
        return;
    }
    if(relay.isValidSampleCh((Relay::Mode_t)ch))
    {
        test_ch = (Relay::Mode_t)ch;
    }
    mb.Hreg(REG_TEST_CH,test_ch);
    test_ch_changed = true;
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