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
    uint8_t         ch_num;//���ͨ����������������
    uint8_t         ch_table[2][18];//[0]:����IO������[1]������IO��Ӧadcͨ������
    uint16_t        adc_buf[18];//DMA���ϵİ������˳��adc����ͨ��ֵ��������������
    ADC_HandleTypeDef hadc;
    DMA_HandleTypeDef   hdma;


};
#endif
