/**
  ******************************************************************************
  * @file    twi.c
  * @author  WI6LABS
  * @version V1.0.0
  * @date    01-August-2016
  * @brief   provide the TWI interface
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#include "twi.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private Defines */
/// @brief I2C timeout in tick unit
#ifndef I2C_TIMEOUT_TICK
#define I2C_TIMEOUT_TICK        100
#endif

#define SLAVE_MODE_TRANSMIT     0
#define SLAVE_MODE_RECEIVE      1
#define SLAVE_MODE_LISTEN       2

#if defined (STM32L451xx)
    #define I2C_TIMING
#endif


/*  Family specific description for I2C */
typedef enum {
#if defined(I2C1_BASE)
  I2C1_INDEX,
#endif
#if defined(I2C2_BASE)
  I2C2_INDEX,
#endif
#if defined(I2C3_BASE)
  I2C3_INDEX,
#endif
#if defined(I2C4_BASE)
  I2C4_INDEX,
#endif
#if defined(I2C5_BASE)
  I2C5_INDEX,
#endif
#if defined(I2C6_BASE)
  I2C6_INDEX,
#endif
  I2C_NUM
} i2c_index_t;

/* Private Variables */
static I2C_HandleTypeDef *i2c_handles[I2C_NUM];


/**
* @brief Compute I2C timing according current I2C clock source and
required I2C clock.
* @param  obj : pointer to i2c_t structure
* @param frequency
 Required I2C clock in Hz.
* @retval I2C timing or 0 in case of error.
*/
static uint32_t i2c_getTiming(i2c_t *obj, uint32_t frequency)
{
  uint32_t ret = 0;
  uint32_t i2c_speed = 0;
  if (frequency <= 100000) {
    i2c_speed = 100000;
  } else if (frequency <= 400000) {
    i2c_speed = 400000;
  } else if (frequency <= 1000000) {
    i2c_speed = 1000000;
  }
return i2c_speed;
}

/**
  * @brief  Default init and setup GPIO and I2C peripheral
  * @param  obj : pointer to i2c_t structure
  * @retval none
  */
void i2c_init(i2c_t *obj)
{
  i2c_custom_init(obj, 100000, I2C_ADDRESSINGMODE_7BIT, 0x33);
}

/**
  * @brief  Initialize and setup GPIO and I2C peripheral
  * @param  obj : pointer to i2c_t structure
  * @param  timing : one of the i2c_timing_e
  * @param  addressingMode : I2C_ADDRESSINGMODE_7BIT or I2C_ADDRESSINGMODE_10BIT
  * @param  ownAddress : device address
  * @retval none
  */
