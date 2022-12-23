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

    HAL_ADC_DeInit(&hadc);//������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

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
    //����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
    //ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����

    for(int i = 0; i < ch_num; i++)
    {
        if(ch_table[1][i] == ADC_CHANNEL_TEMPSENSOR)
        {
         
//            ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��¶ȴ�����
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


//    // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
//    ADC_DMACmd(ADCx, ENABLE);

//    /* Enable ADC1 */
//    ADC_Cmd(ADCx, ENABLE);	   //ʹ��ָ����ADC1
//    /* Enable ADC1 reset calibaration register */
//    ADC_ResetCalibration(ADCx);	  //��λָ����ADC1��У׼�Ĵ���
//    /* Check the end of ADC1 reset calibration register */
//    while(ADC_GetResetCalibrationStatus(ADCx));	//��ȡADC1��λУ׼�Ĵ�����״̬,����״̬��ȴ�

//    /* Start ADC1 calibaration */
//    ADC_StartCalibration(ADCx);		//��ʼָ��ADC1��У׼״̬
//    /* Check the end of ADC1 calibration */
//    while(ADC_GetCalibrationStatus(ADCx));		//��ȡָ��ADC1��У׼����,����״̬��ȴ�



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

    temperate = (float)adc_value * (3300 / 4096.0); //��ѹֵ
    temperate = (1430 - temperate) / 4.35 + 24.958; //ת��Ϊ�¶�ֵ
    return temperate;
}
