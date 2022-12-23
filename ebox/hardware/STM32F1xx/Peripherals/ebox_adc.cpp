#include "ebox_adc.h"
#include "rcc.h"
#include "nvic.h"
void Adc::add_temp_senser()
{
    ch_table[0][ch_num] = 0xff;//ADC_Channel_16
    ch_table[1][ch_num] = ADC_CHANNEL_TEMPSENSOR;//ADC_Channel_16
    ch_num++;
}

void  Adc::add_ch(Gpio *io)
{
    io->mode(AIN);
    ch_table[0][ch_num] = (uint8_t)io->id;

    switch((uint32_t)io->id)
    {
    case PA0_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_0;
        break;
    case PA1_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_1;
        break;
    case PA2_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_2;
        break;
    case PA3_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_3;
        break;
    case PA4_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_4;
        break;
    case PA5_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_5;
        break;
    case PA6_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_6;
        break;
    case PA7_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_7;
        break;

    case PB0_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_8;
        break;
    case PB1_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_9;
        break;

    case PC0_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_10;
        break;
    case PC1_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_11;
        break;
    case PC2_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_12;
        break;
    case PC3_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_13;
        break;
    case PC4_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_14;
        break;
    case PC5_ID:
        ch_table[1][ch_num] = ADC_CHANNEL_15;
        break;

    }
    ch_num++;
}
void  Adc::begin()
{
    //RCC_AHBPeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    rcc_clock_cmd((uint32_t)hadc.Instance, ENABLE);

    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK){}
        
        
    switch((uint32_t)hadc.Instance)
    {
    case ADC1_BASE:
        hdma.Instance = DMA1_Channel1;
        break;
    case ADC2_BASE:
        hdma.Instance = DMA1_Channel1;
        break;
    }
    
    

    rcc_clock_cmd((uint32_t)hdma.Instance,ENABLE);
    nvic_dev_disable((uint32_t)hdma.Instance,0);
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

    

    ADC_InitTypeDef  ADC_InitStructure;
    ADC_ChannelConfTypeDef sConfig = {0};

    HAL_ADC_DeInit(&hadc);//将外设 ADC1 的全部寄存器重设为缺省值

    hadc.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc.Init.ContinuousConvMode = ENABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.NbrOfConversion = ch_num;
    if (HAL_ADC_Init(&hadc) != HAL_OK)
    {
    }
    

  /* ADC1 regular channel11 configuration */
    //设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
    //ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期

    for(int i = 0; i < ch_num; i++)
    {
        if(ch_table[1][i] == ADC_CHANNEL_TEMPSENSOR)
        {
         
//            ADC_TempSensorVrefintCmd(ENABLE); //开启内部温度传感器
          sConfig.Channel = ch_table[1][i];
          sConfig.Rank = i + 1;
          sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
          HAL_ADC_ConfigChannel(&hadc, &sConfig);
        }
        else
        {
          sConfig.Channel = ch_table[1][i];
          sConfig.Rank = i + 1;
          sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
          HAL_ADC_ConfigChannel(&hadc, &sConfig);
        }
    }


//    // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
//    ADC_DMACmd(ADCx, ENABLE);

//    /* Enable ADC1 */
//    ADC_Cmd(ADCx, ENABLE);	   //使能指定的ADC1
//    /* Enable ADC1 reset calibaration register */
//    ADC_ResetCalibration(ADCx);	  //复位指定的ADC1的校准寄存器
//    /* Check the end of ADC1 reset calibration register */
//    while(ADC_GetResetCalibrationStatus(ADCx));	//获取ADC1复位校准寄存器的状态,设置状态则等待

//    /* Start ADC1 calibaration */
//    ADC_StartCalibration(ADCx);		//开始指定ADC1的校准状态
//    /* Check the end of ADC1 calibration */
//    while(ADC_GetCalibrationStatus(ADCx));		//获取指定ADC1的校准程序,设置状态则等待



    HAL_ADC_Start_DMA(&hadc,(uint32_t *)adc_buf,ch_num);
//    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}
uint16_t Adc::read(Gpio *io)
{
    for(int i = 0; i < ch_num; i++)
    {
        if(io->id == ch_table[0][i])
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
    for(int i = 0; i < ch_num; i++)
    {
        if(io->id == ch_table[0][i])
            return (adc_buf[i] * 3287.0 / 4096);
    }
    return 0;
}
float Adc::read_voltage(int index)
{
    return (adc_buf[index] * 3287.0 / 4096);
}
float Adc::read_temp_senser()
{
    float temperate;
    uint16_t adc_value = 0;
    for(int i = 0; i < ch_num; i++)
    {
        if(0XFF == ch_table[0][i])
        {
            adc_value = adc_buf[i];
            break;
        }
    }

    temperate = (float)adc_value * (3300 / 4096.0); //电压值
    temperate = (1430 - temperate) / 4.35 + 24.958; //转换为温度值
    return temperate;
}