void i2c_custom_init(i2c_t *obj, uint32_t timing, uint32_t addressingMode, uint32_t ownAddress)
{
  if (obj != NULL) {


    I2C_HandleTypeDef *handle = &(obj->handle);


#if defined I2C1_BASE
    // Enable I2C1 clock if not done
    if (obj->i2c == I2C1) 
    {
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_I2C1_FORCE_RESET();
        __HAL_RCC_I2C1_RELEASE_RESET();

        obj->irq = I2C1_EV_IRQn;
#if !defined(STM32F0xx) && !defined(STM32G0xx) && !defined(STM32L0xx)
          obj->irqER = I2C1_ER_IRQn;
#endif /* !STM32F0xx && !STM32G0xx && !STM32L0xx */
        i2c_handles[I2C1_INDEX] = handle;
    }
#endif // I2C1_BASE
#if defined I2C2_BASE
    // Enable I2C2 clock if not done
    if (obj->i2c == I2C2) 
    {
        __HAL_RCC_I2C2_CLK_ENABLE();
        __HAL_RCC_I2C2_FORCE_RESET();
        __HAL_RCC_I2C2_RELEASE_RESET();
        obj->irq = I2C2_EV_IRQn;
#if !defined(STM32F0xx) && !defined(STM32G0xx) && !defined(STM32L0xx)
        obj->irqER = I2C2_ER_IRQn;
#endif /* !STM32F0xx && !STM32G0xx && !STM32L0xx */
        i2c_handles[I2C2_INDEX] = handle;
    }
#endif // I2C2_BASE
#if defined I2C3_BASE
        // Enable I2C3 clock if not done
        if (obj->i2c == I2C3) {
          __HAL_RCC_I2C3_CLK_ENABLE();
          __HAL_RCC_I2C3_FORCE_RESET();
          __HAL_RCC_I2C3_RELEASE_RESET();
          obj->irq = I2C3_EV_IRQn;
#if !defined(STM32G0xx) && !defined(STM32L0xx)
          obj->irqER = I2C3_ER_IRQn;
#endif /* !STM32L0xx */
          i2c_handles[I2C3_INDEX] = handle;
        }
#endif // I2C3_BASE
#if defined I2C4_BASE
        // Enable I2C4 clock if not done
        if (obj->i2c == I2C4) {
          __HAL_RCC_I2C4_CLK_ENABLE();
          __HAL_RCC_I2C4_FORCE_RESET();
          __HAL_RCC_I2C4_RELEASE_RESET();
          obj->irq = I2C4_EV_IRQn;
          obj->irqER = I2C4_ER_IRQn;
          i2c_handles[I2C4_INDEX] = handle;
        }
#endif // I2C4_BASE
#if defined I2C5_BASE
        // Enable I2C5 clock if not done
        if (obj->i2c == I2C5) {
          __HAL_RCC_I2C5_CLK_ENABLE();
          __HAL_RCC_I2C5_FORCE_RESET();
          __HAL_RCC_I2C5_RELEASE_RESET();
          obj->irq = I2C5_EV_IRQn;
          obj->irqER = I2C5_ER_IRQn;
          i2c_handles[I2C5_INDEX] = handle;
        }
#endif // I2C5_BASE
#if defined I2C6_BASE
        // Enable I2C6 clock if not done
        if (obj->i2c == I2C6) {
          __HAL_RCC_I2C6_CLK_ENABLE();
          __HAL_RCC_I2C6_FORCE_RESET();
          __HAL_RCC_I2C6_RELEASE_RESET();
          obj->irq = I2C6_EV_IRQn;
          obj->irqER = I2C6_ER_IRQn;
          i2c_handles[I2C6_INDEX] = handle;
        }
#endif // I2C6_BASE

        /* Configure I2C GPIO pins */


        handle->Instance             = obj->i2c;
       
        #ifdef I2C_TIMING
            i2c_setTiming(obj, timing);
        #else
//            obj->handle.Init.ClockSpeed = i2c_getTiming(obj, frequency);
//            /* Standard mode (sm) is up to 100kHz, then it's Fast mode (fm)     */
//            /* In fast mode duty cyble bit must be set in CCR register          */
//            if (frequency > 100000) {
//            obj->handle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
//            } else {
//            obj->handle.Init.DutyCycle       = I2C_DUTYCYCLE_2;
////            }
        #endif
        handle->Init.OwnAddress1     = ownAddress;
        handle->Init.OwnAddress2     = 0;
        handle->Init.AddressingMode  = addressingMode;
        handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        handle->Init.GeneralCallMode = (obj->generalCall == 0) ? I2C_GENERALCALL_DISABLE : I2C_GENERALCALL_ENABLE;
        handle->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

        handle->State = HAL_I2C_STATE_RESET;

        HAL_NVIC_SetPriority(obj->irq, I2C_IRQ_PRIO, I2C_IRQ_SUBPRIO);
        HAL_NVIC_EnableIRQ(obj->irq);
#if !defined(STM32F0xx) && !defined(STM32G0xx) && !defined(STM32L0xx)
        HAL_NVIC_SetPriority(obj->irqER, I2C_IRQ_PRIO, I2C_IRQ_SUBPRIO);
        HAL_NVIC_EnableIRQ(obj->irqER);
#endif /* !STM32F0xx && !STM32G0xx && !STM32L0xx */

        /* Init the I2C */
        if (HAL_I2C_Init(handle) != HAL_OK)
       {
          /* Initialization Error */
          Error_Handler();
        }

        /* Initialize default values */
        obj->slaveRxNbData = 0;
        obj->slaveMode = SLAVE_MODE_LISTEN;
      }
}

