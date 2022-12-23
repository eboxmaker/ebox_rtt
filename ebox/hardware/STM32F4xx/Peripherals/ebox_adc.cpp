#include "ebox_adc.h"


#define STM32
#define GD32
#define ADC_VREF 3300.0

typedef struct 
{
    uint16_t pin;
    uint32_t ch;
}AdcChannel_t;

AdcChannel_t adc_ch[16];


void Adc::add_temp()
{
    adc_ch[ch_num].pin = 0x0101;
    adc_ch[ch_num].ch = ADC_CHANNEL_TEMPSENSOR;
    temp_index = ch_num;
    ch_num++;
}
void Adc::add_vbat()
{
    adc_ch[ch_num].pin = 0x0102;
    adc_ch[ch_num].ch = ADC_CHANNEL_VBAT;
    vbat_index = ch_num;
    ch_num++;
}
void Adc::add_vref()
{
    adc_ch[ch_num].pin = 0x0103;
    adc_ch[ch_num].ch = ADC_CHANNEL_VREFINT;
    vref_index = ch_num;
    ch_num++;
}

void  Adc::add_ch(Gpio *io)
{
    io->mode(AIN);
    adc_ch[ch_num].pin = (uint8_t)io->id;

    switch((uint32_t)io->id)
    {
    case PA0_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_0;
        break;
    case PA1_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_1;
        break;
    case PA2_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_2;
        break;
    case PA3_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_3;
        break;
    case PA4_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_4;
        break;
    case PA5_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_5;
        break;
    case PA6_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_6;
        break;
    case PA7_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_7;
        break;

    case PB0_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_8;
        break;
    case PB1_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_9;
        break;

    case PC0_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_10;
        break;
    case PC1_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_11;
        break;
    case PC2_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_12;
        break;
    case PC3_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_13;
        break;
    case PC4_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_14;
        break;
    case PC5_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_15;
        break;

    }
    ch_num++;
}
void  Adc::begin()
{
    ADC_ChannelConfTypeDef sConfig = {0};
  
        
    switch((uint32_t)hadc.Instance)
    {
    case ADC1_BASE:
        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_DMA2_CLK_ENABLE();
        hadc.Instance = ADC1;
        hdma.Instance = DMA2_Stream0;
        break;
    case ADC2_BASE:
        __HAL_RCC_ADC2_CLK_ENABLE();
        __HAL_RCC_DMA2_CLK_ENABLE();
        hadc.Instance = ADC2;
        hdma.Instance = DMA2_Stream2;
        break;
    }
    
    
/* ADC1 DMA Init */
//    nvic_dev_disable((uint32_t)hdma.Instance,0);
    
    hdma.Init.Channel = DMA_CHANNEL_0;
    hdma.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma.Init.MemInc = DMA_MINC_ENABLE;
    hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma.Init.Mode = DMA_CIRCULAR;
    hdma.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma) != HAL_OK)
    {
    }
//    HAL_DMA_Start(&hdma,(uint32_t)&hadc.Instance->DR,(uint32_t)&adc_buf,ch_num);
    __HAL_LINKDMA(&hadc,DMA_Handle,hdma);

    


    HAL_ADC_DeInit(&hadc);//将外设 ADC1 的全部寄存器重设为缺省值

    
    
    hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
    hadc.Init.Resolution = ADC_RESOLUTION_12B;
    hadc.Init.ScanConvMode = ENABLE;
    hadc.Init.ContinuousConvMode = ENABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.NbrOfConversion = ch_num;
    hadc.Init.DMAContinuousRequests = ENABLE;
    hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    
    if (HAL_ADC_Init(&hadc) != HAL_OK)
    {
    }
    

  /* ADC1 regular channel11 configuration */
    //设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
    //ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期

    for(int i = 0; i < ch_num; i++)
    {
        sConfig.Channel = adc_ch[i].ch;
        sConfig.Rank = i + 1;
        sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
        HAL_ADC_ConfigChannel(&hadc, &sConfig);
    }

    HAL_ADC_Start_DMA(&hadc,(uint32_t *)adc_buf,ch_num);
}
uint16_t Adc::read(Gpio *io)
{
    for(int i = 0; i < ch_num; i++)
    {
        if(io->id == adc_ch[i].pin)
            return adc_buf[i];
    }
    return 0;
}
uint16_t Adc::read(int index)
{
    return adc_buf[index];
}

float Adc::read_voltage(Gpio *io)
{
    return (read(io) * ADC_VREF / 4096.0);
}
float Adc::read_voltage(int index)
{
    return (adc_buf[index] * ADC_VREF / 4096);
}
#include "ebox.h"
float Adc::read_temp()
{
#if defined(GD32)
    float temperate;
    uint16_t adc_value = 0;
   
    adc_value = adc_buf[temp_index];
    float voltage = (float)adc_value * ADC_VREF/4096.0; //电压值mv
    temperate = (1450 - voltage)/4.1 + 25.0f;//GD32      
    return temperate;
#elif defined(STM32)
    float temperate;
    uint16_t adc_value = 0;
    uint16_t TS_CAL1 = *(TEMPSENSOR_CAL1_ADDR);
	uint16_t TS_CAL2 = *(TEMPSENSOR_CAL2_ADDR);
    adc_value = adc_buf[temp_index];
    float voltage = (float)adc_value * ADC_VREF / TEMPSENSOR_CAL_VREFANALOG ; //电压值mv
    temperate = ((float)(TEMPSENSOR_CAL2_TEMP - TEMPSENSOR_CAL1_TEMP) / (TS_CAL2 - TS_CAL1)) * (voltage - TS_CAL1) + 30.0f;
    return temperate;
#endif    

    
}
float Adc::read_vbat()
{
    float voltage;
    uint16_t adc_value = 0;
    adc_value = adc_buf[vbat_index];
    voltage = (float)adc_value * ADC_VREF / 4096.0; //
#if defined(GD32)
    voltage *= 4; //Ratio on VBAT measurement=2
#elif defined(STM32)
    voltage *= 2; //Ratio on VBAT measurement=2
#endif    
    return voltage;
}
float Adc::read_vref()
{
    float voltage = 0;
#if defined(GD32)
    return voltage;
#elif defined(STM32)
    uint16_t adc_value = 0;
    adc_value = adc_buf[vref_index];
    //VREF_CAL1:1655
    //VREF:1.224v
    uint16_t VrefCal = *(VREFINT_CAL_ADDR);
    float VrefInt = VREFINT_CAL_VREF * VrefCal / 4096.0;
    
//    UART.printf("VREF_CAL1:%d\n",VrefCal1);
//    UART.printf("VrefInt:%0.3f\n",VrefInt);
//    UART.printf("adc_value:%d\n",adc_value);
    voltage = VrefInt*4096.0/adc_value ; //calculate vref
    return voltage;
 #endif    



}
