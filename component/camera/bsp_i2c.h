#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

#ifdef __cplusplus
 extern "C" {
#endif
/****************************** Includes *****************************/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
/****************************** Defines *******************************/

#define I2C_OWN_ADDRESS           0x00
#define OV2640_DEVICE_ADDRESS     0x60

//���뼶��ʱ(��Ҫ��ʱ��֧��)��������дDelay��
#define Delay 		HAL_Delay

 
#define I2Cx_FLAG_TIMEOUT             ((uint32_t) 1000) //0x1100
#define I2Cx_LONG_TIMEOUT             ((uint32_t) (300 * I2Cx_FLAG_TIMEOUT)) //was300
 
 
/*���Ŷ���*/ 

#define SENSORS_I2C_SCL_GPIO_PORT         		GPIOB
#define SENSORS_I2C_SCL_GPIO_CLK_ENABLE()    	__GPIOB_CLK_ENABLE()
#define SENSORS_I2C_SCL_GPIO_PIN         		GPIO_PIN_8
 
#define SENSORS_I2C_SDA_GPIO_PORT         		GPIOB
#define SENSORS_I2C_SDA_GPIO_CLK_ENABLE()   	__GPIOB_CLK_ENABLE()
#define SENSORS_I2C_SDA_GPIO_PIN          		GPIO_PIN_9

#define SENSORS_I2C_AF                    		GPIO_AF4_I2C1

#define SENSORS_I2C              		  		I2C1
#define SENSORS_I2C_RCC_CLK_ENABLE()     		__HAL_RCC_I2C1_CLK_ENABLE()

#define SENSORS_I2C_FORCE_RESET()      			__HAL_RCC_I2C1_FORCE_RESET()
#define SENSORS_I2C_RELEASE_RESET()    			__HAL_RCC_I2C1_RELEASE_RESET()

/*��Ϣ���*/
#define I2C_DEBUG_ON         1
#define I2C_DEBUG_FUNC_ON    0

#define I2C_INFO(fmt,arg...)           rt_kprintf("<<-I2C-INFO->> "fmt"\n",##arg)
#define I2C_ERROR(fmt,arg...)          rt_kprintf("<<-I2C-ERROR->> "fmt"\n",##arg)
#define I2C_DEBUG(fmt,arg...)          do{\
                                          if(I2C_DEBUG_ON)\
                                          rt_kprintf("<<-I2C-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define I2C_DEBUG_FUNC()               do{\
                                         if(I2C_DEBUG_FUNC_ON)\
                                         rt_kprintf("<<-I2C-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


																			 
void I2CMaster_Init(void);
unsigned short Get_I2C_Retry(void);
																			 
HAL_StatusTypeDef Sensors_I2C_ReadRegister(unsigned char slave_addr,
                                       unsigned char reg_addr,
                                       unsigned short len, 
                                       unsigned char *data_ptr);
HAL_StatusTypeDef Sensors_I2C_WriteRegister(unsigned char slave_addr,
                                        unsigned char reg_addr,
                                        unsigned short len, 
                                        unsigned char *data_ptr);

uint8_t OV2640_WriteReg(uint16_t Addr, uint8_t Data);
uint8_t OV2640_ReadReg(uint16_t Addr);
                                        
                                        
#ifdef __cplusplus
}
#endif
#endif // __BSP_I2C_H__


