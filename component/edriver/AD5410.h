/**
  ******************************************************************************
  * @file    74hc595.h
  * @author  shentq
  * @version V1.2
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __AD5410_H
#define  __AD5410_H

#include "ebox_core.h"


/*! \brief Internal address of DAC STATUS register.*/
#define AD5420_STATUS_REG        0x00
/*! \brief Internal address of DAC DATA register.*/
#define AD5420_DATA_REG          0x01
/*! \brief Internal address of DAC CONTROL register.*/
#define AD5420_CONTROL_REG       0x55

#define AD5420_RESET_REG       0x56



/*! \brief Internal flag of using REXT as reference resistance. Please set this
bit before or during changing #AD5420_OUTEN bit.*/
#define AD5420_REXT              0x2000
/*! \brief Internal flag of switching DAC on.*/
#define AD5420_OUTEN             0x1000
/*! \brief Internal flag of switching slew rate control on.*/
#define AD5420_SREN              0x0010
/*! \brief Internal flag of setting daisy-chain mode of SPI bus.*/
#define AD5420_DCEN              0x0008
/*! \brief Internal flag of setting 4-20 mA output range.*/
#define AD5420_4_20_RANGE        0x0005
/*! \brief Internal flag of setting 0-20 mA output range.*/
#define AD5420_0_20_RANGE        0x0006
/*! \brief Internal flag of setting 0-24 mA output range.*/
#define AD5420_0_24_RANGE        0x0007

/*! \brief Internal flag of output current fault in status register.*/
#define AD5420_IOUT_FAULT        0x0004
/*! \brief Internal flag of slew rate being active in status register.*/
#define AD5420_SR_ACTIVE         0x0002
/*! \brief Internal flag of overheat fault of DAC in status register.*/
#define AD5420_OVRHEAT_FAULT     0x0001


/******************************************************************************
**                                                                           **
**      Set of slew rate control defines                                     **
**                                                                           **
******************************************************************************/

/*! \brief Bit value that sets slewrate to 257,730 Hz.*/
#define AD5420_SR_CLK_257730     0
/*! \brief Bit value that sets slewrate to 198,410 Hz.*/
#define AD5420_SR_CLK_198410     1
/*! \brief Bit value that sets slewrate to 152,440 Hz.*/
#define AD5420_SR_CLK_152440     2
/*! \brief Bit value that sets slewrate to 131,580 Hz.*/
#define AD5420_SR_CLK_131580     3
/*! \brief Bit value that sets slewrate to 115,740 Hz.*/
#define AD5420_SR_CLK_115740     4
/*! \brief Bit value that sets slewrate to 69,440 Hz.*/
#define AD5420_SR_CLK_69440      5
/*! \brief Bit value that sets slewrate to 37,590 Hz.*/
#define AD5420_SR_CLK_37590      6
/*! \brief Bit value that sets slewrate to 25,770 Hz.*/
#define AD5420_SR_CLK_25770      7
/*! \brief Bit value that sets slewrate to 20,160 Hz.*/
#define AD5420_SR_CLK_20160      8
/*! \brief Bit value that sets slewrate to 16,030 Hz.*/
#define AD5420_SR_CLK_16030      0x9
/*! \brief Bit value that sets slewrate to 10,290 Hz.*/
#define AD5420_SR_CLK_10290      0xa
/*! \brief Bit value that sets slewrate to 8,280 Hz.*/
#define AD5420_SR_CLK_8280       0xb
 /*! \brief Bit value that sets slewrate to 6,900 Hz.*/
#define AD5420_SR_CLK_6900       0xc
/*! \brief Bit value that sets slewrate to 5,530 Hz.*/
#define AD5420_SR_CLK_5530       0xd
/*! \brief Bit value that sets slewrate to 4,240 Hz.*/
#define AD5420_SR_CLK_4240       0xe
/*! \brief Bit value that sets slewrate to 3,300 Hz.*/
#define AD5420_SR_CLK_3300       0xf

/*! \brief Bit value that sets slewrate step to 1 LSB.*/
#define AD5420_SR_STEPSIZE_1     0x0000
/*! \brief Bit value that sets slewrate step to 2 LSB.*/
#define AD5420_SR_STEPSIZE_2     1
/*! \brief Bit value that sets slewrate step to 4 LSB.*/
#define AD5420_SR_STEPSIZE_4     2
/*! \brief Bit value that sets slewrate step to 8 LSB.*/
#define AD5420_SR_STEPSIZE_8     3
/*! \brief Bit value that sets slewrate step to 16 LSB.*/
#define AD5420_SR_STEPSIZE_16    4
/*! \brief Bit value that sets slewrate step to 32 LSB.*/
#define AD5420_SR_STEPSIZE_32    5
/*! \brief Bit value that sets slewrate step to 64 LSB.*/
#define AD5420_SR_STEPSIZE_64    6
/*! \brief Bit value that sets slewrate step to 128 LSB.*/
#define AD5420_SR_STEPSIZE_128   7

class AD5410
{
public:
    typedef struct{
        
        
        //101:4 mA至20 mA电流范围
        //110:0mA至20 mA电流范围
        //111:0 mA至24 mA电流范围
        uint16_t r:3;//输出范围选择。
        
        uint16_t dcEnable:1;//菊花链使能
        
        uint16_t srEnable:1;//数字压摆率控制使能。
        uint16_t srStep:3;//数字压摆率控制
        uint16_t srClock:4;//数字压摆率控制
        
        uint16_t outEnalbe:1;//输出使能。此位必须置1才能使能输出
        
        uint16_t rext:1;//此位置1将选择外部电流设置电阻。
        
        uint16_t null:2;
    }CtrBits_t;
    typedef union 
    {
        uint16_t data;
        CtrBits_t bits;
    }Ctr_t;
    
    typedef enum{MA0_20,MA0_24,MA4_20}MAmode_t;
    
    Ctr_t ctr[2];
    uint16_t cur_value[2];
public:

    AD5410(Gpio *latch,Gpio *clear,Gpio *fault1,Gpio *fault2,Spi *spi);
    void begin();
    void set(uint8_t index,float cur);

public:

    Gpio *latch;
    Gpio *clear;
    Gpio *fault1;
    Gpio *fault2;
    Spi  *spi;
    Spi::Config_t cfg;

    uint8_t nums;

private:
    void reset();
    void set_ma_mode(uint8_t index,MAmode_t mode);
    void update_ctr();    
    void write(uint8_t addr,uint16_t data);
    uint8_t max_cur[2];
    uint8_t min_cur[2];


};

#endif
