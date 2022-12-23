/**
  ******************************************************************************
  * @file    iflash.cpp
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
#include "ebox_flash.h"
#include "stm32_hal.h"



//#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_PFLASH_ENABLE       true
#define EBOX_DEBUG_PFLASH_ENABLE_ERR   true
//#endif


#if EBOX_DEBUG_PFLASH_ENABLE
#define flashDebug(...)  rt_kprintf("[iflash]:%d: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define flashDebug(...)
#endif

#if EBOX_DEBUG_PFLASH_ENABLE_ERR
#define flashDebugErr(fmt, ...)  rt_kprintf("[iflash err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define flashDebugErr(fmt, ...)
#endif

#define AddrToPage(addr) (addr / FLASH_PAGE_SIZE)


/**
  * @brief  Erases a specified FLASH page.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Page_Address: The page address to be erased.
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
  #include "rthw.h"
  #include "ebox.h"
static HAL_StatusTypeDef FLASH_ErasePage(uint32_t PageIndex)
{
    
    PA10.mode(INPUT_PU);
    PC5.mode(INPUT_PU);
    HAL_FLASH_Unlock();
    rt_kprintf("PageIndex: %d !!!\n",PageIndex);

    HAL_StatusTypeDef sta;
    uint32_t PageError = 0;
    FLASH_EraseInitTypeDef fe;
    fe.TypeErase = FLASH_TYPEERASE_PAGES;
    fe.Banks = FLASH_BANK_1;          
    fe.Page = PageIndex;
    fe.NbPages = 1;
    rt_enter_critical();
    int level = rt_hw_interrupt_disable();
    sta = HAL_FLASHEx_Erase(&fe, &PageError);  //Erase page
    rt_hw_interrupt_enable(level);
    rt_exit_critical();
    if(sta != HAL_OK)
    {
//        rt_kprintf("HAL_FLASHEx_Erase err = %d !!!\n",sta);
    }
    HAL_FLASH_Lock();
    PA10.mode(AF_PP,GPIO_AF7_USART1);
    PC5.mode(AF_PP,GPIO_AF7_USART3);
    return sta;
}

PFlash::PFlash()
{


}
bool PFlash::begin(uint16_t _page)
{
    uint16_t total_page = cpu.flash.size / FLASH_PAGE_SIZE;
    uint32_t page_pgm_end = cpu.flash.used / FLASH_PAGE_SIZE;
    uint32_t page_remained = total_page - 1 - page_pgm_end;
    if(page_remained <= 0)
    {
        flashDebug("没有足够的空间page_remained=%d\n",page_remained);
        return false;
    }
    page = total_page - 1;
    
    _end_addr =  cpu.flash.end;
    _start_addr = page * FLASH_PAGE_SIZE + MCU_FLASH_BEGIN; 
    
//    flashDebug("pgm_end_addr:0x%08x\n",MCU_FLASH_PRG_END);
//    flashDebug("total_page:%d\n",total_page);
//    flashDebug("cpu.flash.size:%d\n",cpu.flash.size);
//    flashDebug("FLASH_PAGE_SIZE:%d\n",FLASH_PAGE_SIZE);
//    flashDebug("flash used:%d\n",cpu.flash.used);
//    flashDebug("user page      :%d\n",page);
//    flashDebug("user_start_addr:0X%08X\n",_start_addr);
//    flashDebug("user_end_addr  :0X%08X\n",_end_addr);
    return true;
}

void PFlash::put(uint8_t *data,uint16_t len)
{
    
    flashDebug("flash put---------------!\n");
//    rt_enter_critical();
    FLASH_ErasePage(page);

    HAL_FLASH_Unlock();

    len = RT_ALIGN(len,8);
    uint8_t ndword = len / 8;
    uint32_t address = _start_addr;
    uint64_t *ptr = (uint64_t *)data;
//    flashDebug("ndword  :%d\n",ndword);
//    flashDebug("address  :0x%08x\n",address);
//    flashDebug("ptr  :%d\n",ptr);
//    flashDebug("len  :%d\n",len);
    uint16_t i;
    i = 0;
    while(i < ndword)
    {
        HAL_StatusTypeDef st = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, ptr[i]);
        if(st != 0)
        {
//            rt_kprintf("[%d/%d]HAL_FLASH_Program err = %d (addr = 0x%08x)!!!\n",i,ndword,st,address);
        }
        i++;
        address = address + 8;
    }
	HAL_FLASH_Lock();
//    rt_exit_critical();
    flashDebug("flash put!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    
}

int PFlash::get(uint8_t *buf,uint16_t len)
{
    int i = 0;
    uint8_t *ptr = ( uint8_t *)_start_addr;
    while (i < len )
    {
        *(buf + i) = *ptr++;
//        iflashDebug("read: iAddress=0X%08X ,value:0X%02X\n",iAddress - 1 ,*(buf + i));
        i++;
    }
    delay_ms(10);
    return i;
}


