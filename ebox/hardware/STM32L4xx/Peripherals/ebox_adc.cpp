#include "ebox_adc.h"
#include "rcc.h"
#include "nvic.h"
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef   hdma_adc1;

#define ADC_VREF 2500.0
uint32_t rank[20] = {
ADC_REGULAR_RANK_1	,
ADC_REGULAR_RANK_2	,
ADC_REGULAR_RANK_3	,
ADC_REGULAR_RANK_4	,
ADC_REGULAR_RANK_5	,
ADC_REGULAR_RANK_6	,
ADC_REGULAR_RANK_7	,
ADC_REGULAR_RANK_8	,
ADC_REGULAR_RANK_9	,
ADC_REGULAR_RANK_10	,
ADC_REGULAR_RANK_11	,
ADC_REGULAR_RANK_12	,
ADC_REGULAR_RANK_13	,
ADC_REGULAR_RANK_14	,
ADC_REGULAR_RANK_15	,
ADC_REGULAR_RANK_16	
};

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
    case PC0_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_1;
        break;
    case PC1_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_2;
        break;
    case PC2_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_3;
        break;
    case PC3_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_4;
        break;
    case PA0_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_5;
        break;
    case PA1_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_6;
        break;
    case PA2_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_7;
        break;
    case PA3_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_8;
        break;
    case PA4_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_9;
        break;
    case PA5_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_10;
        break;
    case PA6_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_11;
        break;
    case PA7_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_12;
        break;
    case PC5_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_13;
        break;
    case PC6_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_14;
        break;

    case PB0_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_15;
        break;
    case PB1_ID:
        adc_ch[ch_num].ch = ADC_CHANNEL_16;
        break;


    }
    ch_num++;
}
void  Adc::begin()
{

    
    switch((uint32_t)hadc1.Instance)
    {
    case ADC1_BASE:
        hdma_adc1.Instance = DMA1_Channel1;
        break;
    #if defined(ADC2)
    case ADC2_BASE:
        hadc.Instance = ADC2;
        hdma.Instance = DMA2_Stream2;
        break;
    #endif
    }
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
  
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the peripherals clock
    */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
    PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
    PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV8;
    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV8;
    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    
    
    __HAL_RCC_ADC_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Request = DMA_REQUEST_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);
        
  

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.NbrOfConversion = ch_num;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
    hadc1.Init.OversamplingMode = DISABLE;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    ADC_ChannelConfTypeDef sConfig = {0};


  /** Configure Regular Channel
  */
  sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  for(int i = 0; i < ch_num; i++)
  {
      sConfig.Channel = adc_ch[i].ch;
      sConfig.Rank = rank[i];
      if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      {
        Error_Handler();
      }
  }


  	HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);//??ADC 

    HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adc_buf,ch_num);
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
float Adc::read_temp()
{
    float temperate;
    uint16_t adc_value = 0;
    uint16_t TS_CAL1 = *(TEMPSENSOR_CAL1_ADDR);
	uint16_t TS_CAL2 = *(TEMPSENSOR_CAL2_ADDR);
    adc_value = adc_buf[temp_index];
    float voltage = (float)adc_value * ADC_VREF / (float)TEMPSENSOR_CAL_VREFANALOG ; //µÁ—π÷µmv
    temperate = ((float)(TEMPSENSOR_CAL2_TEMP - TEMPSENSOR_CAL1_TEMP) / (TS_CAL2 - TS_CAL1)) * (voltage - TS_CAL1) + 30.0f;
    return temperate;
}
float Adc::read_vbat()
{
    float voltage;
    uint16_t adc_value = 0;
    adc_value = adc_buf[vbat_index];
    voltage = (float)adc_value * ADC_VREF / 4096.0; //
    voltage *= 3; //Ratio on VBAT measurement
    return voltage;
}
float Adc::read_vref()
{
    float voltage;
    uint16_t adc_value = 0;
    adc_value = adc_buf[vref_index];
    //VREF_CAL1:1655
    //VREF:1.224v
    uint16_t VREF_CAL1 = *(VREFINT_CAL_ADDR);
    float VrefInt =  VREF_CAL1 * VREFINT_CAL_VREF / 4096;
    
//    rt_kprintf("VrefInt:%0.3f\n",VrefInt);
//    rt_kprintf("adc_value:%d\n",adc_value);
    voltage = VrefInt*4096/adc_value ; //calculate vref
    return voltage;
}
extern "C"
{

void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */
        rt_interrupt_enter();

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
    rt_interrupt_leave();
  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

}

