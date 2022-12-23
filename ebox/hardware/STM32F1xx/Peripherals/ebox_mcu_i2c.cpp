
extern "C" {
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
}
#include "ebox_mcu_i2c.h"
#include "rcc.h"


#ifndef EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_MCUI2C_ENABLE       true
#define EBOX_DEBUG_MCUI2C_ENABLE_ERR   true
#endif


#if EBOX_DEBUG_MCUI2C_ENABLE
#define mcuI2cDebug(...)  rt_kprintf("[mcuI2C DBG]:%d: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define mcuI2cDebug(...)
#endif

#if EBOX_DEBUG_MCUI2C_ENABLE_ERR
#define mcuI2cDebugErr(fmt, ...)  rt_kprintf("[mcuI2C err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define mcuI2cDebugErr(fmt, ...)
#endif


#ifndef I2C_TIMEOUT_TICK
#define I2C_TIMEOUT_TICK        100
#endif
static const uint8_t MASTER_ADDRESS = 0x01;

mcuI2c::mcuI2c(I2C_TypeDef *I2Cx,Gpio *sclPin, Gpio *sdaPin)
{
    _scl = sclPin;
    _sda = sdaPin;
    _i2c.i2c = I2Cx;
    setTimeout(100);
}


/**
  *@brief    根据i2c时钟和设置速率speed计算timing,默认80K
  *@param    address:  0X01 default address as master,othere will begin as slave
  *@retval   None
  */
void mcuI2c::begin(uint8_t address)
{    
    _sda->mode(AF_OD_PU);
    _scl->mode(AF_OD_PU);
    ownAddress = address << 1;
    _i2c.__this = (void *)this;
    transmitting = 0;

    _i2c.isMaster = (address == MASTER_ADDRESS) ? 1 : 0;
    _i2c.generalCall =  0;
    i2c_custom_init(&_i2c, 100000, I2C_ADDRESSINGMODE_7BIT, ownAddress);
    if (_i2c.isMaster == 0) 
    {
        // i2c_attachSlaveTxEvent(&_i2c, reinterpret_cast<void(*)(i2c_t*)>(&TwoWire::onRequestService));
        // i2c_attachSlaveRxEvent(&_i2c, reinterpret_cast<void(*)(i2c_t*, uint8_t*, int)>(&TwoWire::onReceiveService));

        i2c_attachSlaveTxEvent(&_i2c, onRequestService);
        i2c_attachSlaveRxEvent(&_i2c, onReceiveService);
    }
}
void mcuI2c::begin(int address)
{
    begin((uint8_t)address);
}
void mcuI2c::begin()
{
    begin((uint8_t)MASTER_ADDRESS);
}
/**
 * @brief 设置i2c通信速度.
 *
 * @param[in] speed i2c速度，当其值为400K，300K，200K，100K时，i2c通信频率为该值.
 *
 * @return 0.
 */
void mcuI2c::setClock(Speed_t speed)
{
   switch (speed)
    {
    case K10:
        i2c_setTiming(&_i2c,10000);
        break;
    case K100:
        i2c_setTiming(&_i2c,100000);
        break;
    case K200:
        i2c_setTiming(&_i2c,200000);
        break;
    case K400:
        i2c_setTiming(&_i2c,400000);
        break;
    case K1000:
        i2c_setTiming(&_i2c,1000000);
        break;
    default:
        i2c_setTiming(&_i2c,100000);
    }
 
}
// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t mcuI2c::write(uint8_t data)
{
  size_t ret = 0;
  if (transmitting)
  {
    if(txLength >= I2C_BUFFER_LENGTH) 
    {
        return 0;
    }
    txBuffer[txIndex] = data;
    ++txIndex;
    txLength = txIndex;        
    return 1;
  } else 
  {
    // in slave send mode
    // reply to master
    if (i2c_slave_write_IT(&_i2c, &data, 1) != I2C_OK) 
    {
      ret = 0;
    }
  }
  return ret;
}

/**
  * @brief  This function must be called in slave Tx event callback or after
  *         beginTransmission() and before endTransmission().
  * @param  pdata: pointer to the buffer data
  * @param  quantity: number of bytes to write
  * @retval number of bytes ready to write.
  */