/**
  * @brief  Initialize and setup GPIO and I2C peripheral
  * @param  obj : pointer to i2c_t structure
  * @retval none
  */
void i2c_deinit(i2c_t *obj)
{
  HAL_NVIC_DisableIRQ(obj->irq);
#if !defined(STM32F0xx) && !defined(STM32G0xx) && !defined(STM32L0xx)
  HAL_NVIC_DisableIRQ(obj->irqER);
#endif /* !STM32F0xx && !STM32G0xx && !STM32L0xx */
  HAL_I2C_DeInit(&(obj->handle));
}

/**
  * @brief  Setup transmission speed. I2C must be configured before.
  * @param  obj : pointer to i2c_t structure
  * @param  frequency : i2c transmission speed
  * @retval none
  */
void i2c_setTiming(i2c_t *obj, uint32_t frequency)
{
  uint32_t f = i2c_getTiming(obj, frequency);
  __HAL_I2C_DISABLE(&(obj->handle));

#ifdef I2C_TIMING
  obj->handle.Init.Timing = f;
#else
obj->handle.Init.ClockSpeed = f;
/* Standard mode (sm) is up to 100kHz, then it's Fast mode (fm)     */
/* In fast mode duty cyble bit must be set in CCR register          */
if (frequency > 100000) {
obj->handle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
} else {
obj->handle.Init.DutyCycle       = I2C_DUTYCYCLE_2;
}
#endif
  HAL_I2C_Init(&(obj->handle));
  __HAL_I2C_ENABLE(&(obj->handle));
}

/**
  * @brief  Write bytes at a given address
  * @param  obj : pointer to i2c_t structure
  * @param  dev_address: specifies the address of the device.
  * @param  data: pointer to data to be write
  * @param  size: number of bytes to be write.
  * @retval read status
  */
i2c_status_e i2c_master_write(i2c_t *obj, uint8_t dev_address,
                              uint8_t *data, uint16_t size)

