#include "power.h"
    
#define    PwrDigitalPin_p  PA4
#define    PwrDigitalPin_n  PE2
#define    PwrAnalogPin     PB2
#define    PweGpsPin        PB1
#define    PwrBatCheckPin   PC3

void power_init()
{
    PwrDigitalPin_p.mode(OUTPUT_PP_PU);
    PwrDigitalPin_n.mode(OUTPUT_PP_PU);
    PwrAnalogPin.mode(OUTPUT_PP_PU);
    PweGpsPin.mode(OUTPUT_PP_PU);
    PwrBatCheckPin.mode(OUTPUT_PP_PU);
    power_off(PwrAll);
}

void power_on(Power_t p)
{
    if(p == PwrAll)
    {
        PwrDigitalPin_p.reset();
        PwrDigitalPin_n.set();
        delay_ms(1);
        PwrAnalogPin.set();
        delay_ms(1);
        PweGpsPin.set();
        delay_ms(1);
        PwrBatCheckPin.set();
        delay_ms(10);
        PwrDigitalPin_n.reset();
    }
    else if(p == PwrDigital)
    {
        PwrDigitalPin_p.reset();
        PwrDigitalPin_n.set();
        delay_ms(10);
        PwrDigitalPin_n.reset();

    }
    else if(p == PwrAnalog)
    {
        PwrAnalogPin.set();
    }
    else if(p == PwrGps)
    {
        PweGpsPin.set();
    }
    else if(p == PwrBatCheck)
    {
        PwrBatCheckPin.set();
    }
}

void power_off(Power_t p)
{
    if(p == PwrAll)
    {
        PwrDigitalPin_p.set();
        PwrDigitalPin_n.reset();
        PwrAnalogPin.reset();
        PweGpsPin.reset();
        PwrBatCheckPin.reset();
        delay_ms(10);
        PwrDigitalPin_p.reset();
    }
    else if(p == PwrDigital)
    {
        PwrDigitalPin_p.set();
        PwrDigitalPin_n.reset();
        delay_ms(10);
        PwrDigitalPin_p.reset();
    }
    else if(p == PwrAnalog)
    {
        PwrAnalogPin.reset();
    }
    else if(p == PwrGps)
    {
        PweGpsPin.reset();
    }
    else if(p == PwrBatCheck)
    {
        PwrBatCheckPin.reset();
    }
}
void power_ctr(Power_t p,bool state)
{
    if(state)
    {
       power_on(p);
    }
    else
    {
        power_off(p);
    }
}