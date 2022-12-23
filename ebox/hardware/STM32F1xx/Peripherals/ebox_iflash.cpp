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
#include "ebox_iflash.h"
#include "stm32f1xx_hal.h"


//#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define EBOX_DEBUG_IFLASH_ENABLE       false
#define EBOX_DEBUG_IFLASH_ENABLE_ERR   false
//#endif


#if EBOX_DEBUG_IFLASH_ENABLE
#define iflashDebug(...)  rt_kprintf("[iflash DBG]:%d: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define iflashDebug(...)
#endif

#if EBOX_DEBUG_IFLASH_ENABLE_ERR
#define iflashDebugErr(fmt, ...)  rt_kprintf("[iflash err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define iflashDebugErr(fmt, ...)
#endif

#define AddrToPage(addr) (addr / cpu.flash.page_size)

/**
 *@brief    ��ָ����������
 *@param    uint32_t iAddress ��ʼ��ַ
 *@retval   ���������ݳ���
*/
int ebox_flashRead(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToRead)
{
    int i = 0;
    while (i < iNbrToRead )
    {
        *(buf + i) = *( uint8_t *) iAddress++;
//        iflashDebug("read: iAddress=0X%08X ,value:0X%02X\n",iAddress - 1 ,*(buf + i));
        i++;
    }
    return i;
}

/**
  * @brief  Erases a specified FLASH page.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Page_Address: The page address to be erased.
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
HAL_StatusTypeDef FLASH_ErasePage(uint32_t PageAddress)
{
    uint32_t PageError = 0;
    FLASH_EraseInitTypeDef fe;
    fe.TypeErase = FLASH_TYPEERASE_PAGES;
    fe.PageAddress = PageAddress;
    fe.NbPages = 1;
    return HAL_FLASHEx_Erase(&fe, &PageError);  //Erase page
}
uint16_t write_without_check(uint32_t address, uint8_t *buf, uint16_t iNumByteToWrite)
{
    uint16_t i;
    i = 0;
    while(i < iNumByteToWrite)
    {
        HAL_StatusTypeDef st = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *(uint32_t *)buf);
//        iflashDebug("st:%d,i:%d,num:%d\t 0X%08X\n",st,i,iNumByteToWrite,*(uint32_t *)buf);
        i = i + 4;
        address = address + 4;
        buf = buf + 4;
    }
    return iNumByteToWrite;
}
int Flash_write(uint32_t address, uint8_t *buf, uint32_t length)
{
    /* Unlock the Flash Bank1 Program Erase controller */
    uint32_t PageAddress;
    uint32_t iNumByteToWrite = length;
    uint16_t PageOffset;
    uint16_t PageRemain;
    uint16_t i = 0;
    // ���建���������������ҳ����
    uint8_t *flash_buf = (uint8_t *)rt_malloc(FLASH_PAGE_SIZE+32);

    
    if (address < cpu.flash.start ||address >= cpu.flash.end)
    {
        iflashDebug("write:address(0x%08x) overflow; flash end:0x%x \n", address,cpu.flash.end);
        return -1;
    }
    HAL_FLASH_Unlock();
    PageAddress = address & (~(cpu.flash.page_size - 1 )) ; //������ַ
    PageOffset = address & (cpu.flash.page_size - 1);  //�������ڵ�ƫ��
    PageRemain = cpu.flash.page_size - PageOffset;       //����ʣ��ռ��С
    iflashDebug("PageAddress = 0x%x, secoff = 0x%x, secremain = 0x%x \n", PageAddress, PageOffset, PageRemain);
    if(iNumByteToWrite <= PageRemain) PageRemain = iNumByteToWrite; //������4096���ֽ�

    while( 1 )
    {
        ebox_flashRead(PageAddress, flash_buf, cpu.flash.page_size);   //������������
        for(i = 0; i < PageRemain; i++)   //У������
        {
            if(flash_buf[PageOffset + i] != 0XFF)break;   //��Ҫ����
        }
        if(i < PageRemain)  //��Ҫ����
        {
            FLASH_ErasePage(PageAddress); //�����������
            for(i = 0; i < PageRemain; i++) //����
            {
                flash_buf[i + PageOffset] = buf[i];
            }
            write_without_check(PageAddress, flash_buf, cpu.flash.page_size);   //д����������
        }
        else
        {
            write_without_check(address, buf, PageRemain); //д�Ѿ������˵�,ֱ��д������ʣ������.
        }

        if(iNumByteToWrite == PageRemain)  //д�������
        {
            break;
        }
        else     //����д�룬�Ƶ���һ������
        {
            PageAddress += cpu.flash.page_size;
            if (PageAddress >= cpu.flash.end)
            {
                iflashDebug("write:address(0x%08x) overflow; flash end:0x%x \n", address,cpu.flash.end);
                return -1;
            }
            PageOffset = 0;//ƫ��λ��Ϊ0
            buf += PageRemain;  //ָ��ƫ��
            address += PageRemain;//д��ַƫ��
            iNumByteToWrite -= PageRemain;  //�ֽ����ݼ�
            if(iNumByteToWrite > cpu.flash.page_size) PageRemain = cpu.flash.page_size; //��һ����������д����
            else PageRemain = iNumByteToWrite;  //��һ����������д����
        }

    }
	HAL_FLASH_Lock();
    rt_free(flash_buf);
    return length;
}


