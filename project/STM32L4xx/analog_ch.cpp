#include "analog_ch.h"
#include "bsp_ebox.h"
#include "config.h"

float analog_get_cal(uint8_t index)
{
    float vol = adc.read_voltage();
    float value = config_cal.k[index] * vol  + config_cal.b[index];
//    UART.printf("index:%d\t hex:%0.0f\t value:%0.3f\t",index,adc.hex,value);
//    UART.printf("k:%0.6f\t b:%0.6f\n",config_cal.k[index],config_cal.b[index]);
    return value;
}


void set_relay_ch(Relay::Mode_t ch)
{
    if(!relay.isValidSampleCh(ch))return;

    switch((uint8_t)ch)
    {
        case 1:
            relay.mode(Relay::Mode1ADV);
            adc.set_ch_dc();
            break;
        case 2:
            relay.mode(Relay::Mode2AAV);
            adc.set_ch_ac();
            break;
        case 3:
            relay.mode(Relay::Mode3ADC);
            adc.set_ch_dc();
            break;
        case 4:
            relay.mode(Relay::Mode4AAC);
            adc.set_ch_ac();
            break;
        case 5:
            relay.mode(Relay::Mode5BDV);
            adc.set_ch_dc();
            break;
        case 6:
            relay.mode(Relay::Mode6BAV);
            adc.set_ch_ac();
            break;
        case 7:
            relay.mode(Relay::Mode7BDC);
            adc.set_ch_dc();
            break;
        case 8:
            relay.mode(Relay::Mode8BAC);
            adc.set_ch_ac();
            break;
        case 9:
            relay.mode(Relay::Mode9NDV);
            adc.set_ch_dc();
            break;
        case 10:
            relay.mode(Relay::Mode10NADV);
            adc.set_ch_dc();
            break;
        case 11:
            relay.mode(Relay::Mode11NBDV);
            adc.set_ch_dc();
            break;
        case 12:
            relay.mode(Relay::Mode12NCuZnDV);
            adc.set_ch_dc();
            break;
    }
}


float analog_read_direct(Relay::Mode_t ch)
{
    if(!relay.isValidSampleCh(ch))return 0;
    int index = relay.mode_bitmap((Relay::Mode_t)ch)&0X03;
    return analog_get_cal(index);
}


float analog_get(Relay::Mode_t ch,uint16_t timeout)
{
    if(!relay.isValidSampleCh(ch))return 0;
    uint32_t last = millis();
    set_relay_ch(ch);
//    UART.printf("analog_get:timeout%d\n",timeout);
    while(1)
    {
        if(millis() - last >= timeout)
        {
            return analog_read_direct(ch);
        }
        delay_ms(50);
    }
    return 0;
}







uint16_t vol_to_10mv(float voltage)
{
    voltage *=100; 
    uint16_t mv10 = round(voltage);
    return mv10;
}
uint16_t vol_to_mv(float voltage)
{
    voltage *=1000; 
    uint16_t mv = round(voltage);
    return mv;
}
uint16_t vol_to_10ua(float voltage)
{
    voltage *=100; 
    uint16_t ua10 = round(voltage);
    return ua10;
}
uint16_t vol_to_ua(float voltage)
{
    voltage *=1000; 
    uint16_t ua = round(voltage);
    return ua;
}


uint16_t get_10mv(Relay::Mode_t mode)
{
    int index = relay.mode_bitmap(mode)&0X03;
    float value = analog_get_cal(index);
    uint16_t mv10 = vol_to_10mv(value);
//    UART.printf("mode:%d\t index:%d\t value:%0.3f, 10mv:%d\n",mode,index,value,mv10);
    return mv10;
}

uint16_t get_mv(Relay::Mode_t mode)
{
    int index = relay.mode_bitmap(mode)&0X03;
    float value = analog_get_cal(index);
    return vol_to_mv(value);
}
uint16_t get_10ua(Relay::Mode_t mode)
{
    int index = relay.mode_bitmap(mode)&0X03;
    float value = analog_get_cal(index);
    uint16_t ua10 = vol_to_10ua(value);
//    UART.printf("mode:%d\t index:%d\t value:%0.3f, 10mv:%d\n",mode,index,value,mv10);
    return ua10;
}

uint16_t get_ua(Relay::Mode_t mode)
{
    int index = relay.mode_bitmap(mode)&0X03;
    float value = analog_get_cal(index);
    uint16_t ua = vol_to_ua(value);
//    UART.printf("mode:%d\t index:%d\t value:%0.3f, 10mv:%d\n",mode,index,value,mv10);
    return ua;
}
