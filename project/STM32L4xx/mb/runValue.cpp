#include "runValue.h"
#include "bsp_rtc.h"
#include "apps.h"
#include "config.h"
RunValue run;

RunList runList;

RunList::RunList()
{
}
void RunList::begin()
{
    mutex = rt_mutex_create("rlst", RT_IPC_FLAG_FIFO);
}
void RunList::add(RunSample *node)
{
    int ret;
    ret = rt_mutex_take(mutex, RT_WAITING_FOREVER);
    list.add(node);
    rt_mutex_release(mutex);

}
RunSample *RunList::get()
{
    RunSample *node;
    int ret;
    ret = rt_mutex_take(mutex, RT_WAITING_FOREVER);
    node = list.shift();
    rt_mutex_release(mutex);
    return node;
}
int RunList::size()
{
    int ret;
    int cnt;
    ret = rt_mutex_take(mutex, RT_WAITING_FOREVER);
    cnt = list.size();
    rt_mutex_release(mutex);
    return cnt;
}




void RunValue::begin()
{
    r0_hex = 999;//1
    net_state.value = 1;
    net_server.value = 11;
    bat_vol = 4.2;
    
    len = para_to_buf();
}
bool RunValue::warnning_check()
{
    //
    if(r24_nadv <= config_threshold.off_dc_min)return true;
    if(r26_nbdv <= config_threshold.off_dc_min)return true;
    
    if(r24_nadv >= config_threshold.off_dc_max)return true;
    if(r26_nbdv >= config_threshold.off_dc_max)return true;
    
    if(r6_adv >= config_threshold.on_dc_max)return true;
    if(r14_bdv >= config_threshold.on_dc_max)return true;
    
    if(r8_aav >= config_threshold.on_ac_max)return true;
    if(r16_bav >= config_threshold.on_ac_max)return true;
    
    
    return false;
}
int RunValue::para_to_buf()
{
    DataU16_t XUint16;    
    DataFloat_t xFloat;    
    uint16_t counter = 0;

    XUint16.value = r0_hex;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];
    XUint16.value = r1_gain;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];  
    xFloat.value = r2_voltage;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = r4_rpl;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
            
    xFloat.value = r6_adv;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = r8_aav;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = r10_adc;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = r12_aac;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];

    xFloat.value = r14_bdv;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = r16_bav;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = r18_bdc;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = r20_bac;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];

    xFloat.value = r22_ndv;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = r24_nadv;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = r26_nbdv;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = r28_ncuznv;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];


    XUint16.value = gdt.year;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];
    XUint16.value = gdt.month;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];
    XUint16.value = gdt.day;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];
    XUint16.value = gdt.hour;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];
    XUint16.value = gdt.minute;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];
    XUint16.value = gdt.second;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];



    XUint16.value = gm5.net_connected;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];
    XUint16.value = gm5.mqtt_state;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];
                
    xFloat.value = bat_vol;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];


    xFloat.value = gps.location.lng();
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = gps.location.lat();
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
        
    xFloat.value = sys.cpu;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];

    xFloat.value = sys.mem.percent;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];

    xFloat.value = adc1.read_temp();
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];

//     print_buf(buf,len);        
    len = counter;
    return counter;
}