size_t mcuI2c::write(const uint8_t *data, size_t quantity)
{
  size_t ret = quantity;

  if (transmitting) 
  {
    for(size_t i = 0; i < quantity; ++i) 
    {
        if(!write(data[i]))
         {
            return i;
        }
    }
    return quantity;
  } 
  else 
  {
    // in slave send mode
    // reply to master
    if (i2c_slave_write_IT(&_i2c, (uint8_t *)data, quantity) != I2C_OK) 
    {
      ret = 0;
    }
  }
  return ret;
}


/*
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 *          5 .. timeout
 */
i2c_err_t mcuI2c::_write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop)
{
#if !defined(I2C_OTHER_FRAME)
  UNUSED(sendStop);
#endif
  int8_t ret = I2C_ERROR_BUS;
  // check transfer options and store it in the I2C handle
#if defined(I2C_OTHER_FRAME)
  if (sendStop == 0) {
    _i2c.handle.XferOptions = I2C_OTHER_FRAME ;
  } else {
    _i2c.handle.XferOptions = I2C_OTHER_AND_LAST_FRAME;
  }
#endif
   ret = (i2c_err_t)i2c_master_write(&_i2c, address, (uint8_t *)data, quantity); 
   return (i2c_err_t)ret;

}
/**
  *@brief    指定寄存器连续读取. start->WslaveAddr->regAddr->RSlaverAddr->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  从机地址
  *          uint16_t regAddr: 寄存器地址
  *          uint8_t *data: 读取到的数据
  *          uint16_t nRead：要读取的数据长度
  *          : 超时
  *@retval   EOK，EWAIT
  */
i2c_err_t mcuI2c::_read(uint8_t address,uint8_t *data, uint16_t length,uint8_t sendStop)
{
    i2c_status_e ret;
    uint8_t read = 0;

    // perform blocking read into buffer
#if defined(I2C_OTHER_FRAME)
    if (sendStop == 0) {
      _i2c.handle.XferOptions = I2C_OTHER_FRAME ;
    } else {
      _i2c.handle.XferOptions = I2C_OTHER_AND_LAST_FRAME;
    }
#endif
    ret = i2c_master_read(&_i2c, address, data, length);
    if (ret == I2C_OK) {
      read = length;
    }
    return (i2c_err_t)ret;
}




// behind the scenes function that is called when data is received
void mcuI2c::onReceiveService(i2c_t *obj)
{
  uint8_t *inBytes = (uint8_t *) obj->i2cTxRxBuffer;
  int numBytes = obj->slaveRxNbData;

  mcuI2c *TW = (mcuI2c *)(obj->__this);

  // don't bother if user hasn't registered a callback
  if (TW->user_onReceive) 
  {
    // don't bother if rx buffer is in use by a master requestFrom() op
    // i know this drops data, but it allows for slight stupidity
    // meaning, they may not have read all the master requestFrom() data yet
//    if (TW->rxIndex >= TW->rxLength) 
    {

      // copy twi rx buffer into local read buffer
      // this enables new reads to happen in parallel
      memcpy(TW->rxBuffer, inBytes, numBytes);
      // set rx iterator vars
      TW->rxIndex = 0;
      TW->rxLength = numBytes;
      // alert user program
      TW->user_onReceive(numBytes);
    }
  }
}

// behind the scenes function that is called when data is requested
void mcuI2c::onRequestService(i2c_t *obj)
{
  mcuI2c *TW = (mcuI2c *)(obj->__this);

  // don't bother if user hasn't registered a callback
  if (TW->user_onRequest) {
    // reset tx data size
    // !!! this will kill any pending pre-master sendTo() activity
    TW->txIndex = 0;
    TW->txLength = 0;
    // alert user program
    TW->user_onRequest();
  }
}

// sets function called on slave write
void mcuI2c::onReceive(void (*function)(int))
{
  user_onReceive = function;
}

// sets function called on slave read
void mcuI2c::onRequest(void (*function)(void))
{
  user_onRequest = function;
}

