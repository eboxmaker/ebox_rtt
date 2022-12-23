#ifndef __EBOX_ADC_H
#define __EBOX_ADC_H
#include "ebox_core.h"
#include "stm32f1xx_hal.h"
class Adc
{
public:
    Adc(ADC_TypeDef *ADCx)
    {
        hadc.Instance = ADCx;

    }

    void add_temp_senser();
    void add_ch(Gpio *io);
    void begin();


    uint16_t read(Gpio *io);
    uint16_t read(int index);

    float read_voltage(Gpio *io);
    float read_voltage(int index);

    float read_temp_senser();

private:
    uint8_t         ch_num;//添加通道的数量自增变量
    uint8_t         ch_table[2][18];//[0]:保存IO索引，[1]：保存IO对应adc通道索引
    uint16_t        adc_buf[18];//DMA不断的按照添加顺序将adc各个通道值。传输至此数组
    ADC_HandleTypeDef hadc;
    DMA_HandleTypeDef   hdma;


};
#endif