Flash::Flash()
{
 
}

bool Flash::begin()
{
    
    uint8_t total_page = cpu.flash.size / cpu.flash.page_size;
    uint32_t page_pgm_end = cpu.flash.used / cpu.flash.page_size;
    uint32_t page_remained = total_page - 1 - page_pgm_end;
    if(page_remained <= 0)
    {
        iflashDebugErr("û���㹻�Ŀռ�page_remained=%d\n",page_remained);
        return false;
    }
    uint32_t page_user_start = 1 + page_pgm_end;
    int used_page_maj = cpu.flash.used  / cpu.flash.page_size + \
        ((cpu.flash.used % cpu.flash.page_size)?1:0);
    
    _end_addr =  cpu.flash.end;
    _start_addr =  _end_addr - page_remained * cpu.flash.page_size + 1; 
    
    iflashDebug("pgm_end_addr:0x%08x\n",MCU_FLASH_PRG_END);
    iflashDebug("total_page:%d\n",total_page);
    iflashDebug("flash used:%d\n",cpu.flash.used);
    iflashDebug("used_page:%d\n",used_page_maj);
    iflashDebug("user_start_page:%d\n",AddrToPage(_start_addr));
    iflashDebug("user_end_page:%d\n",AddrToPage(_end_addr));
    return true;

}

/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  buf: specifies the data to be programmed.
  * @param  iNbrToWrite: the number to write into flash
  * @retval if success return the number to write, -1 if error
  *
  */
int Flash::write(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToWrite)
{
    uint32_t address = _start_addr + offsetAdd;
    Flash_write(address,buf,iNbrToWrite);
}
/**
 *@brief    ����һ������
 *@param    uint32_t offsetAdd  	Ҫ��ȡ�ĵ�ַ
		   uint8_t *buf			�����ȡ������
		   uint32_t iNbrToWrite	Ҫ��ȡ�����ݳ���
 *@retval   ���������ݳ��ȡ� ��� 0 ����
*/
int Flash::read(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToRead)
{
    // ���Ե�ַ
    int size = _end_addr - _start_addr;
    uint32_t iAddress = _start_addr + offsetAdd;
    if (offsetAdd >= size)
    {
        iflashDebugErr("read: offsetAdd=%d ���������޶���Χ\n",offsetAdd);
        return 0;
    }
    return ebox_flashRead(iAddress, buf, iNbrToRead);
}

uint32_t Flash::getSize()
{
    return (_end_addr - _start_addr);
}



