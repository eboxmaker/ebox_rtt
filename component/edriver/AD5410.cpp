/**
  ******************************************************************************
  * @file    74hc595.cpp
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
#include "ad5410.h"
#include "ebox.h"
AD5410::AD5410(Gpio *latch,Gpio *clear,Gpio *fault1,Gpio *fault2,Spi *spi)
{
    this->latch 	= latch;
    this->clear 	= clear;
    this->fault1 	= fault1;
    this->fault2 	= fault2;
    this->spi       = spi;
    nums = 2;
}

void AD5410::begin()
{
    latch->mode(OUTPUT_PP);
    clear->mode(OUTPUT_PP);
    fault1->mode(INPUT);
    fault2->mode(INPUT);
    latch->reset();
    clear->reset();

    cfg.dev_num = 0;
    cfg.mode = Spi::MODE0;
    cfg.prescaler = Spi::DIV16;
    cfg.bit_order = Spi::MSB;
    
    spi->begin(&cfg);
    
    
    
    for(int i = 0; i < 2; i++)
    {
        ctr[i].data = 0;
        ctr[i].bits.r = AD5420_0_24_RANGE;
        ctr[i].bits.dcEnable   = 1;
        ctr[i].bits.srEnable   = 0;
        ctr[i].bits.srStep     = AD5420_SR_STEPSIZE_1;
        ctr[i].bits.srClock    = AD5420_SR_CLK_257730;
        ctr[i].bits.outEnalbe  = 1;
        ctr[i].bits.rext       = 0;
    }
    reset();


    update_ctr();  
//    write_cur(0,10);
//    write_cur(1,20);
//    update_ctr();

}

void AD5410::write(uint8_t addr,uint16_t data)
{
    spi->take(&cfg);
    spi->write(addr);
    spi->write(data>>8);
    spi->write(data&0xff);
    spi->release();
}
void AD5410::update_ctr()
{

    for(int i = 0; i < 2; i++)
    {
        write(AD5420_CONTROL_REG,ctr[1 - i].data);
        latch->set();
//        delay_us(5);
        latch->reset();
//        delay_us(5);
    }

}


void AD5410::set_ma_mode(uint8_t index, MAmode_t mode)
{
    switch((uint8_t)mode)
    {
        case MA0_20:
            ctr[index].bits.r  = AD5420_0_20_RANGE;
            max_cur[index] = 20;
            min_cur[index] = 0;
            break;
        case MA0_24:
            ctr[index].bits.r  = AD5420_0_24_RANGE;
            max_cur[index] = 24;
            min_cur[index] = 0;
            break;
        case MA4_20:
            ctr[index].bits.r  = AD5420_4_20_RANGE;
            max_cur[index] = 20;
            min_cur[index] = 4;
            break;
    }
    update_ctr();
}



void AD5410::set(uint8_t index,float cur)
{
//    cur -= 3.5;
    switch(ctr[index].bits.r)
    {
        case AD5420_4_20_RANGE:
            cur_value[index] = (cur - 4)*65536/16 - 1;
            break;
        case AD5420_0_20_RANGE:
            cur_value[index] = (cur - 0)*65536/20 - 1;
            break;            
        case AD5420_0_24_RANGE:
            cur_value[index] = (cur - 0)*65536/24 - 1;
            break; 
        default:
            uart1.printf("parameter err\r\n");
            break;
    }
//    uart1.printf("…Ë÷√cur[%d]:0x%04x;%d;%0.2f\r\n",index,cur_value[index],ctr[index].bits.r,cur);
    
    
    for(int i = 0; i < 2; i++)
    {
        write(AD5420_DATA_REG,cur_value[1 - i]);
    }
    latch->set();
//    delay_us(1);
    latch->reset();
//    uart1.printf("cur[0]:0x%04x\r\n",cur_value[0]);
//    uart1.printf("cur[1]:0x%04x\r\n",cur_value[1]);
}
void AD5410::reset()   
{
    for(int i = 0; i < 2; i++)
	write(AD5420_RESET_REG , 1);
    delay_us(100);
    latch->set();
    delay_us(5);
    latch->reset();
    delay_us(5);
}


