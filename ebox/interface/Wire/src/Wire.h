/*
  TwoWire.h - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
*/

#ifndef TwoWire_h
#define TwoWire_h

#include "Stream.h"
#include "ebox_core.h"
extern "C" {
#include "twi.h"
}

// Minimal buffer length. Buffers length will be increased when needed,
// but TX buffer is limited to a maximum to avoid too much stack consumption
// Note: Buffer length and max buffer length are limited by uin16_t type
#define BUFFER_LENGTH 32
#if !defined(WIRE_MAX_TX_BUFF_LENGTH)
  #define WIRE_MAX_TX_BUFF_LENGTH       1024U
#endif

// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

class TwoWire : public Stream {
  private:
    uint8_t *rxBuffer;
    uint16_t rxBufferAllocated;
    uint16_t rxBufferIndex;
    uint16_t rxBufferLength;

    uint8_t txAddress;
    uint8_t *txBuffer;
    uint16_t txBufferAllocated;
    uint16_t txDataSize;

    uint8_t transmitting;

    uint8_t ownAddress;


    void (*user_onRequest)(void);
    void (*user_onReceive)(int);
    static void onRequestService(i2c_t *);
    static void onReceiveService(i2c_t *);

    void allocateRxBuffer(size_t length);
    size_t allocateTxBuffer(size_t length);

    void resetRxBuffer(void);
    void resetTxBuffer(void);

    Gpio *sda;
    Gpio *scl;
  public:
    i2c_t _i2c;
  TwoWire();
    TwoWire(Gpio *_sda, Gpio *_scl);
    // setSCL/SDA have to be called before begin()
    void begin(bool generalCall = false);
    void begin(Gpio*, Gpio*);
    void begin(uint8_t, bool generalCall = false);
    void begin(int, bool generalCall = false);
    void end();
    void setClock(uint32_t);
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, size_t, bool);
    uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive(void (*)(int));
    void onRequest(void (*)(void));

    inline size_t write(unsigned long n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(long n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(unsigned int n)
    {
      return write((uint8_t)n);
    }
    inline size_t write(int n)
    {
      return write((uint8_t)n);
    }
    using Print::write;
};



extern TwoWire Wire;

#endif
