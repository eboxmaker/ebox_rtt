#include "apps.h"
#include "analog_ch.h"
#include "runValue.h"
#include "modbus_addr.h"
#include "linearRegression/Linear.h"
#include "config.h"
#include "low_power.h"




#define REG_ADC 225
#define REG_GAIN (REG_ADC + 1)
#define REG_VOL (REG_GAIN + 1)
#define REG_RPL (REG_VOL + 2)
#define REG_CNT (REG_RPL + 2)
#define REG_R (REG_CNT + 1)
#define REG_K (REG_R + 2)
#define REG_B (REG_K + 2)
#define REG_OUT (REG_B + 2)


static bool cal1_task_state = false;
rt_thread_t cal1_thread;


Linear linear[4];
uint8_t cal_ch = 0;



void cal_update_reg();
void cal_clear(uint8_t ch);
static void set_run_para(Relay::Mode_t ch,float val);



void app_cal1_delete()
{
    if(cal1_task_state)
    {
        appDebug("app cal1 stoping(thread:0X%08X)\n",cal1_thread);
        rt_thread_delete(cal1_thread);
        cal1_task_state = false;
        appDebug("app_cal1 stop by other one(thread:0X%08X)\n",cal1_thread);
        cal_clear(cal_ch);
        mb.Hreg(REG_CH,0);
        mb.Hreg(REG_SAVE,0);
        cal_update_reg();
        lowpower_standby_unlock(PWR_BIT_INDEX_CAL);
    }
    else
    {
        appDebug("app cal1 was stoped.(thread:0X%08X)\n",cal1_thread);
    }
}


void cal1_thread_entry(void* paremeter)
{
    cal1_task_state = true;
    lowpower_standby_lock(PWR_BIT_INDEX_CAL);
    appDebug("------------------\n");
    appDebug("loop_cal start\n");
    cal_set_ch(1);
    while(1)
    {
        cal_update_reg();
        delay_ms(200);
    }
    cal1_task_state = false;
    appDebug("loop_cal end: %d\n",sample_mode);
    lowpower_standby_unlock(PWR_BIT_INDEX_CAL);
    return ;
}

int app_cal1_create()
{
    cal1_thread = rt_thread_create( "cal",     /*name*/
                                    cal1_thread_entry,/*入口*/
                                    RT_NULL,/*参数*/
                                    1024*4,    /*heap*/
                                    2 ,    /*优先级*/
                                     20);   /*时间片段*/
    if(cal1_thread !=RT_NULL)
    {
        rt_thread_startup(cal1_thread);
    }
    else 
    {
        appDebug("create failed\n");
        return -1;
    }
    return 0;
}

void cal_set_ch(uint8_t ch)
{
    if(sample_mode != WORK_CAL)    
    {
        mb.Hreg(REG_CH,0);
        return;
    }
    if(ch >= 1 && ch <= 4)
    {
        cal_ch = ch;
    }
    else
    {
        mb.Hreg(REG_CH,cal_ch);
        return ;
    }
    
    linear[ch - 1].clear();
    cal_update_reg();
    mb.Hreg(REG_CH,cal_ch);
    appDebug("cal_set_ch:%d\n",cal_ch);
    switch(ch)
    {
        case 1:
            set_relay_ch(Relay::Mode1ADV);
            break;
        case 2:
            set_relay_ch(Relay::Mode2AAV);
            break;
        case 3:
            set_relay_ch(Relay::Mode3ADC);
            appDebug("Relay:0x%04x\n",Relay::Mode3ADC);
            break;
        case 4:
            set_relay_ch(Relay::Mode4AAC);
            break;
        default:
            break;
    }
}
void cal_add_data(float value)
{
    
    if(cal_ch>=1 && cal_ch<=4)
    {
        float vol = adc.read_voltage();
        linear[cal_ch - 1].add_row(vol,value);
        appDebug("HEX:%0.0f; data:%0.6f,rows:%d\n",vol,value,linear[cal_ch - 1].get_rows());
        linear[cal_ch - 1].exe();
    }
}
bool cal_set_kb(uint8_t ch)
{
    if(ch >= 1 && ch <= 4)
    {
        if(linear[ch - 1].avaliable())
        {
            config_cal.k[ch - 1] = linear[ch - 1].rate;
            config_cal.b[ch - 1] = linear[ch - 1].offset;
            linear[ch - 1].clear();
            config.para_to_frame();
            mb_frame_to_hreg();
            config.save();        
            return true;
        }
    }
    return false;
}
void cal_clear(uint8_t ch)
{
    if(ch >= 1 && ch <= 4)
    {
        linear[ch - 1].clear();
    }
}
void cal_save(bool val)
{
    if(sample_mode!= WORK_CAL)    
    {
        mb.Hreg(REG_SAVE,0);
        return;
    }
    
    if(val == 0)
    {
        cal_clear(cal_ch);
        return;
    }
    if(cal_set_kb(cal_ch))
    {
        mb.Hreg(REG_SAVE,0);
    }
    else
    {
        mb.Hreg(REG_SAVE,2);//err
    }
    cal_clear(cal_ch);
    return;
}

void cal_update_reg()
{
    float vol = adc.read_voltage();
    
    mb.Hreg(REG_ADC,adc.read_hex());
    mb.Hreg(REG_GAIN,adc.read_gain());
    float_to_hreg(REG_VOL,vol);
    float_to_hreg(REG_RPL,adc.rpl->get());

    if(cal_ch >= 1 && cal_ch <= 4)
    {
        float out = linear[cal_ch - 1].rate * vol + linear[cal_ch - 1].offset;
        mb.Hreg(REG_CNT,linear[cal_ch - 1].get_rows());
        float_to_hreg(REG_R,linear[cal_ch - 1].get_r());
        float_to_hreg(REG_K,linear[cal_ch - 1].rate);
        float_to_hreg(REG_B,linear[cal_ch - 1].offset);
        float_to_hreg(REG_OUT,out);
//        appDebug("get_rows:%d\n",linear[cal_ch - 1].get_rows());
//        appDebug("rate:%0.6f\n",linear[cal_ch - 1].rate);
//        appDebug("offset:%0.6f\n",linear[cal_ch - 1].offset);
    }
}

