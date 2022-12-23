#include "sdio.h"

// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_SDIO_ENABLE       true


#if EBOX_DEBUG_SDIO_ENABLE
#define sdioDebug(...)  rt_kprintf("[sdio]:%d: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define sdioDebug(...)
#endif



SD_HandleTypeDef hsd[1];

#if defined(SDMMC_DATATIMEOUT)
#define SD_TIMEOUT 1000
#elif defined(SD_DATATIMEOUT)
#define SD_TIMEOUT SD_DATATIMEOUT
#else
#define SD_TIMEOUT 30 * 1000
#endif

#define SD_DEFAULT_BLOCK_SIZE 512
const uint32_t SDIO::_block_size = SD_DEFAULT_BLOCK_SIZE;

#define SD_DELAY 30
SDIO::SDIO(SDMMC_TypeDef *_sd)
{
    sd = _sd;
    if(_sd==SDMMC1)
    {
        index = 0;
        hsd[index].Instance = SDMMC1;
    }
    index = 0;

}

void SDIO::MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd[index].Instance = sd;
  hsd[index].Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd[index].Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd[index].Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd[index].Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd[index].Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd[index].Init.ClockDiv = 8;
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}

int SDIO::init()
{
    MX_SDMMC1_SD_Init();
//    sdioDebug("BSP_SD_Init...\n");
    uint8_t ret = BSP_SD_Init() ;
    if(ret == MSD_OK)
    {
//        sdioDebug("SDIO::init end\n");
        return 0;
//        Stat = SD_CheckStatus(lun);
    }
    else
    {
        sdioDebug("init:%d\n",ret);
    }
    return -1;
}
int  SDIO::deinit()
{
    hsd[index].State = HAL_SD_STATE_RESET;
//    _is_initialized = false;
    if(hsd->Instance==SDMMC1)
    {
        __HAL_RCC_SDMMC1_CLK_DISABLE();

        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12);

        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
        HAL_NVIC_DisableIRQ(SDMMC1_IRQn);
    }
    return 0;
}
int SDIO::read(void *buffer, bd_addr_t addr, bd_size_t size)
{
    int res;
    size = size /  _block_size;
    addr = addr / _block_size;
    if(BSP_SD_ReadBlocks((uint32_t*)buffer,
                       (uint32_t) (addr),
                       size, SD_TIMEOUT) == MSD_OK)
    {
        /* wait until the read operation is finished */
        while(BSP_SD_GetCardState()!= MSD_OK)
        {
        }
        res = 0;
    }

    return res;
}
int SDIO::program(const void *buffer, bd_addr_t addr, bd_size_t size)
{
  int res = -1;
    size = size /  hsd[index].SdCard.BlockSize;
    addr = addr / _block_size;

  if(BSP_SD_WriteBlocks((uint32_t*)buffer,
                        (uint32_t)(addr),
                        size, SD_TIMEOUT) == MSD_OK)
  {
	/* wait until the Write operation is finished */
    while(BSP_SD_GetCardState() != MSD_OK)
    {
    }
    res = 0;
  }

  return res;

}
int SDIO::erase(bd_addr_t addr, bd_size_t size)
{
    uint32_t EndAddr = addr + size;
    if(BSP_SD_Erase(addr,EndAddr) == MSD_OK)
    {
        while(BSP_SD_GetCardState() != MSD_OK)
        {
        }
    }
    return 0;
}

bd_size_t SDIO::get_read_size() const
{
    return _block_size;
}
bd_size_t SDIO::get_program_size() const
{
    return _block_size;
}
bd_size_t SDIO::size() const
{
    return _block_size * _sectors;
}
const char *SDIO::get_type() const
{
//    switch(SD_Type)
//    {
//        case 0:
//            return "SD_TYPE_MMC";
//        case 1:
//            return "SD_TYPE_V1";
//        case 2:
//            return "SD_TYPE_V2";
//        case 3:
//            return "UNKNOW(3)";
//        case 4:
//            return "SD_TYPE_V2HC";
//        default:
//            return "UNKNOW";
//    }
    return "UNKNOW";
}
 


