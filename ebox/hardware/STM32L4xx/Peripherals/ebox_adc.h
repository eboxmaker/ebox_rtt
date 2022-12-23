#ifndef __EBOX_ADC_H
#define __EBOX_ADC_H
#include "stm32_hal.h"
#include "ebox_core.h"
class Adc
{
public:
    Adc(ADC_TypeDef *ADCx)
    {
        ch_num = 0;
    }

    void add_temp();
    void add_vbat();
    void add_vref();
    void add_ch(Gpio *io);
    void begin();


    uint16_t read(Gpio *io);
    uint16_t read(int index);

    float read_voltage(Gpio *io);
    float read_voltage(int index);
    float read_temp();
    float read_vbat();
    float read_vref();

private:
    uint8_t         ch_num;//添加通道的数量自增变量
    uint16_t        adc_buf[18];//DMA不断的按照添加顺序将adc各个通道值。传输至此数组

    uint8_t temp_index;
    uint8_t vbat_index;
    uint8_t vref_index;

};
#endif
