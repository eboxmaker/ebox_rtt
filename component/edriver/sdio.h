/**
  ******************************************************************************
  * @file    w25xxx.h
  * @author  shentq
  * @version V2.2
  * @date    2019/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SDIO_H
#define _SDIO_H
#include "stm32_hal.h"
#include "ebox_core.h"
#include "interface/storage/blockdevice/BlockDevice.h"

using namespace ebox;
#define   MSD_OK                        ((uint8_t)0x00)
#define   MSD_ERROR                     ((uint8_t)0x01)
#define   MSD_ERROR_SD_NOT_PRESENT      ((uint8_t)0x02)

/**
  * @brief  SD transfer state definition
  */
#define   SD_TRANSFER_OK                ((uint8_t)0x00)
#define   SD_TRANSFER_BUSY              ((uint8_t)0x01)
#define   SD_TRANSFER_ERROR             ((uint8_t)0x02)

#define SD_DATATIMEOUT           ((uint32_t)100000000)

#define SD_PRESENT               ((uint8_t)0x01)
#define SD_NOT_PRESENT           ((uint8_t)0x00)
    
class SDIO : public ebox::BlockDevice
{

public:
    SDIO(SDMMC_TypeDef *_sd);
    virtual int read_sector( uint8_t *buffer, uint32_t sector,uint8_t count);
    virtual int write_sector(const uint8_t *buffer,uint32_t sector,   uint8_t count);
    
    
    virtual int init();
    virtual int deinit();

    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size);
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size);
    virtual int erase(bd_addr_t addr, bd_size_t size);
    
    virtual bd_size_t get_read_size() const;
    virtual bd_size_t get_program_size() const;
    virtual bd_size_t size() const ;
    virtual const char *get_type() const;
//    
//    
//    int         get_CID(uint8_t *cid_data);
//    int         get_CSD(uint8_t *csd_data);        

private:
    SDMMC_TypeDef *sd;
    void HAL_SD_MspInit(void);
    void MX_SDMMC1_SD_Init(void);



    uint8_t BSP_SD_Init(void);
    uint8_t BSP_SD_ITConfig(void);
    void    BSP_SD_DetectIT(void);
    void    BSP_SD_DetectCallback(void);
    uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
    uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);
    uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
    uint8_t BSP_SD_GetCardState(void);
    void    BSP_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo);
    uint8_t BSP_SD_IsDetected(void);

        
    int _initialise_card();
    ebox::bd_size_t _sectors;
    ebox::bd_size_t _sd_sectors();
    
    bool _is_valid_trim(ebox::bd_addr_t addr, ebox::bd_size_t size);

    ebox::bd_size_t  _get_capacity(void);


private:
    static const uint32_t _block_size;
    uint32_t _erase_size;
    bool _is_initialized;
    bool _dbg;
    uint32_t _init_ref_count;

#if MBED_CONF_SD_CRC_ENABLED
    bool _crc_on;
#endif
    uint8_t index;


};


#endif

