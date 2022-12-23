#include "low_power.h"
#include "bsp_rtc.h"
#include "config.h"
#include "bsp_ebox.h"
#include "power.h"
#include "modbus_addr.h"

uint32_t pwr_flag;



void pd_all()
{

 GPIO_InitTypeDef GPIO_InitStruct = {0};   
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  
    /*Configure GPIO pin : PC13 */
    GPIO_InitStruct.Pin = GPIO_PIN_All;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;//
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;//
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;//
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); 
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); 
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_All, GPIO_PIN_RESET);//??
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_All, GPIO_PIN_RESET);//??
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_RESET);//??
//    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_All, GPIO_PIN_RESET);//??
//    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_All, GPIO_PIN_RESET);//??

  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOD_CLK_DISABLE();
  __HAL_RCC_GPIOE_CLK_DISABLE();
}
uint32_t lowpower_flag_get()
{
    return pwr_flag;
}

void lowpower_standby_lock(uint8_t index)
{
    bitSet(pwr_flag,index);
    UART.printf("LOW POWER LOCK bit index:[%d,0X%02X]\n",index,pwr_flag);
}
void lowpower_standby_unlock(uint8_t index)
{
    bitClear(pwr_flag,index);
    UART.printf("LOW POWER UNLOCK bit index:[%d,0X%02X]\n",index,pwr_flag);
}

void lowpower_init()
{
	PA0.mode(INPUT_PD);
	PA3.mode(INPUT_PD);
}
void lowpower_standby_enter(uint8_t index)
{
    lowpower_standby_unlock(index);
//    stop_iwdg();
// 
//    /* Clear all related wakeup flags*/
//    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

//    /* Enable WakeUp Pin PWR_WAKEUP_PIN1 connected to PA.00 PA00*/
//    __HAL_RCC_LPUART1_CLK_DISABLE();

//    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0|GPIO_PIN_1);\
    
    if(pwr_flag != 0)
    {
        UART.printf("some [0X%04X] task is runing\n",pwr_flag);
        UART.flush();
        return ;
    }
//    power_off(PwrDigital);
//    delay_ms(100);
//    if(PA9.read() == 1)
//    {
//        UART.printf("!!!!!!enter stadnby failed PA9 is high \n");
//        power_off(PwrDigital);
//        set_sample_mode(sample_mode);
//        return;
//    }
    rtc_set_alarm_minute(config.sample_regu_interval);
    UART.printf("!!!!!!enter stadnby,wake up after: %d min\n",config.sample_regu_interval);
    UART.flush();
    power_off(PwrAll);
    delay_ms(100);
    __HAL_RCC_USART1_CLK_DISABLE();
    pd_all();
    delay_ms(100);

    delay_ms(2*1000);
//    delay_ms(10*1000);

    
    
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB); 
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF1);
//    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF3);
    
    
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN3);
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN3);
    HAL_PWR_EnterSTANDBYMode();    //??standby??
//    UART.printf("enter standby err\r\n");

}