{
  i2c_status_e ret = I2C_OK;
  uint32_t tickstart = HAL_GetTick();
  uint32_t delta = 0;
  uint32_t err = 0;

  /* When size is 0, this is usually an I2C scan / ping to check if device is there and ready */
  if (size == 0) {
    ret = i2c_IsDeviceReady(obj, dev_address, 1);
  } else {
#if defined(I2C_OTHER_FRAME)
    uint32_t XferOptions = obj->handle.XferOptions; // save XferOptions value, because handle can be modified by HAL, which cause issue in case of NACK from slave
#endif

#if defined(I2C_OTHER_FRAME)
    if (HAL_I2C_Master_Seq_Transmit_IT(&(obj->handle), dev_address, data, size, XferOptions) == HAL_OK) {
#else
    if (HAL_I2C_Master_Transmit_IT(&(obj->handle), dev_address, data, size) == HAL_OK) {
#endif
      // wait for transfer completion
      while ((HAL_I2C_GetState(&(obj->handle)) != HAL_I2C_STATE_READY) && (delta < I2C_TIMEOUT_TICK)) {
        delta = (HAL_GetTick() - tickstart);
        if (HAL_I2C_GetError(&(obj->handle)) != HAL_I2C_ERROR_NONE) {
          break;
        }
      }

      err = HAL_I2C_GetError(&(obj->handle));
      if ((delta >= I2C_TIMEOUT_TICK)|| ((err & HAL_I2C_ERROR_TIMEOUT) == HAL_I2C_ERROR_TIMEOUT))
      {
        ret = I2C_TIMEOUT;
      } else {
        if ((err & HAL_I2C_ERROR_AF) == HAL_I2C_ERROR_AF) {
          ret = I2C_NACK_DATA;
        } else if (err != HAL_I2C_ERROR_NONE) {
          ret = I2C_ERROR;
        }
      }
    }
  }
  return ret;
}

/**
  * @brief  Write bytes to master
  * @param  obj : pointer to i2c_t structure
  * @param  data: pointer to data to be write
  * @param  size: number of bytes to be write.
  * @retval status
  */
i2c_status_e i2c_slave_write_IT(i2c_t *obj, uint8_t *data, uint16_t size)
{
  uint8_t i = 0;
  i2c_status_e ret = I2C_OK;

  // Protection to not override the TxBuffer
  if (size > I2C_TXRX_BUFFER_SIZE) {
    ret = I2C_DATA_TOO_LONG;
  } else {
    // Check the communication status
    for (i = 0; i < size; i++) {
      obj->i2cTxRxBuffer[obj->i2cTxRxBufferSize + i] = *(data + i);
    }

    obj->i2cTxRxBufferSize += size;
  }
  return ret;
}

/**
  * @brief  read bytes in master mode at a given address
  * @param  obj : pointer to i2c_t structure
  * @param  dev_address: specifies the address of the device.
  * @param  data: pointer to data to be read
  * @param  size: number of bytes to be read.
  * @retval read status
  */
i2c_status_e i2c_master_read(i2c_t *obj, uint8_t dev_address, uint8_t *data, uint16_t size)
{
  i2c_status_e ret = I2C_OK;
  uint32_t tickstart = HAL_GetTick();
  uint32_t delta = 0;
  uint32_t err = 0;

#if defined(I2C_OTHER_FRAME)
  uint32_t XferOptions = obj->handle.XferOptions; // save XferOptions value, because handle can be modified by HAL, which cause issue in case of NACK from slave
#endif

#if defined(I2C_OTHER_FRAME)
  if (HAL_I2C_Master_Seq_Receive_IT(&(obj->handle), dev_address, data, size, XferOptions) == HAL_OK) {
#else
  if (HAL_I2C_Master_Receive_IT(&(obj->handle), dev_address, data, size) == HAL_OK) {
#endif
    // wait for transfer completion
    while ((HAL_I2C_GetState(&(obj->handle)) != HAL_I2C_STATE_READY) && (delta < I2C_TIMEOUT_TICK)) {
      delta = (HAL_GetTick() - tickstart);
      if (HAL_I2C_GetError(&(obj->handle)) != HAL_I2C_ERROR_NONE) {
        break;
      }
    }

    err = HAL_I2C_GetError(&(obj->handle));
    if ((delta >= I2C_TIMEOUT_TICK)
        || ((err & HAL_I2C_ERROR_TIMEOUT) == HAL_I2C_ERROR_TIMEOUT)) {
      ret = I2C_TIMEOUT;
    } else {
      if ((err & HAL_I2C_ERROR_AF) == HAL_I2C_ERROR_AF) {
        ret = I2C_NACK_DATA;
      } else if (err != HAL_I2C_ERROR_NONE) {
        ret = I2C_ERROR;
      }
    }
  }
  return ret;
}

/**
  * @brief  Checks if target device is ready for communication
  * @param  obj : pointer to i2c_t structure
  * @param  devAddr: specifies the address of the device.
  * @param  trials : Number of trials.
  * @retval status
  */
i2c_status_e i2c_IsDeviceReady(i2c_t *obj, uint8_t devAddr, uint32_t trials)
{
  i2c_status_e ret = I2C_OK;

  switch (HAL_I2C_IsDeviceReady(&(obj->handle), devAddr, trials, I2C_TIMEOUT_TICK)) {
    case HAL_OK:
      ret = I2C_OK;
      break;
    case HAL_TIMEOUT:
      ret = (obj->handle.State != HAL_I2C_STATE_READY) ? I2C_TIMEOUT : I2C_NACK_ADDR;
      break;
    case HAL_BUSY:
      ret = (obj->handle.State != HAL_I2C_STATE_READY) ? I2C_BUSY : I2C_NACK_ADDR;
      break;
    default:
      ret = (obj->handle.State != HAL_I2C_STATE_READY) ? I2C_ERROR : I2C_NACK_ADDR;
      break;
  }
  return ret;
}

/* Aim of the function is to get i2c_s pointer using hi2c pointer */
/* Highly inspired from magical linux kernel's "container_of" */
/* (which was not directly used since not compatible with IAR toolchain) */
i2c_t *get_i2c_obj(I2C_HandleTypeDef *hi2c)
{
  struct i2c_s *obj_s;
  i2c_t *obj;

  obj_s = (struct i2c_s *)((char *)hi2c - offsetof(struct i2c_s, handle));
  obj = (i2c_t *)((char *)obj_s - offsetof(i2c_t, i2c));

  return (obj);
}

/** @brief  sets function called before a slave read operation
  * @param  obj : pointer to i2c_t structure
  * @param  function: callback function to use
  * @retval None
  */
void i2c_attachSlaveRxEvent(i2c_t *obj, void (*function)(i2c_t *))
{
  if ((obj != NULL) && (function != NULL)) {
    obj->i2c_onSlaveReceive = function;
    HAL_I2C_EnableListen_IT(&(obj->handle));
  }
}

/** @brief  sets function called before a slave write operation
  * @param  obj : pointer to i2c_t structure
  * @param  function: callback function to use
  * @retval None
  */
void i2c_attachSlaveTxEvent(i2c_t *obj, void (*function)(i2c_t *))
{
  if ((obj != NULL) && (function != NULL)) {
    obj->i2c_onSlaveTransmit = function;
    HAL_I2C_EnableListen_IT(&(obj->handle));
  }
}

/**
  * @brief  Slave Address Match callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  TransferDirection: Master request Transfer Direction (Write/Read), value of @ref I2C_XferOptions_definition
  * @param  AddrMatchCode: Address Match Code
  * @retval None
  */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
  i2c_t *obj = get_i2c_obj(hi2c);
  if ((obj->slaveMode == SLAVE_MODE_RECEIVE) && (obj->slaveRxNbData != 0)) {
    obj->i2c_onSlaveReceive(obj);
    obj->slaveMode = SLAVE_MODE_LISTEN;
    obj->slaveRxNbData = 0;
  }
//    rt_kprintf("HAL_I2C_AddrCallback\n");
  if (AddrMatchCode == hi2c->Init.OwnAddress1) {
    if (TransferDirection == I2C_DIRECTION_RECEIVE) {
      obj->slaveMode = SLAVE_MODE_TRANSMIT;

      if (obj->i2c_onSlaveTransmit != NULL) {
        obj->i2cTxRxBufferSize = 0;
        obj->i2c_onSlaveTransmit(obj);
      }
      HAL_I2C_Slave_Seq_Transmit_IT(hi2c, (uint8_t *) obj->i2cTxRxBuffer,
                                    obj->i2cTxRxBufferSize, I2C_LAST_FRAME);
//      rt_kprintf("HAL_I2C_Slave_Seq_Transmit_IT:%d\n",obj->i2cTxRxBufferSize);

    } else {
      obj->slaveRxNbData = 0;
      obj->slaveMode = SLAVE_MODE_RECEIVE;
      /*  We don't know in advance how many bytes will be sent by master so
       *  we'll fetch one by one until master ends the sequence */
      HAL_I2C_Slave_Seq_Receive_IT(hi2c, (uint8_t *) & (obj->i2cTxRxBuffer[obj->slaveRxNbData]),
                                   1, I2C_NEXT_FRAME);
    }
  }
}

/**
  * @brief  Listen Complete callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
  i2c_t *obj = get_i2c_obj(hi2c);

  /*  Previous master transaction now ended, so inform upper layer if needed
   *  then prepare for listening to next request */
  if ((obj->slaveMode == SLAVE_MODE_RECEIVE) && (obj->slaveRxNbData != 0)) {
    obj->i2c_onSlaveReceive(obj);
  }
  obj->slaveMode = SLAVE_MODE_LISTEN;
  obj->slaveRxNbData = 0;
  HAL_I2C_EnableListen_IT(hi2c);
//    rt_kprintf("HAL_I2C_ListenCpltCallback\n");
}