int SDIO::read_sector( uint8_t *buffer, uint32_t sector,uint8_t count)
{
    uint8_t sd_state = MSD_OK;

  if (HAL_SD_ReadBlocks(&hsd[0], buffer, sector, count, SD_TIMEOUT) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  return sd_state;
    return 0;
}
int SDIO::write_sector(const uint8_t *buffer,uint32_t sector,   uint8_t count)
{
    return 0;
}

uint8_t SDIO::BSP_SD_Init(void)
{
  uint8_t sd_state = MSD_OK;
  /* Check if the SD card is plugged in the slot */
  if (BSP_SD_IsDetected() != SD_PRESENT)
  {
    sdioDebug("BSP_SD_IsDetected : MSD_ERROR_SD_NOT_PRESENT\n");
    return MSD_ERROR_SD_NOT_PRESENT;
  }

//    sdioDebug("HAL_SD_Init...\n");
  /* HAL SD initialization */
  sd_state = HAL_SD_Init(&hsd[index]);
  /* Configure SD Bus width (4 bits mode selected) */
//    sdioDebug("HAL_SD_ConfigWideBusOperation...\n");
//  if (sd_state == MSD_OK)
//  {
//    /* Enable wide operation */
//    HAL_StatusTypeDef res = HAL_SD_ConfigWideBusOperation(&hsd[index], SDMMC_BUS_WIDE_4B);
//    if (res != HAL_OK)
//    {
//        sd_state = MSD_ERROR;
//    }
//    sdioDebug("HAL_SD_ConfigWideBusOperation:%d\n",res);

//  }
//  else
//  {
//    sdioDebug("init:sd_state = %d\n",sd_state);
//  }

  return sd_state;
}

uint8_t SDIO::BSP_SD_ITConfig(void)
{
  /* Code to be updated by the user or replaced by one from the FW pack (in a stmxxxx_sd.c file) */

  return (uint8_t)0;
}
uint8_t SDIO::BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
  uint8_t sd_state = MSD_OK;

  if (HAL_SD_ReadBlocks(&hsd[index], (uint8_t *)pData, ReadAddr, NumOfBlocks, Timeout) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  return sd_state;
}
uint8_t SDIO::BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
  uint8_t sd_state = MSD_OK;

  if (HAL_SD_WriteBlocks(&hsd[index], (uint8_t *)pData, WriteAddr, NumOfBlocks, Timeout) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  return sd_state;
}


uint8_t SDIO::BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
  uint8_t sd_state = MSD_OK;

  if (HAL_SD_Erase(&hsd[0], StartAddr, EndAddr) != HAL_OK)
  {
    sd_state = MSD_ERROR;
  }

  return sd_state;
}
uint8_t SDIO::BSP_SD_GetCardState(void)
{
  return ((HAL_SD_GetCardState(&hsd[0]) == HAL_SD_CARD_TRANSFER ) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}

void SDIO::BSP_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo)
{
  /* Get SD card Information */
  HAL_SD_GetCardInfo(&hsd[0], CardInfo);
}
uint8_t SDIO::BSP_SD_IsDetected(void)
{
  __IO uint8_t status = SD_PRESENT;

  /* USER CODE BEGIN 1 */
  /* user code can be inserted here */
  /* USER CODE END 1 */

  return status;
}

extern "C" {

/**
  * @brief This function handles SDMMC1 global interrupt.
  */
void SDMMC1_IRQHandler(void)
{
  /* USER CODE BEGIN SDMMC1_IRQn 0 */

  /* USER CODE END SDMMC1_IRQn 0 */
  HAL_SD_IRQHandler(&hsd[0]);
  /* USER CODE BEGIN SDMMC1_IRQn 1 */

  /* USER CODE END SDMMC1_IRQn 1 */
}

void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(hsd->Instance==SDMMC1)
  {
  /* USER CODE BEGIN SDMMC1_MspInit 0 */

  /* USER CODE END SDMMC1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_SDMMC1;
    PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
    PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
    PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV4;
    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_SDMMC1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**SDMMC1 GPIO Configuration
    PC8     ------> SDMMC1_D0
    PC9     ------> SDMMC1_D1
    PC10     ------> SDMMC1_D2
    PC11     ------> SDMMC1_D3
    PC12     ------> SDMMC1_CK
    PD2     ------> SDMMC1_CMD
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* SDMMC1 interrupt Init */
//    HAL_NVIC_SetPriority(SDMMC1_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
  /* USER CODE BEGIN SDMMC1_MspInit 1 */
//    sdioDebug("[ok] HAL_SD_MspInit\n");
  /* USER CODE END SDMMC1_MspInit 1 */
  }
  else
  {
    sdioDebug("[err] hsd->Instance= 0x%08x   SDMMC1= 0x%08x\n",hsd->Instance,SDMMC1);

  }
}

}
