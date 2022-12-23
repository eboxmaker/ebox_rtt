/**
  ******************************************************************************
  * @file    i2c.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   i2c �������������������I2C������ͨѶ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "bsp_i2c.h"

  
I2C_HandleTypeDef I2C_Handle;					
/*******************************  Function ************************************/

/**
  * @brief  ��ʼ��I2C���ߣ�ʹ��I2Cǰ��Ҫ����
  * @param  ��
  * @retval ��
  */
void I2CMaster_Init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��I2Cxʱ�� */
	SENSORS_I2C_RCC_CLK_ENABLE();

	/* ʹ��I2C GPIO ʱ�� */
	SENSORS_I2C_SCL_GPIO_CLK_ENABLE();
	SENSORS_I2C_SDA_GPIO_CLK_ENABLE();

	/* ����I2Cx����: SCL ----------------------------------------*/
	GPIO_InitStructure.Pin =  SENSORS_I2C_SCL_GPIO_PIN; 
	GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull= GPIO_NOPULL;
	GPIO_InitStructure.Alternate=SENSORS_I2C_AF; 
	HAL_GPIO_Init(SENSORS_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

	/* ����I2Cx����: SDA ----------------------------------------*/
	GPIO_InitStructure.Pin = SENSORS_I2C_SDA_GPIO_PIN;  
	HAL_GPIO_Init(SENSORS_I2C_SDA_GPIO_PORT, &GPIO_InitStructure); 
	
	if(HAL_I2C_GetState(&I2C_Handle) == HAL_I2C_STATE_RESET)
	{	
		/* ǿ�Ƹ�λI2C����ʱ�� */  
		SENSORS_I2C_FORCE_RESET(); 

		/* �ͷ�I2C����ʱ�Ӹ�λ */  
		SENSORS_I2C_RELEASE_RESET(); 
		
    	/* I2C ���� */
		I2C_Handle.Instance = SENSORS_I2C;
    I2C_Handle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    I2C_Handle.Init.ClockSpeed      = 40000;
    I2C_Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2C_Handle.Init.DutyCycle       = I2C_DUTYCYCLE_2;
    I2C_Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2C_Handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    I2C_Handle.Init.OwnAddress1     = 0xFE ;
    I2C_Handle.Init.OwnAddress2     = 0; 
   
		/* ��ʼ��I2C */
		HAL_I2C_Init(&I2C_Handle);	
//		/* ʹ��ģ���˲��� */
//		HAL_I2CEx_AnalogFilter_Config(&I2C_Handle, I2C_ANALOGFILTER_ENABLE); 
	}
}
/**
  * @brief  Manages error callback by re-initializing I2C.
  * @param  Addr: I2C Address
  * @retval None
  */
static void I2Cx_Error(void)
{
	/* �ָ�I2C�Ĵ���ΪĬ��ֵ */
	HAL_I2C_DeInit(&I2C_Handle); 
	/* ���³�ʼ��I2C���� */
	I2CMaster_Init();
}
/**
  * @brief  дһ�ֽ����ݵ�OV2640�Ĵ���
  * @param  Addr: OV2640 �ļĴ�����ַ
  * @param  Data: Ҫд�������
  * @retval ����0��ʾд��������0xFF��ʾ����
  */
uint8_t OV2640_WriteReg(uint16_t Addr, uint8_t Data)
{
//  I2Cx_WriteMultiple(&I2C_Handle, OV2640_DEVICE_WRITE_ADDRESS, (uint16_t)Addr, I2C_MEMADD_SIZE_8BIT,(uint8_t*)&Data, 1);
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&I2C_Handle, OV2640_DEVICE_ADDRESS, (uint16_t)Addr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&Data, 1, 1000);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initiaize the I2C Bus */
    I2Cx_Error();
  }
  return status;
}

/**
  * @brief  ��OV2640�Ĵ����ж�ȡһ���ֽڵ�����
  * @param  Addr: �Ĵ�����ַ
  * @retval ���ض�ȡ�õ�����
  */
uint8_t OV2640_ReadReg(uint16_t Addr)
{
    uint8_t Data = 0;
//    I2Cx_ReadMultiple(&I2C_Handle, OV2640_DEVICE_READ_ADDRESS, Addr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&Data, 1);
    
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_I2C_Mem_Read(&I2C_Handle, OV2640_DEVICE_ADDRESS, (uint16_t)Addr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&Data, 1, 1000);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* I2C error occurred */
    I2Cx_Error();
  }
  /* return the read data */
    return Data;
}