/**
  * @brief Slave RX complete callback
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  i2c_t *obj = get_i2c_obj(hi2c);
  /* One more byte was received, store it then prepare next */
  if (obj->slaveRxNbData < I2C_TXRX_BUFFER_SIZE) {
    obj->slaveRxNbData++;
  } else {
  }
  /* Restart interrupt mode for next Byte */
  if (obj->slaveMode == SLAVE_MODE_RECEIVE) {
    HAL_I2C_Slave_Seq_Receive_IT(hi2c, (uint8_t *) & (obj->i2cTxRxBuffer[obj->slaveRxNbData]),
                                 1, I2C_NEXT_FRAME);
  }
//    rt_kprintf("HAL_I2C_SlaveRxCpltCallback\n");

}

/**
  * @brief Slave TX complete callback
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  i2c_t *obj = get_i2c_obj(hi2c);
  /* Reset transmit buffer size */
  obj->i2cTxRxBufferSize = 0;
//    rt_kprintf("HAL_I2C_SlaveTxCpltCallback\n");
}

/**
  * @brief  I2C error callback.
  * @note   In master mode, the callback is not used because the error is reported
  *         to the Arduino API from i2c_master_write() and i2c_master_read().
  *         In slave mode, there is no mechanism in Arduino API to report an error
  *         so the error callback forces the slave to listen again.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
  i2c_t *obj = get_i2c_obj(hi2c);

  if (obj->isMaster == 0) {
//      rt_kprintf("HAL_I2C_ErrorCallback\n");
      if(READ_BIT(hi2c->Instance->CR1, I2C_CR1_SWRST) != RESET )
      {
         CLEAR_BIT(hi2c->Instance->CR1, I2C_CR1_SWRST);
        i2c_custom_init(obj, 100000, I2C_ADDRESSINGMODE_7BIT, hi2c->Init.OwnAddress1);
        HAL_I2C_EnableListen_IT(&(obj->handle));
      }
//    HAL_I2C_EnableListen_IT(hi2c);
  }
}

#if defined(I2C1_BASE)
/**
* @brief  This function handles I2C1 interrupt.
* @param  None
* @retval None
*/
void I2C1_EV_IRQHandler(void)
{
  I2C_HandleTypeDef *handle = i2c_handles[I2C1_INDEX];
  HAL_I2C_EV_IRQHandler(handle);
#if defined(STM32F0xx) || defined(STM32G0xx) || defined(STM32L0xx)
  HAL_I2C_ER_IRQHandler(handle);
#endif /* STM32F0xx || STM32G0xx || STM32L0xx */
}

