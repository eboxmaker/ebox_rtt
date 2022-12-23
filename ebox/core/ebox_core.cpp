#include "ebox_core.h"
#include "stm32_hal.h"

#if __cplusplus
extern "C" {
#endif

Cpu_t cpu;
    
static uint16_t micro_para;
    
static void update_system_clock(CpuClock_t *clock)
{   
    clock->core = HAL_RCC_GetSysClockFreq();
    clock->hclk = HAL_RCC_GetHCLKFreq();
    clock->pclk2 = HAL_RCC_GetPCLK2Freq();
    clock->pclk1 = HAL_RCC_GetPCLK1Freq();
}    
static void update_chip_info()
{
    uint8_t *p = (uint8_t *)(UID_BASE);
    for(int i = 0 ; i < 12; i++)
    {
        cpu.chip_id[i] = *p++;
    }
    

    cpu.flash.size = *(uint16_t *)(FLASHSIZE_BASE);   //оƬflash����
    switch(cpu.flash.size)
    {
        case 32:
        case 64:
        case 128:
            cpu.flash.page_size = 1024;
            break;
        default:
            cpu.flash.page_size = 2048;
            break;
    }
    cpu.flash.size = cpu.flash.size * 1024;
    cpu.flash.start = MCU_FLASH_BEGIN;
    cpu.flash.end = MCU_FLASH_BEGIN + cpu.flash.size - 1;
    cpu.flash.used = MCU_FLASH_USED;
}

void ebox_init()
{
    update_system_clock(&cpu.clock);
    update_chip_info();
    micro_para = cpu.clock.core / 1000000; //���ٸ�systickʱ��Ϊ1us
}

void print_buf(uint8_t *buf, uint16_t len)
{
    rt_kprintf("-----------len=[%d]---------------\n",len);
    for(int i = 0; i < len; i++)
    {
        rt_kprintf("0X%02X ",buf[i]);
    }
    rt_kprintf("\n--------------------------\n");
}
/**
*@brief    ��ȡus
*@param    mcu
*@retval   none
*/
uint32_t micros(void)
{
    uint32_t micro;
    micro = (millis() * 1000 + ((SysTick->LOAD - SysTick->VAL) / micro_para));
    return  micro;
}

/**
  *@brief    ��ȡms
  *@param    mcu
  *@retval   none
  */
uint32_t  millis( void )
{
    return (rt_tick_get()* (1000 / RT_TICK_PER_SECOND));
}

/**
  *@brief    ms��ʱ
  *@param    uint32_t ms  Ҫ��ʱ��ʱ������С1ms
  *@retval   none
 */
void delay_ms(uint32_t ms)
{
    rt_err_t ret = rt_thread_mdelay(ms);
    return;
}
/**
  *@brief    us��ʱ,ʹ��systick��������48Mhz������ʱ��ʱ��������us(1.3)���ȡ�
  *          8Mhzʱ��С6-7us,24Mhzʱ��С2.2us,16Mhzʱ��С3.5us
  *@param    uint32_t us  Ҫ��ʱ��ʱ������С1us
  *@retval   none
 */
void  delay_us(uint32_t us)
{
    if(us == 0) return;
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;

    ticks = (us - 1) * micro_para;          /* �������� */
    tcnt = 0;
    told = SysTick->VAL;               /* ���浱ǰ����ֵ */

    while (1)
    {
        tnow = SysTick->VAL;
        tcnt += (tnow < told) ? (told - tnow) : (SysTick->LOAD - tnow + told);
        if (tcnt >= ticks)break;
        told = tnow;
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
__WEAK void _Error_Handler(const char *msg, int val)
{
  /* User can add his own implementation to report the HAL error return state */
  rt_kprintf("Error: %s (%i)\n", msg, val);
  while (1) {
  }
}


#if __cplusplus
} // extern "C"
#endif

uint16_t makeWord(unsigned int w)
{
    return w;
}
uint16_t makeWord(unsigned char h, unsigned char l)
{
    return (h << 8) | l;
}

char char2digital(
    char c	/**< is a character('0'-'F') to convert to HEX */
)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return 10 + c - 'a';
    if (c >= 'A' && c <= 'F')
        return 10 + c - 'A';

    return (char)c;
}

uint8_t bcd_to_dec(uint8_t bcd_code)
{
    uint8_t temp, dec;
    temp = (bcd_code >> 4) * 10;
    dec = (bcd_code & 0x0f) + temp;
    return dec;
}
uint8_t dec_to_bcd(uint8_t dec)
{
    uint8_t temp, temp2, bcd;
    temp = dec / 10;
    temp2 = dec % 10;

    bcd =  (temp << 4) + temp2;

    return bcd;
}
double ebox_round(float number, int digits)
{
    double out ;
    int rate = 1;
    for (uint8_t i = 0; i < digits; ++i)
        rate *= 10;

    number = number*rate + 0.5;
    long long int_part = (long long)number;
    out = int_part / (double)rate;
    return out;
}



/** @defgroup common����
  * @{
  */
/**
 *@brief    Gpio����ʱ���������һ��u8���͵����ݣ����ڴ���ͨ�š�
 *@param    data_pin��  ������������
            clock_pin:  ʱ���źŵ�����
            bit_order:  ���ݴ�С�˿���LSB_FIRST����λ�ȷ��ͣ�MSB_FIRST��λ�ȷ���
 *@retval   ���յ�������
*/
uint8_t shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order)
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i)
    {
        clock_pin->write(HIGH);
        if (bit_order == LSB_FIRST)
            value |= data_pin->read() << i;
        else
            value |= data_pin->read() << (7 - i);
        clock_pin->write(LOW);
    }
    return value;
}


/**
 * @brief   Gpio����ʱ���������һ��u8���͵����ݣ����ڴ���ͨ�š�
 * @param   data_pin��  �����������
 *          clock_pin:  ʱ���źŵ�����
 *          bit_order:  ���ݴ�С�˿���LSB_FIRST����λ�ȷ��ͣ�MSB_FIRST��λ�ȷ���
 *          val��       Ҫ���͵�����
 * @retval  NONE
 */
void shift_out(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order, uint8_t val)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        if (bit_order == LSB_FIRST)
            data_pin->write(!!(val & (1 << i)));
        else
            data_pin->write(!!(val & (1 << (7 - i))));

        clock_pin->write(HIGH);
        clock_pin->write(LOW);

    }
}

/**
 * @brief
 *
 * @param
 *
 *
 *
 * @retval  NONE
 */
void random_seed(unsigned int seed)
{
    srand(seed);
}

/**
 * @brief   ����һ��0~65535�������
 * @param   NONE
 * @retval  0~65535��������
 */
unsigned int random()
{
    return rand();
}
/**
 * @brief   ����һ��С��ĳ��ֵ�������
 * @param   max:����������ֵ����ֵ�Ȳ��������ֵ��1.
 * @retval  ��������
 */
unsigned int random(unsigned int max)
{
    return (rand() % max);
}

/**
 * @name
 * @brief
 *
 * @param
 *
 *
 *
 * @retval  NONE
 */
unsigned int random(unsigned int min, unsigned int max)
{
    return (min + rand() % (max - min));
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
