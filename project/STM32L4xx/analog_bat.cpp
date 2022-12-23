#include "bsp_ebox.h"


void bat_analog_init()
{
    
    adc1.add_ch(&PC2);
    adc1.add_temp();
    adc1.add_vbat();
    adc1.add_vref();
    adc1.begin();
}