#if !defined(STM32F0xx) && !defined(STM32G0xx) && !defined(STM32L0xx)
/**
* @brief  This function handles I2C1 interrupt.
* @param  None
* @retval None
*/
void I2C1_ER_IRQHandler(void)
{
  I2C_HandleTypeDef *handle = i2c_handles[I2C1_INDEX];
  HAL_I2C_ER_IRQHandler(handle);
}
#endif /* !STM32F0xx && !STM32G0xx && !STM32L0xx */
#endif // I2C1_BASE

#if defined(I2C2_BASE)
/**
* @brief  This function handles I2C2 interrupt.
* @param  None
* @retval None
*/
void I2C2_EV_IRQHandler(void)
{
#if defined(I2C3_BASE) && defined(STM32G0xx)
  /* I2C2_3_IRQHandler */
  I2C_HandleTypeDef *handle2 = i2c_handles[I2C2_INDEX];
  I2C_HandleTypeDef *handle3 = i2c_handles[I2C3_INDEX];
  if (handle2) {
    HAL_I2C_EV_IRQHandler(handle2);
    HAL_I2C_ER_IRQHandler(handle2);
  }
  if (handle3) {
    HAL_I2C_EV_IRQHandler(handle3);
    HAL_I2C_ER_IRQHandler(handle3);
  }
#else
  I2C_HandleTypeDef *handle = i2c_handles[I2C2_INDEX];
//    rt_kprintf("EV\n");
  HAL_I2C_EV_IRQHandler(handle);
#if defined(STM32F0xx) || defined(STM32G0xx) || defined(STM32L0xx)
  HAL_I2C_ER_IRQHandler(handle);
#endif /* STM32F0xx || STM32G0xx || STM32L0xx */
#endif
}

