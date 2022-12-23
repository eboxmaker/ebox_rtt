/**
  ******************************************************************************
  * @file    iflash.h
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef   __EBOX_FLASH_H__
#define   __EBOX_FLASH_H__

#include "ebox_core.h"


class PFlash
{
public:

    PFlash();

    bool begin(uint16_t page = -1);

    void put(uint8_t *data,uint16_t len);
    int get(uint8_t *buf,uint16_t len);

private:
    uint32_t _start_addr;	// 起始地址
    uint32_t _end_addr;		// 结束地址
    uint16_t page;
};



#endif
