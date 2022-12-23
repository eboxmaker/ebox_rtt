#ifndef __EBOXTWOWIRE_H
#define __EBOXTWOWIRE_H
#include "ebox_core.h"
#include "i2c.h"
#include "stm32_hal.h"

extern "C" {
#include "twi.h"
}
class mcuI2c : public I2c
{

public:

    // public methods
    mcuI2c();
    mcuI2c(I2C_TypeDef *I2Cx, Gpio *sclPin, Gpio *sdaPin);

    virtual void begin(uint8_t address);
    virtual void begin(int address);
    virtual void begin();
    virtual void setClock(Speed_t speed = K100);
    virtual size_t write(uint8_t data);
    virtual size_t write(const uint8_t *data, size_t size);
    using   I2c::write;
    void onReceive(void (*)(int));
    void onRequest(void (*)(void));

    uint8_t ownAddress;
    i2c_t _i2c;

private:
    
  // per object data
    Gpio            *_sda;
    Gpio            *_scl;

//    virtual i2c_err_t _write(const uint8_t *data, size_t);
    virtual i2c_err_t _write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop);
    virtual i2c_err_t _read(uint8_t address,uint8_t *data, uint16_t quantity,uint8_t sendStop);
    
    void (*user_onRequest)(void);
    void (*user_onReceive)(int);
    static void onRequestService(i2c_t *);
    static void onReceiveService(i2c_t *);
       
};
#endif