#if !defined(STM32F0xx) && !defined(STM32G0xx) && !defined(STM32L0xx)
/**
* @brief  This function handles I2C2 interrupt.
* @param  None
* @retval None
*/
void I2C2_ER_IRQHandler(void)
{
  I2C_HandleTypeDef *handle = i2c_handles[I2C2_INDEX];
//    rt_kprintf("I2C2_ER_IRQHandler\n");
  HAL_I2C_ER_IRQHandler(handle);
}
#endif /* !STM32F0xx && !STM32G0xx && !STM32L0xx */
#endif // I2C2_BASE

#if defined(I2C3_BASE) && !defined(STM32G0xx)
/**
* @brief  This function handles I2C3 interrupt.
* @param  None
* @retval None
*/
void I2C3_EV_IRQHandler(void)
{
  I2C_HandleTypeDef *handle = i2c_handles[I2C3_INDEX];
  HAL_I2C_EV_IRQHandler(handle);
#if defined(STM32L0xx)
  HAL_I2C_ER_IRQHandler(handle);
#endif /* STM32L0xx */
}

#if !defined(STM32L0xx)
/**
* @brief  This function handles I2C3 interrupt.
* @param  None
* @retval None
*/
void I2C3_ER_IRQHandler(void)
{
  I2C_HandleTypeDef *handle = i2c_handles[I2C3_INDEX];
  HAL_I2C_ER_IRQHandler(handle);
}
#endif /* !STM32L0xx */
#endif /* I2C3_BASE && ! STM32G0xx */

#if defined(I2C4_BASE)
/**
* @brief  This function handles I2C4 interrupt.
* @param  None
* @retval None
*/
void I2C4_EV_IRQHandler(void)
{
  I2C_HandleTypeDef *handle = i2c_handles[I2C4_INDEX];
  HAL_I2C_EV_IRQHandler(handle);
  HAL_I2C_ER_IRQHandler(handle);
}

/**
* @brief  This function handles I2C4 interrupt.
* @param  None
* @retval None
*/
void I2C4_ER_IRQHandler(void)
{
  I2C_HandleTypeDef *handle = i2c_handles[I2C4_INDEX];
  HAL_I2C_ER_IRQHandler(handle);
}
#endif // I2C4_BASE

#if defined(I2C5_BASE)
/**
* @brief  This function handles I2C5 interrupt.
* @param  None
* @retval None
*/
void I2C5_EV_IRQHandler(void)
{
  I2C_HandleTypeDef *handle = i2c_handles[I2C5_INDEX];
  HAL_I2C_EV_IRQHandler(handle);
}

/**
* @brief  This function handles I2C5 interrupt.
* @param  None
* @retval None
*/
void I2C5_ER_IRQHandler(void)
{
  I2C_HandleTypeDef *handle = i2c_handles[I2C5_INDEX];
  HAL_I2C_ER_IRQHandler(handle);
}
#endif // I2C5_BASE

#if defined(I2C6_BASE)
/**
* @brief  This function handles I2C6 interrupt.
* @param  None
* @retval None
*/
void I2C6_EV_IRQHandler(void)
{
  I2C_HandleTypeDef *handle = i2c_handles[I2C6_INDEX];
  HAL_I2C_EV_IRQHandler(handle);
}

/**
* @brief  This function handles I2C6 interrupt.
* @param  None
* @retval None
*/
void I2C6_ER_IRQHandler(void)
{
  I2C_HandleTypeDef *handle = i2c_handles[I2C6_INDEX];
  HAL_I2C_ER_IRQHandler(handle);
}
#endif // I2C6_BASE

#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
