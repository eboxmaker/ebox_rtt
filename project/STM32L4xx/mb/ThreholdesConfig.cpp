#include "config.h"


void ConfigThreshold::begin()
{  
    off_dc_min = -1.2;
    off_dc_max = -0.85;
    on_dc_max = 4;
    on_ac_max = 4;
    
    para_to_buf();
}

int ConfigThreshold::para_to_buf()
{
    DataFloat_t xFloat;    
    uint16_t counter = 0;

    xFloat.value = off_dc_min;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = off_dc_max;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = on_dc_max;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    xFloat.value = on_ac_max;
    for(int i = 0; i < 4; i++)
        buf[counter++] = xFloat.byte[i];
    
    len = counter;
    
//    print_buf(buf,len);
    return counter;
}




int ConfigThreshold::buf_to_para()
{
    DataFloat_t xFloat;
    uint16_t counter = 0;
   
    
    for(int i = 0; i < 4; i++)
        xFloat.byte[i] = buf[counter++];
    off_dc_min = xFloat.value;
    for(int i = 0; i < 4; i++)
        xFloat.byte[i] = buf[counter++];
    off_dc_max = xFloat.value;
    for(int i = 0; i < 4; i++)
        xFloat.byte[i] = buf[counter++];
    on_dc_max = xFloat.value;
    for(int i = 0; i < 4; i++)
        xFloat.byte[i] = buf[counter++];
    on_ac_max = xFloat.value;

    len = counter;

    return counter;
}


void ConfigThreshold::print(Uart &uart)
{
    cfgDebug("========ConfigThreshold (%d)===========\n",len);
    cfgDebug("off_dc_min:%dmv\n",(int)(off_dc_min*1000));      
    cfgDebug("off_dc_max:%dmv\n",(int)(off_dc_max*1000));      
    cfgDebug("on_dc_max:%dmv\n",(int)(on_dc_max*1000));      
    cfgDebug("on_ac_max:%dmv\n",(int)(on_ac_max*1000));           
    cfgDebug("================================\n");

    
}
