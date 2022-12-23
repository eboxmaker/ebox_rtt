/**
  ******************************************************************************
  * @file    _spi.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
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


/* Includes ------------------------------------------------------------------*/
#include "ebox_mcu_spi.h"
#include "rcc.h"

#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define EBOX_DEBUG_MCUSPI_ENABLE       true
#define EBOX_DEBUG_MCUSPI_ENABLE_ERR   true
#endif


#if EBOX_DEBUG_MCUSPI_ENABLE
#define mcuSpiDebug(...)  ebox_printf("[mcuSPI DBG]:%d: ",__LINE__),ebox_printf(__VA_ARGS__ )
#else
#define mcuSpiDebug(...)
#endif

#if EBOX_DEBUG_MCUSPI_ENABLE_ERR
#define mcuSpiDebugErr(fmt, ...)  ebox_printf("[mcuSPI err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define mcuSpiDebugErr(fmt, ...)
#endif

mcuSpi::mcuSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
{
    _busy = 0;
    _spi = SPIx;
    _sck = sck;
    _miso = miso;
    _mosi = mosi;
    hspi.Instance = SPIx;
}

void mcuSpi::begin(Config_t *newConfig)
{
    _sck->mode(AF_PP);
    _miso->mode(AF_PP);
    _mosi->mode(AF_PP);

    rcc_clock_cmd((uint32_t)_spi, ENABLE);
    config(newConfig);
}
void mcuSpi::config(Config_t *newConfig)
{
    SPI_InitTypeDef SPI_InitStructure;

    current_dev_num = newConfig->dev_num;


    
    HAL_SPI_DeInit(&hspi);
    
    
    hspi.Init.Direction = SPI_DIRECTION_2LINES;//ȫ˫��
    hspi.Init.DataSize = SPI_DATASIZE_8BIT;//8λ����ģʽ
    hspi.Init.NSS  = SPI_NSS_SOFT;//NSS�������
    hspi.Init.CRCPolynomial = 7;//CRC����ʽ
    hspi.Init.Mode = SPI_MODE_MASTER;//����ģʽ

    if(newConfig->mode == MODE0)
    {
        hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
        hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
    }
    else if(newConfig->mode == MODE1)
    {
        hspi.Init.CLKPhase = SPI_PHASE_2EDGE;
        hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
    }
    else if(newConfig->mode == MODE2)
    {

        hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
        hspi.Init.CLKPolarity = SPI_POLARITY_HIGH;
    }
    else if(newConfig->mode == MODE3)
    {
        hspi.Init.CLKPhase = SPI_PHASE_2EDGE;
        hspi.Init.CLKPolarity = SPI_POLARITY_HIGH;
    }
    switch(newConfig->prescaler)
    {
    case DIV2:
        hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
        break;
    case DIV4:
        hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
        break;
    case DIV8:
        hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
        break;
    case DIV16:
        hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
        break;
    case DIV32:
        hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
        break;
    case DIV64:
        hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
        break;
    case DIV128:
        hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
        break;
    case DIV256:
        hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
        break;
    default :
        hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
        break;

    }

    switch(newConfig->bit_order)
    {
    case MSB:
        hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
        break;
    case LSB:
        hspi.Init.FirstBit = SPI_FIRSTBIT_LSB;
        break;
    default :
        hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
        break;
    }
    HAL_SPI_Init(&hspi);
    __HAL_SPI_ENABLE(&hspi);

}
/**
  *@brief    ��ȡ������Ϣ
  *@param    void
  *@retval   ���ص�ǰ�豸id
  */
uint8_t mcuSpi::read_config(void)
{
    return current_dev_num;
}

/**
  *@brief    transfer
  *@param    data Ҫ���͵�����
  *@retval   uint8_t ���ܵ�������
  */
uint8_t mcuSpi::transfer(uint8_t data)
{
//    mcuSpiDebug("\n===========err\n");
    ;
    while (__HAL_SPI_GET_FLAG(&hspi,SPI_FLAG_TXE) == RESET)
        ;
    _spi->DR = data;
    while (__HAL_SPI_GET_FLAG(&hspi,SPI_FLAG_RXNE) == RESET)
        ;
    return _spi->DR;
}
/**
  *@brief    д����
  *@param    data Ҫд�������
  *@retval   none
  */
int8_t mcuSpi::write(uint8_t data)
{
    __IO uint8_t dummyByte;
    while (__HAL_SPI_GET_FLAG(&hspi,SPI_FLAG_TXE) == RESET)
        ;
    _spi->DR = data;
    while (__HAL_SPI_GET_FLAG(&hspi,SPI_FLAG_RXNE) == RESET)
        ;
    dummyByte = _spi->DR;

    return 0;
}
/**
  *@brief    ����д����
  *@param    data Ҫд������ݣ�data_length ����
  *@retval   none
  */
int8_t mcuSpi::write_buf(uint8_t *data, uint16_t len)
{
    __IO uint8_t dummyByte;
    if(len == 0)
        return -1;
    while(len--)
    {
        while (__HAL_SPI_GET_FLAG(&hspi,SPI_FLAG_TXE) == RESET)
            ;
        _spi->DR = *data++;
        while (__HAL_SPI_GET_FLAG(&hspi,SPI_FLAG_RXNE) == RESET)
            ;
        dummyByte = _spi->DR;
    }
    return 0;
}
uint8_t mcuSpi::read()
{
    while (__HAL_SPI_GET_FLAG(&hspi,SPI_FLAG_TXE) == RESET)
        ;
    _spi->DR = 0xff;
    while (__HAL_SPI_GET_FLAG(&hspi,SPI_FLAG_RXNE) == RESET)
        ;
    return(_spi->DR);

}


/**
  *@brief    ������ȡ����
  *@param    recv_data ��ȡ�������ݣ�data_length Ҫ��ȡ�����ݳ���
  *@retval   ���ص�ǰ�豸id
  */
int8_t mcuSpi::read_buf(uint8_t *recv_data, uint16_t len)
{
    if(len == 0)
        return -1;
    while(len--)
    {
        while (__HAL_SPI_GET_FLAG(&hspi,SPI_FLAG_TXE) == RESET)
            ;
        _spi->DR = 0xff;
        while (__HAL_SPI_GET_FLAG(&hspi,SPI_FLAG_RXNE) == RESET)
            ;
        *recv_data++ = _spi->DR;
    }
    return 0;
}


/**
  *@brief    ��ȡ����Ȩ
  *@param    none
  *@retval   none
  */
int8_t mcuSpi::take(Config_t *newConfig)
{
    int i = 0;

    while (_busy == 1)
    {
        delay_ms(1);
        if (i > 100)
        {
            return -1;
        }
    }
    if (newConfig->dev_num != read_config()) 
        config(newConfig);
    _busy = 1;
    return 0;
}
/**
  *@brief    �ͷſ���Ȩ
  *@param    none
  *@retval   none
  */
int8_t mcuSpi::release(void)
{
    _busy = 0;
    return 0;
}



