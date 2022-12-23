/**
  ******************************************************************************
  * @file    bsp_NT35510_lcd.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   NT35510Һ��������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./lcd/bsp_NT35510_lcd.h"
#include "./font/fonts.h"	

//����Һ��ɨ�跽����仯��XY���ؿ��
//����NT35510_GramScan�������÷���ʱ���Զ�����
uint16_t LCD_X_LENGTH = NT35510_MORE_PIXEL;
uint16_t LCD_Y_LENGTH = NT35510_LESS_PIXEL;

//Һ����ɨ��ģʽ����������Ҫ���ڷ���ѡ�������ļ������
//������ѡֵΪ0-7
//����NT35510_GramScan�������÷���ʱ���Զ�����
//LCD�ճ�ʼ�����ʱ��ʹ�ñ�Ĭ��ֵ
uint8_t LCD_SCAN_MODE =6;


static sFONT *LCD_Currentfonts = &Font16x32;  //Ӣ������
static uint16_t CurrentTextColor   = WHITE;//ǰ��ɫ
static uint16_t CurrentBackColor   = BLACK;//����ɫ

__inline void                 NT35510_Write_Cmd           ( uint16_t usCmd );
__inline void                 NT35510_Write_Data          ( uint16_t usData );
__inline uint16_t             NT35510_Read_Data           ( void );
static void                   NT35510_Delay               ( __IO uint32_t nCount );
static void                   NT35510_GPIO_Config         ( void );
static void                   NT35510_FSMC_Config         ( void );
static void                   NT35510_REG_Config          ( void );
static void                   NT35510_SetCursor           ( uint16_t usX, uint16_t usY );
static __inline void          NT35510_FillColor           ( uint32_t ulAmout_Point, uint16_t usColor );
static uint16_t               NT35510_Read_PixelData      ( void );


/**
  * @brief  ����ʱ����
  * @param  nCount ����ʱ����ֵ
  * @retval ��
  */	
static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}

/**
  * @brief  ��NT35510д������
  * @param  usCmd :Ҫд��������Ĵ�����ַ��
  * @retval ��
  */	
__inline void NT35510_Write_Cmd ( uint16_t usCmd )
{
	* ( __IO uint16_t * ) ( FSMC_Addr_NT35510_CMD ) = usCmd;
	
}


/**
  * @brief  ��NT35510д������
  * @param  usData :Ҫд�������
  * @retval ��
  */	
__inline void NT35510_Write_Data ( uint16_t usData )
{
	* ( __IO uint16_t * ) ( FSMC_Addr_NT35510_DATA ) = usData;
	
}


/**
  * @brief  ��NT35510��ȡ����
  * @param  ��
  * @retval ��ȡ��������
  */	
__inline uint16_t NT35510_Read_Data ( void )
{
	return ( * ( __IO uint16_t * ) ( FSMC_Addr_NT35510_DATA ) );
	
}


/**
  * @brief  ���� NT35510 ����ʱ����
  * @param  nCount ����ʱ����ֵ
  * @retval ��
  */	
static void NT35510_Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}


/**
  * @brief  ��ʼ��NT35510��IO����
  * @param  ��
  * @retval ��
  */
static void NT35510_GPIO_Config ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��*/
	RCC_AHB1PeriphClockCmd ( 	
													/*�����ź�*/
													NT35510_CS_CLK|NT35510_DC_CLK|NT35510_WR_CLK|
													NT35510_RD_CLK	|NT35510_BK_CLK|NT35510_RST_CLK|
													/*�����ź�*/
													NT35510_D0_CLK|NT35510_D1_CLK|	NT35510_D2_CLK | 
													NT35510_D3_CLK | NT35510_D4_CLK|NT35510_D5_CLK|
													NT35510_D6_CLK | NT35510_D7_CLK|NT35510_D8_CLK|
													NT35510_D9_CLK | NT35510_D10_CLK|NT35510_D11_CLK|
													NT35510_D12_CLK | NT35510_D13_CLK|NT35510_D14_CLK|
													NT35510_D15_CLK	, ENABLE );
		
	
	/* ����FSMC���Ӧ��������,FSMC-D0~D15 */	
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = NT35510_D0_PIN; 
    GPIO_Init(NT35510_D0_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D0_PORT,NT35510_D0_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D1_PIN; 
    GPIO_Init(NT35510_D1_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D1_PORT,NT35510_D1_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D2_PIN; 
    GPIO_Init(NT35510_D2_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D2_PORT,NT35510_D2_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D3_PIN; 
    GPIO_Init(NT35510_D3_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D3_PORT,NT35510_D3_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D4_PIN; 
    GPIO_Init(NT35510_D4_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D4_PORT,NT35510_D4_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D5_PIN; 
    GPIO_Init(NT35510_D5_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D5_PORT,NT35510_D5_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D6_PIN; 
    GPIO_Init(NT35510_D6_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D6_PORT,NT35510_D6_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D7_PIN; 
    GPIO_Init(NT35510_D7_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D7_PORT,NT35510_D7_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D8_PIN; 
    GPIO_Init(NT35510_D8_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D8_PORT,NT35510_D8_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D9_PIN; 
    GPIO_Init(NT35510_D9_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D9_PORT,NT35510_D9_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D10_PIN; 
    GPIO_Init(NT35510_D10_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D10_PORT,NT35510_D10_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D11_PIN; 
    GPIO_Init(NT35510_D11_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D11_PORT,NT35510_D11_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D12_PIN; 
    GPIO_Init(NT35510_D12_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D12_PORT,NT35510_D12_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D13_PIN; 
    GPIO_Init(NT35510_D13_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D13_PORT,NT35510_D13_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D14_PIN; 
    GPIO_Init(NT35510_D14_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D14_PORT,NT35510_D14_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_D15_PIN; 
    GPIO_Init(NT35510_D15_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_D15_PORT,NT35510_D15_PinSource,FSMC_AF);

	/* ����FSMC���Ӧ�Ŀ�����
	 * FSMC_NOE   :LCD-RD
	 * FSMC_NWE   :LCD-WR
	 * FSMC_NE1   :LCD-CS
	 * FSMC_A0    :LCD-DC
	 */
    GPIO_InitStructure.GPIO_Pin = NT35510_RD_PIN; 
    GPIO_Init(NT35510_RD_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_RD_PORT,NT35510_RD_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_WR_PIN; 
    GPIO_Init(NT35510_WR_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_WR_PORT,NT35510_WR_PinSource,FSMC_AF);

    GPIO_InitStructure.GPIO_Pin = NT35510_CS_PIN; 
    GPIO_Init(NT35510_CS_PORT, &GPIO_InitStructure);   
    GPIO_PinAFConfig(NT35510_CS_PORT,NT35510_CS_PinSource,FSMC_AF);  

    GPIO_InitStructure.GPIO_Pin = NT35510_DC_PIN; 
    GPIO_Init(NT35510_DC_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(NT35510_DC_PORT,NT35510_DC_PinSource,FSMC_AF);
	

  /* ����LCD��λRST���ƹܽ�*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = NT35510_RST_PIN; 
	GPIO_Init ( NT35510_RST_PORT, & GPIO_InitStructure );
		
	/* ����LCD������ƹܽ�BK*/
	GPIO_InitStructure.GPIO_Pin = NT35510_BK_PIN; 
	GPIO_Init ( NT35510_BK_PORT, & GPIO_InitStructure );

}


 /**
  * @brief  LCD  FSMC ģʽ����
  * @param  ��
  * @retval ��
  */
static void NT35510_FSMC_Config ( void )
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 	
	
	/* ʹ��FSMCʱ��*/
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);

	//��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK 3/168M=18ns
	readWriteTiming.FSMC_AddressSetupTime      = 0x02;	 //��ַ����ʱ��
	//���ݱ���ʱ�䣨DATAST��+ 1��HCLK = 3/168M=18ns	
	readWriteTiming.FSMC_DataSetupTime         = 0x02;	 //���ݽ���ʱ��
	//ѡ����Ƶ�ģʽ
	//ģʽB,�첽NOR FLASHģʽ����NT35510��8080ʱ��ƥ��
	readWriteTiming.FSMC_AccessMode            = FSMC_AccessMode_B;	
	
	/*����������ģʽB�޹�*/
	//��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�
	readWriteTiming.FSMC_AddressHoldTime       = 0x00;	 //��ַ����ʱ��
	//��������ת�����ڣ������ڸ���ģʽ��NOR����
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	//����ʱ�ӷ�Ƶ��������ͬ�����͵Ĵ洢��
	readWriteTiming.FSMC_CLKDivision           = 0x00;
	//���ݱ���ʱ�䣬������ͬ���͵�NOR	
	readWriteTiming.FSMC_DataLatency           = 0x00;	

	
	FSMC_NORSRAMInitStructure.FSMC_Bank                  = FSMC_Bank1_NORSRAMx;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux        = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType            = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth       = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode       = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity    = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode              = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive      = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation        = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal            = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode          = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst            = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct     = &readWriteTiming;  
	
	FSMC_NORSRAMInit ( & FSMC_NORSRAMInitStructure ); 
	
	
	/* ʹ�� FSMC_Bank1_NORSRAM3 */
	FSMC_NORSRAMCmd ( FSMC_Bank1_NORSRAMx, ENABLE );  
		
		
}


/**
 * @brief  ��ʼ��NT35510�Ĵ���
 * @param  ��
 * @retval ��
 */
/**
 * @brief  ��ʼ��NT35510�Ĵ���
 * @param  ��
 * @retval ��
 */
static void NT35510_REG_Config ( void )
{	
	//Һ�������ṩ�����ְ汾����Ļ������һ�������ǵ�����������������
	//������ͨ��#if #else #endif�����ã�����Ļ��ʾ���������#if 0�ĳ�#if 1����1�ĳ�0
	//��2017-12-14�պ���Ĳ�Ʒ��ʹ��#if 0
#if 0
	//�ɰ�
	/* EXTC Command Set enable register */
	DEBUG_DELAY  ();
	NT35510_Write_Cmd ( 0xFF  );
	NT35510_Write_Data ( 0xFF  );
	NT35510_Write_Data ( 0x98  );
	NT35510_Write_Data ( 0x06  );

	/* GIP 1(BCh)  */
	DEBUG_DELAY ();
	NT35510_Write_Cmd(0xBC);
	NT35510_Write_Data(0x01); 
	NT35510_Write_Data(0x0E); 
	NT35510_Write_Data(0x61); 
	NT35510_Write_Data(0xFB); 
	NT35510_Write_Data(0x10); 
	NT35510_Write_Data(0x10); 
	NT35510_Write_Data(0x0B); 
	NT35510_Write_Data(0x0F); 
	NT35510_Write_Data(0x2E); 
	NT35510_Write_Data(0x73); 
	NT35510_Write_Data(0xFF); 
	NT35510_Write_Data(0xFF); 
	NT35510_Write_Data(0x0E); 
	NT35510_Write_Data(0x0E); 
	NT35510_Write_Data(0x00); 
	NT35510_Write_Data(0x03); 
	NT35510_Write_Data(0x66); 
	NT35510_Write_Data(0x63); 
	NT35510_Write_Data(0x01); 
	NT35510_Write_Data(0x00); 
	NT35510_Write_Data(0x00);

	/* GIP 2 (BDh) */
	DEBUG_DELAY ();
	NT35510_Write_Cmd(0xBD);
	NT35510_Write_Data(0x01); 
	NT35510_Write_Data(0x23); 
	NT35510_Write_Data(0x45); 
	NT35510_Write_Data(0x67); 
	NT35510_Write_Data(0x01); 
	NT35510_Write_Data(0x23); 
	NT35510_Write_Data(0x45); 
	NT35510_Write_Data(0x67); 

	/* GIP 3 (BEh) */
	DEBUG_DELAY ();
	NT35510_Write_Cmd(0xBE);
	NT35510_Write_Data(0x00); 
	NT35510_Write_Data(0x21); 
	NT35510_Write_Data(0xAB); 
	NT35510_Write_Data(0x60); 
	NT35510_Write_Data(0x22); 
	NT35510_Write_Data(0x22); 
	NT35510_Write_Data(0x22); 
	NT35510_Write_Data(0x22); 
	NT35510_Write_Data(0x22); 

	/* Vcom  (C7h) */
	DEBUG_DELAY ();
	NT35510_Write_Cmd ( 0xC7 );
	NT35510_Write_Data ( 0x6F );

	/* EN_volt_reg (EDh)*/
	DEBUG_DELAY ();
	NT35510_Write_Cmd ( 0xED );
	NT35510_Write_Data ( 0x7F );
	NT35510_Write_Data ( 0x0F );
	NT35510_Write_Data ( 0x00 );

	/* Power Control 1 (C0h) */
	DEBUG_DELAY ();
	NT35510_Write_Cmd ( 0xC0 );
	NT35510_Write_Data ( 0x37 );
	NT35510_Write_Data ( 0x0B );
	NT35510_Write_Data ( 0x0A );

	/* LVGL (FCh) */
	DEBUG_DELAY ();
	NT35510_Write_Cmd ( 0xFC );
	NT35510_Write_Data ( 0x0A );

	/* Engineering Setting (DFh) */
	DEBUG_DELAY ();
	NT35510_Write_Cmd ( 0xDF );
	NT35510_Write_Data ( 0x00 );
	NT35510_Write_Data ( 0x00 );
	NT35510_Write_Data ( 0x00 );
	NT35510_Write_Data ( 0x00 );
	NT35510_Write_Data ( 0x00 );
	NT35510_Write_Data ( 0x20 );

	/* DVDD Voltage Setting(F3h) */
	DEBUG_DELAY ();
	NT35510_Write_Cmd ( 0xF3 );
	NT35510_Write_Data ( 0x74 );

	/* Display Inversion Control (B4h) */
	NT35510_Write_Cmd ( 0xB4 );
	NT35510_Write_Data ( 0x00 );
	NT35510_Write_Data ( 0x00 );
	NT35510_Write_Data ( 0x00 );

	/* 480x854 (F7h)  */
	NT35510_Write_Cmd ( 0xF7 );
	NT35510_Write_Data ( 0x89 );

	/* Frame Rate (B1h) */
	NT35510_Write_Cmd ( 0xB1 );
	NT35510_Write_Data ( 0x00 );
	NT35510_Write_Data ( 0x12 );
	NT35510_Write_Data ( 0x10 );

	/* Panel Timing Control (F2h) */
	NT35510_Write_Cmd ( 0xF2 );
	NT35510_Write_Data ( 0x80 );
	NT35510_Write_Data ( 0x5B );
	NT35510_Write_Data ( 0x40 );
	NT35510_Write_Data ( 0x28 );

	DEBUG_DELAY ();

	/* Power Control 2 (C1h) */
	NT35510_Write_Cmd ( 0xC1 ); 
	NT35510_Write_Data ( 0x17 );
	NT35510_Write_Data ( 0x7D );
	NT35510_Write_Data ( 0x7A );
	NT35510_Write_Data ( 0x20 );

	DEBUG_DELAY ();

	NT35510_Write_Cmd(0xE0); 
	NT35510_Write_Data(0x00); //P1 
	NT35510_Write_Data(0x11); //P2 
	NT35510_Write_Data(0x1C); //P3 
	NT35510_Write_Data(0x0E); //P4 
	NT35510_Write_Data(0x0F); //P5 
	NT35510_Write_Data(0x0C); //P6 
	NT35510_Write_Data(0xC7); //P7 
	NT35510_Write_Data(0x06); //P8 
	NT35510_Write_Data(0x06); //P9 
	NT35510_Write_Data(0x0A); //P10 
	NT35510_Write_Data(0x10); //P11 
	NT35510_Write_Data(0x12); //P12 
	NT35510_Write_Data(0x0A); //P13 
	NT35510_Write_Data(0x10); //P14 
	NT35510_Write_Data(0x02); //P15 
	NT35510_Write_Data(0x00); //P16 

	DEBUG_DELAY ();

	NT35510_Write_Cmd(0xE1); 
	NT35510_Write_Data(0x00); //P1 
	NT35510_Write_Data(0x12); //P2 
	NT35510_Write_Data(0x18); //P3 
	NT35510_Write_Data(0x0C); //P4 
	NT35510_Write_Data(0x0F); //P5 
	NT35510_Write_Data(0x0A); //P6 
	NT35510_Write_Data(0x77); //P7 
	NT35510_Write_Data(0x06); //P8 
	NT35510_Write_Data(0x07); //P9 
	NT35510_Write_Data(0x0A); //P10 
	NT35510_Write_Data(0x0E); //P11 
	NT35510_Write_Data(0x0B); //P12 
	NT35510_Write_Data(0x10); //P13 
	NT35510_Write_Data(0x1D); //P14 
	NT35510_Write_Data(0x17); //P15 
	NT35510_Write_Data(0x00); //P16  

	/* Tearing Effect ON (35h)  */
	NT35510_Write_Cmd ( 0x35 );
	NT35510_Write_Data ( 0x00 );

	NT35510_Write_Cmd ( 0x3A );
	NT35510_Write_Data ( 0x55 );

	NT35510_Write_Cmd ( 0x11 );
	DEBUG_DELAY ();
	NT35510_Write_Cmd ( 0x29 );
	
#else
	//�°�
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xFF);
	NT35510_Write_Data(0xFF);
	NT35510_Write_Data(0x98);
	NT35510_Write_Data(0x06);
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xBA);
	NT35510_Write_Data(0x60);  
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xBC);
	NT35510_Write_Data(0x03);
	NT35510_Write_Data(0x0E);
	NT35510_Write_Data(0x61);
	NT35510_Write_Data(0xFF);
	NT35510_Write_Data(0x05);
	NT35510_Write_Data(0x05);
	NT35510_Write_Data(0x1B);
	NT35510_Write_Data(0x10);
	NT35510_Write_Data(0x73);
	NT35510_Write_Data(0x63);
	NT35510_Write_Data(0xFF);
	NT35510_Write_Data(0xFF);
	NT35510_Write_Data(0x05);
	NT35510_Write_Data(0x05);
	NT35510_Write_Data(0x00);
	NT35510_Write_Data(0x00);
	NT35510_Write_Data(0xD5);
	NT35510_Write_Data(0xD0);
	NT35510_Write_Data(0x01);
	NT35510_Write_Data(0x00);
	NT35510_Write_Data(0x40); 
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xBD);
	NT35510_Write_Data(0x01);
	NT35510_Write_Data(0x23);
	NT35510_Write_Data(0x45);
	NT35510_Write_Data(0x67);
	NT35510_Write_Data(0x01);
	NT35510_Write_Data(0x23);
	NT35510_Write_Data(0x45);
	NT35510_Write_Data(0x67);  
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xBE);
	NT35510_Write_Data(0x01);
	NT35510_Write_Data(0x2D);
	NT35510_Write_Data(0xCB);
	NT35510_Write_Data(0xA2);
	NT35510_Write_Data(0x62);
	NT35510_Write_Data(0xF2);
	NT35510_Write_Data(0xE2);
	NT35510_Write_Data(0x22);
	NT35510_Write_Data(0x22);
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xC7);
	NT35510_Write_Data(0x63); 
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xED);
	NT35510_Write_Data(0x7F);
	NT35510_Write_Data(0x0F);
	NT35510_Write_Data(0x00);
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xC0);
	NT35510_Write_Data(0x03);
	NT35510_Write_Data(0x0B);
	NT35510_Write_Data(0x00);   
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xFC);
	NT35510_Write_Data(0x08); 
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xDF);
	NT35510_Write_Data(0x00);
	NT35510_Write_Data(0x00);
	NT35510_Write_Data(0x00);
	NT35510_Write_Data(0x00);
	NT35510_Write_Data(0x00);
	NT35510_Write_Data(0x20);
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xF3);
	NT35510_Write_Data(0x74);
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xF9);
	NT35510_Write_Data(0x00);
	NT35510_Write_Data(0xFD);
	NT35510_Write_Data(0x80);
	NT35510_Write_Data(0x80);
	NT35510_Write_Data(0xC0);
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xB4);
	NT35510_Write_Data(0x02);
	NT35510_Write_Data(0x02);
	NT35510_Write_Data(0x02); 
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xF7);
	NT35510_Write_Data(0x81);
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xB1);
	NT35510_Write_Data(0x00);
	NT35510_Write_Data(0x13);
	NT35510_Write_Data(0x13); 
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xF2);
	NT35510_Write_Data(0xC0);
	NT35510_Write_Data(0x02);
	NT35510_Write_Data(0x40);
	NT35510_Write_Data(0x28);  
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xC1);
	NT35510_Write_Data(0x17);
	NT35510_Write_Data(0x75);
	NT35510_Write_Data(0x75);
	NT35510_Write_Data(0x20); 
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xE0);
	NT35510_Write_Data(0x00);
	NT35510_Write_Data(0x05);
	NT35510_Write_Data(0x08);
	NT35510_Write_Data(0x0C);
	NT35510_Write_Data(0x0F);
	NT35510_Write_Data(0x15);
	NT35510_Write_Data(0x09);
	NT35510_Write_Data(0x07);
	NT35510_Write_Data(0x01);
	NT35510_Write_Data(0x06);
	NT35510_Write_Data(0x09);
	NT35510_Write_Data(0x16);
	NT35510_Write_Data(0x14);
	NT35510_Write_Data(0x3E);
	NT35510_Write_Data(0x3E);
	NT35510_Write_Data(0x00);
	DEBUG_DELAY();
	NT35510_Write_Cmd(0xE1);
	NT35510_Write_Data(0x00);
	NT35510_Write_Data(0x09);
	NT35510_Write_Data(0x12);
	NT35510_Write_Data(0x12);
	NT35510_Write_Data(0x13);
	NT35510_Write_Data(0x1c);
	NT35510_Write_Data(0x07);
	NT35510_Write_Data(0x08);
	NT35510_Write_Data(0x05);
	NT35510_Write_Data(0x08);
	NT35510_Write_Data(0x03);
	NT35510_Write_Data(0x02);
	NT35510_Write_Data(0x04);
	NT35510_Write_Data(0x1E);
	NT35510_Write_Data(0x1B);
	NT35510_Write_Data(0x00);
	DEBUG_DELAY();
	NT35510_Write_Cmd(0x3A);
	NT35510_Write_Data(0x55); 
	DEBUG_DELAY();
	NT35510_Write_Cmd(0x35);
	NT35510_Write_Data(0x00); 
	DEBUG_DELAY();
	NT35510_Write_Cmd(0x11);
	DEBUG_DELAY() ;
	NT35510_Write_Cmd(0x29);	   
	DEBUG_DELAY()  ; 
#endif	
}


/**
 * @brief  NT35510��ʼ�����������Ҫ�õ�lcd��һ��Ҫ�����������
 * @param  ��
 * @retval ��
 */
void NT35510_Init ( void )
{
	NT35510_GPIO_Config ();
	NT35510_FSMC_Config ();
	
	
	NT35510_Rst ();
	NT35510_REG_Config ();
	
	//����Ĭ��ɨ�跽������ 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����  
	NT35510_GramScan(LCD_SCAN_MODE);
    
    NT35510_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
    NT35510_BackLed_Control ( ENABLE );      //����LCD�����
}


/**
 * @brief  NT35510����LED����
 * @param  enumState �������Ƿ�ʹ�ܱ���LED
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg ENABLE :ʹ�ܱ���LED
  *     @arg DISABLE :���ñ���LED
 * @retval ��
 */
void NT35510_BackLed_Control ( FunctionalState enumState )
{
	if ( enumState )
		 GPIO_SetBits( NT35510_BK_PORT, NT35510_BK_PIN );	
	else
		 GPIO_ResetBits( NT35510_BK_PORT, NT35510_BK_PIN );
		
}



/**
 * @brief  NT35510 �����λ
 * @param  ��
 * @retval ��
 */
void NT35510_Rst ( void )
{			
	GPIO_ResetBits ( NT35510_RST_PORT, NT35510_RST_PIN );	 //�͵�ƽ��λ

	NT35510_Delay ( 0xAFF ); 					   

	GPIO_SetBits ( NT35510_RST_PORT, NT35510_RST_PIN );		 	 

	NT35510_Delay ( 0xAFF ); 	
	
}




/**
 * @brief  ����NT35510��GRAM��ɨ�跽�� 
 * @param  ucOption ��ѡ��GRAM��ɨ�跽�� 
 *     @arg 0-7 :������ѡֵΪ0-7��˸�����
 *
 *	����������0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�
 *				���Ƽ�ʹ������ģʽ��ʾ����	����ģʽ��ʾ���ֻ��о���Ч��			
 *		
 *	����0��2��4��6 ģʽ��X��������Ϊ480��Y��������Ϊ854
 *	����1��3��5��7 ģʽ��X��������Ϊ854��Y��������Ϊ480
 *
 *	���� 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����
 *	���� 3 ģʽΪ����ͷ����ʹ�õķ���
 *	���� 0 ģʽΪBMPͼƬ��ʾ����ʹ�õķ���
 *
 * @retval ��
 * @note  ����ͼ����A��ʾ���ϣ�V��ʾ���£�<��ʾ����>��ʾ����
					X��ʾX�ᣬY��ʾY��

------------------------------------------------------------
ģʽ0��				.		ģʽ1��		.	ģʽ2��			.	ģʽ3��					
					A		.					A		.		A					.		A									
					|		.					|		.		|					.		|							
					Y		.					X		.		Y					.		X					
					0		.					1		.		2					.		3					
	<--- X0 o		.	<----Y1	o		.		o 2X--->  .		o 3Y--->	
------------------------------------------------------------	
ģʽ4��				.	ģʽ5��			.	ģʽ6��			.	ģʽ7��					
	<--- X4 o		.	<--- Y5 o		.		o 6X--->  .		o 7Y--->	
					4		.					5		.		6					.		7	
					Y		.					X		.		Y					.		X						
					|		.					|		.		|					.		|							
					V		.					V		.		V					.		V		
---------------------------------------------------------				
											 LCD��ʾ��
								|-----------------|
								|			Ұ��Logo		|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|-----------------|
								��Ļ���棨��480����854��

 *******************************************************/
void NT35510_GramScan ( uint8_t ucOption )
{	
	//������飬ֻ������0-7
	if(ucOption >7 )
		return;
	
	//����ģʽ����LCD_SCAN_MODE��ֵ����Ҫ���ڴ�����ѡ��������
	LCD_SCAN_MODE = ucOption;
	
	//����ģʽ����XY��������ؿ��
	if(ucOption%2 == 0)	
	{
		//0 2 4 6ģʽ��X�������ؿ��Ϊ480��Y����Ϊ854
		LCD_X_LENGTH = NT35510_LESS_PIXEL;
		LCD_Y_LENGTH =	NT35510_MORE_PIXEL;
	}
	else				
	{
		//1 3 5 7ģʽ��X�������ؿ��Ϊ854��Y����Ϊ480
		LCD_X_LENGTH = NT35510_MORE_PIXEL;
		LCD_Y_LENGTH =	NT35510_LESS_PIXEL; 
	}

	//0x36��������ĸ�3λ����������GRAMɨ�跽��	
	NT35510_Write_Cmd ( 0x36 ); 
	NT35510_Write_Data (0x00 | (ucOption<<5));//����ucOption��ֵ����LCD��������0-7��ģʽ
	NT35510_Write_Cmd ( CMD_SetCoordinateX ); 
	NT35510_Write_Data ( 0x00 );		/* x ��ʼ�����8λ */
	NT35510_Write_Data ( 0x00 );		/* x ��ʼ�����8λ */
	NT35510_Write_Data ( ((LCD_X_LENGTH-1)>>8)&0xFF ); /* x ���������8λ */	
	NT35510_Write_Data ( (LCD_X_LENGTH-1)&0xFF );				/* x ���������8λ */

	NT35510_Write_Cmd ( CMD_SetCoordinateY ); 
	NT35510_Write_Data ( 0x00 );		/* y ��ʼ�����8λ */
	NT35510_Write_Data ( 0x00 );		/* y ��ʼ�����8λ */
	NT35510_Write_Data ( ((LCD_Y_LENGTH-1)>>8)&0xFF );	/* y ���������8λ */	 
	NT35510_Write_Data ( (LCD_Y_LENGTH-1)&0xFF );				/* y ���������8λ */

	/* write gram start */
	NT35510_Write_Cmd ( CMD_SetPixel );	
}



/**
 * @brief  ��NT35510��ʾ���Ͽ���һ������
 * @param  usX �����ض�ɨ�跽���´��ڵ����X����
 * @param  usY �����ض�ɨ�跽���´��ڵ����Y����
 * @param  usWidth �����ڵĿ��
 * @param  usHeight �����ڵĸ߶�
 * @retval ��
 */
void NT35510_OpenWindow ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{	
	NT35510_Write_Cmd ( CMD_SetCoordinateX ); 				 /* ����X���� */
	NT35510_Write_Data ( usX >> 8  );	 /* �ȸ�8λ��Ȼ���8λ */
	NT35510_Write_Data ( usX & 0xff  );	 /* ������ʼ��ͽ�����*/
	NT35510_Write_Data ( ( usX + usWidth - 1 ) >> 8  );
	NT35510_Write_Data ( ( usX + usWidth - 1 ) & 0xff  );

	NT35510_Write_Cmd ( CMD_SetCoordinateY ); 			     /* ����Y����*/
	NT35510_Write_Data ( usY >> 8  );
	NT35510_Write_Data ( usY & 0xff  );
	NT35510_Write_Data ( ( usY + usHeight - 1 ) >> 8 );
	NT35510_Write_Data ( ( usY + usHeight - 1) & 0xff );
	
	NT35510_Write_Cmd ( CMD_SetPixel ); 			     /* д������*/

}


/**
 * @brief  �趨NT35510�Ĺ������
 * @param  usX �����ض�ɨ�跽���¹���X����
 * @param  usY �����ض�ɨ�跽���¹���Y����
 * @retval ��
 */
static void NT35510_SetCursor ( uint16_t usX, uint16_t usY )	
{
	NT35510_OpenWindow ( usX, usY, 1, 1 );
}


/**
 * @brief  ��NT35510��ʾ������ĳһ��ɫ������ص�
 * @param  ulAmout_Point ��Ҫ�����ɫ�����ص������Ŀ
 * @param  usColor ����ɫ
 * @retval ��
 */
static __inline void NT35510_FillColor ( uint32_t ulAmout_Point, uint16_t usColor )
{
	uint32_t i = 0;
	
	
	/* memory write */
	NT35510_Write_Cmd ( CMD_SetPixel );	
		
	for ( i = 0; i < ulAmout_Point; i ++ )
		NT35510_Write_Data ( usColor );
		
	
}


/**
 * @brief  ��NT35510��ʾ����ĳһ������ĳ����ɫ��������
 * @param  usX �����ض�ɨ�跽���´��ڵ����X����
 * @param  usY �����ض�ɨ�跽���´��ڵ����Y����
 * @param  usWidth �����ڵĿ��
 * @param  usHeight �����ڵĸ߶�
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void NT35510_Clear ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
{
	NT35510_OpenWindow ( usX, usY, usWidth, usHeight );

	NT35510_FillColor ( usWidth * usHeight, CurrentBackColor );		
	
}


/**
 * @brief  ��NT35510��ʾ����ĳһ����ĳ����ɫ�������
 * @param  usX �����ض�ɨ�跽���¸õ��X����
 * @param  usY �����ض�ɨ�跽���¸õ��Y����
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void NT35510_SetPointPixel ( uint16_t usX, uint16_t usY )	
{	
	if ( ( usX < LCD_X_LENGTH ) && ( usY < LCD_Y_LENGTH ) )
  {
		NT35510_SetCursor ( usX, usY );
		
		NT35510_FillColor ( 1, CurrentTextColor );
	}
	
}


/**
 * @brief  ��ȡNT35510 GRAN ��һ����������
 * @param  ��
 * @retval ��������
 */
static uint16_t NT35510_Read_PixelData ( void )	
{	
	uint16_t usR=0, usG=0, usB=0 ;

	
	NT35510_Write_Cmd ( 0x2E );   /* ������ */
	
	usR = NT35510_Read_Data (); 	/*FIRST READ OUT DUMMY DATA*/
	
	usR = NT35510_Read_Data ();  	/*READ OUT RED DATA  */
	usB = NT35510_Read_Data ();  	/*READ OUT BLUE DATA*/
	usG = NT35510_Read_Data ();  	/*READ OUT GREEN DATA*/	
	
  return ( ( ( usR >> 11 ) << 11 ) | ( ( usG >> 10 ) << 5 ) | ( usB >> 11 ) );
	
}


/**
 * @brief  ��ȡ NT35510 ��ʾ����ĳһ����������������
 * @param  usX �����ض�ɨ�跽���¸õ��X����
 * @param  usY �����ض�ɨ�跽���¸õ��Y����
 * @retval ��������
 */
uint16_t NT35510_GetPointPixel ( uint16_t usX, uint16_t usY )
{ 
	uint16_t usPixelData;

	
	NT35510_SetCursor ( usX, usY );
	
	usPixelData = NT35510_Read_PixelData ();
	
	return usPixelData;
	
}


/**
 * @brief  �� NT35510 ��ʾ����ʹ�� Bresenham �㷨���߶� 
 * @param  usX1 �����ض�ɨ�跽�����߶ε�һ���˵�X����
 * @param  usY1 �����ض�ɨ�跽�����߶ε�һ���˵�Y����
 * @param  usX2 �����ض�ɨ�跽�����߶ε���һ���˵�X����
 * @param  usY2 �����ض�ɨ�跽�����߶ε���һ���˵�Y����
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void NT35510_DrawLine ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 )
{
	uint16_t us; 
	uint16_t usX_Current, usY_Current;
	
	int32_t lError_X = 0, lError_Y = 0, lDelta_X, lDelta_Y, lDistance; 
	int32_t lIncrease_X, lIncrease_Y; 	
	
	
	lDelta_X = usX2 - usX1; //������������ 
	lDelta_Y = usY2 - usY1; 
	
	usX_Current = usX1; 
	usY_Current = usY1; 
	
	
	if ( lDelta_X > 0 ) 
		lIncrease_X = 1; //���õ������� 
	
	else if ( lDelta_X == 0 ) 
		lIncrease_X = 0;//��ֱ�� 
	
	else 
  { 
    lIncrease_X = -1;
    lDelta_X = - lDelta_X;
  } 

	
	if ( lDelta_Y > 0 )
		lIncrease_Y = 1; 
	
	else if ( lDelta_Y == 0 )
		lIncrease_Y = 0;//ˮƽ�� 
	
	else 
  {
    lIncrease_Y = -1;
    lDelta_Y = - lDelta_Y;
  } 

	
	if (  lDelta_X > lDelta_Y )
		lDistance = lDelta_X; //ѡȡ�������������� 
	
	else 
		lDistance = lDelta_Y; 

	
	for ( us = 0; us <= lDistance + 1; us ++ )//������� 
	{  
		NT35510_SetPointPixel ( usX_Current, usY_Current );//���� 
		
		lError_X += lDelta_X ; 
		lError_Y += lDelta_Y ; 
		
		if ( lError_X > lDistance ) 
		{ 
			lError_X -= lDistance; 
			usX_Current += lIncrease_X; 
		}  
		
		if ( lError_Y > lDistance ) 
		{ 
			lError_Y -= lDistance; 
			usY_Current += lIncrease_Y; 
		} 
		
	}  
	
	
}   


/**
 * @brief  �� NT35510 ��ʾ���ϻ�һ������
 * @param  usX_Start �����ض�ɨ�跽���¾��ε���ʼ��X����
 * @param  usY_Start �����ض�ɨ�跽���¾��ε���ʼ��Y����
 * @param  usWidth�����εĿ�ȣ���λ�����أ�
 * @param  usHeight�����εĸ߶ȣ���λ�����أ�
 * @param  ucFilled ��ѡ���Ƿ����þ���
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg 0 :���ľ���
  *     @arg 1 :ʵ�ľ��� 
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void NT35510_DrawRectangle ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint8_t ucFilled )
{
	if ( ucFilled )
	{
		NT35510_OpenWindow ( usX_Start, usY_Start, usWidth, usHeight );
		NT35510_FillColor ( usWidth * usHeight ,CurrentTextColor);	
	}
	else
	{
		NT35510_DrawLine ( usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start );
		NT35510_DrawLine ( usX_Start, usY_Start + usHeight - 1, usX_Start + usWidth - 1, usY_Start + usHeight - 1 );
		NT35510_DrawLine ( usX_Start, usY_Start, usX_Start, usY_Start + usHeight - 1 );
		NT35510_DrawLine ( usX_Start + usWidth - 1, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1 );		
	}

}


/**
 * @brief  �� NT35510 ��ʾ����ʹ�� Bresenham �㷨��Բ
 * @param  usX_Center �����ض�ɨ�跽����Բ�ĵ�X����
 * @param  usY_Center �����ض�ɨ�跽����Բ�ĵ�Y����
 * @param  usRadius��Բ�İ뾶����λ�����أ�
 * @param  ucFilled ��ѡ���Ƿ�����Բ
  *   �ò���Ϊ����ֵ֮һ��
  *     @arg 0 :����Բ
  *     @arg 1 :ʵ��Բ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void NT35510_DrawCircle ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint8_t ucFilled )
{
	int16_t sCurrentX, sCurrentY;
	int16_t sError;
	
	
	sCurrentX = 0; sCurrentY = usRadius;	  
	
	sError = 3 - ( usRadius << 1 );     //�ж��¸���λ�õı�־
	
	
	while ( sCurrentX <= sCurrentY )
	{
		int16_t sCountY;
		
		
		if ( ucFilled ) 			
			for ( sCountY = sCurrentX; sCountY <= sCurrentY; sCountY ++ ) 
			{                      
				NT35510_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCountY );           //1���о����� 
				NT35510_SetPointPixel ( usX_Center - sCurrentX, usY_Center + sCountY );           //2       
				NT35510_SetPointPixel ( usX_Center - sCountY,   usY_Center + sCurrentX );           //3
				NT35510_SetPointPixel ( usX_Center - sCountY,   usY_Center - sCurrentX );           //4
				NT35510_SetPointPixel ( usX_Center - sCurrentX, usY_Center - sCountY );           //5    
        NT35510_SetPointPixel ( usX_Center + sCurrentX, usY_Center - sCountY );           //6
				NT35510_SetPointPixel ( usX_Center + sCountY,   usY_Center - sCurrentX );           //7 	
        NT35510_SetPointPixel ( usX_Center + sCountY,   usY_Center + sCurrentX );           //0				
			}
		
		else
		{          
			NT35510_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCurrentY );             //1���о�����
			NT35510_SetPointPixel ( usX_Center - sCurrentX, usY_Center + sCurrentY );             //2      
			NT35510_SetPointPixel ( usX_Center - sCurrentY, usY_Center + sCurrentX );             //3
			NT35510_SetPointPixel ( usX_Center - sCurrentY, usY_Center - sCurrentX );             //4
			NT35510_SetPointPixel ( usX_Center - sCurrentX, usY_Center - sCurrentY );             //5       
			NT35510_SetPointPixel ( usX_Center + sCurrentX, usY_Center - sCurrentY );             //6
			NT35510_SetPointPixel ( usX_Center + sCurrentY, usY_Center - sCurrentX );             //7 
			NT35510_SetPointPixel ( usX_Center + sCurrentY, usY_Center + sCurrentX );             //0
    }			
		
		
		sCurrentX ++;

		
		if ( sError < 0 ) 
			sError += 4 * sCurrentX + 6;	  
		
		else
		{
			sError += 10 + 4 * ( sCurrentX - sCurrentY );   
			sCurrentY --;
		} 	
		
		
	}
	
	
}

/**
 * @brief  �� NT35510 ��ʾ������ʾһ��Ӣ���ַ�
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽���¸õ����ʼY����
 * @param  cChar ��Ҫ��ʾ��Ӣ���ַ�
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void NT35510_DispChar_EN ( uint16_t usX, uint16_t usY, const char cChar )
{
	uint8_t  byteCount, bitCount,fontLength;	
	uint16_t ucRelativePositon;
	uint8_t *Pfont;
	
	//��ascii���ƫ�ƣ���ģ������ASCII���ǰ32����ͼ�η��ţ�
	ucRelativePositon = cChar - ' ';
	
	//ÿ����ģ���ֽ���
	fontLength = (LCD_Currentfonts->Width*LCD_Currentfonts->Height)/8;
		
	//��ģ�׵�ַ
	/*ascii���ƫ��ֵ����ÿ����ģ���ֽ����������ģ��ƫ��λ��*/
	Pfont = (uint8_t *)&LCD_Currentfonts->table[ucRelativePositon * fontLength];
	
	//������ʾ����
	NT35510_OpenWindow ( usX, usY, LCD_Currentfonts->Width, LCD_Currentfonts->Height);
	
	NT35510_Write_Cmd ( CMD_SetPixel );			

	//���ֽڶ�ȡ��ģ����
	//����ǰ��ֱ����������ʾ���ڣ���ʾ���ݻ��Զ�����
	for ( byteCount = 0; byteCount < fontLength; byteCount++ )
	{
			//һλһλ����Ҫ��ʾ����ɫ
			for ( bitCount = 0; bitCount < 8; bitCount++ )
			{
					if ( Pfont[byteCount] & (0x80>>bitCount) )
						NT35510_Write_Data ( CurrentTextColor );			
					else
						NT35510_Write_Data ( CurrentBackColor );
			}	
	}	
}


/**
 * @brief  �� NT35510 ��ʾ������ʾӢ���ַ���
 * @param  line �����ض�ɨ�跽�����ַ�������ʼY����
  *   ��������ʹ�ú�LINE(0)��LINE(1)�ȷ�ʽָ���������꣬
  *   ��LINE(x)����ݵ�ǰѡ�������������Y����ֵ��
	*		��ʾ������ʹ��LINE��ʱ����Ҫ��Ӣ���������ó�Font8x16
 * @param  pStr ��Ҫ��ʾ��Ӣ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void NT35510_DispStringLine_EN (  uint16_t line,  char * pStr )
{
	uint16_t usX = 0;
	
	while ( * pStr != '\0' )
	{
		if ( ( usX - NT35510_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
		{
			usX = NT35510_DispWindow_X_Star;
			line += LCD_Currentfonts->Height;
		}
		
		if ( ( line - NT35510_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
		{
			usX = NT35510_DispWindow_X_Star;
			line = NT35510_DispWindow_Y_Star;
		}
		
		NT35510_DispChar_EN ( usX, line, * pStr);
		
		pStr ++;
		
		usX += LCD_Currentfonts->Width;
		
	}
	
}


/**
 * @brief  �� NT35510 ��ʾ������ʾӢ���ַ���
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽�����ַ�����ʼY����
 * @param  pStr ��Ҫ��ʾ��Ӣ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void NT35510_DispString_EN ( 	uint16_t usX ,uint16_t usY,  char * pStr )
{
	while ( * pStr != '\0' )
	{
		if ( ( usX - NT35510_DispWindow_X_Star + LCD_Currentfonts->Width ) > LCD_X_LENGTH )
		{
			usX = NT35510_DispWindow_X_Star;
			usY += LCD_Currentfonts->Height;
		}
		
		if ( ( usY - NT35510_DispWindow_Y_Star + LCD_Currentfonts->Height ) > LCD_Y_LENGTH )
		{
			usX = NT35510_DispWindow_X_Star;
			usY = NT35510_DispWindow_Y_Star;
		}
		
		NT35510_DispChar_EN ( usX, usY, * pStr);
		
		pStr ++;
		
		usX += LCD_Currentfonts->Width;
		
	}
	
}


/**
 * @brief  �� NT35510 ��ʾ������ʾӢ���ַ���(��Y�᷽��)
 * @param  usX �����ض�ɨ�跽�����ַ�����ʼX����
 * @param  usY �����ض�ɨ�跽�����ַ�����ʼY����
 * @param  pStr ��Ҫ��ʾ��Ӣ���ַ������׵�ַ
 * @note ��ʹ��LCD_SetBackColor��LCD_SetTextColor��LCD_SetColors����������ɫ
 * @retval ��
 */
void NT35510_DispString_EN_YDir (	 uint16_t usX,uint16_t usY ,  char * pStr )
{	
	while ( * pStr != '\0' )
	{
		if ( ( usY - NT35510_DispWindow_Y_Star + LCD_Currentfonts->Height ) >LCD_Y_LENGTH  )
		{
			usY = NT35510_DispWindow_Y_Star;
			usX += LCD_Currentfonts->Width;
		}
		
		if ( ( usX - NT35510_DispWindow_X_Star + LCD_Currentfonts->Width ) >  LCD_X_LENGTH)
		{
			usX = NT35510_DispWindow_X_Star;
			usY = NT35510_DispWindow_Y_Star;
		}
		
		NT35510_DispChar_EN ( usX, usY, * pStr);
		
		pStr ++;
		
		usY += LCD_Currentfonts->Height;		
	}	
}


/**
  * @brief  ����Ӣ����������
  * @param  fonts: ָ��Ҫѡ�������
	*		����Ϊ����ֵ֮һ
  * 	@arg��Font24x32;
  * 	@arg��Font16x24;
  * 	@arg��Font8x16;
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/**
  * @brief  ��ȡ��ǰ��������
  * @param  None.
  * @retval ���ص�ǰ��������
  */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}


/**
  * @brief  ����LCD��ǰ��(����)��������ɫ,RGB565
  * @param  TextColor: ָ��ǰ��(����)��ɫ
  * @param  BackColor: ָ��������ɫ
  * @retval None
  */
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor) 
{
  CurrentTextColor = TextColor; 
  CurrentBackColor = BackColor;
}

/**
  * @brief  ��ȡLCD��ǰ��(����)��������ɫ,RGB565
  * @param  TextColor: �����洢ǰ��(����)��ɫ��ָ�����
  * @param  BackColor: �����洢������ɫ��ָ�����
  * @retval None
  */
void LCD_GetColors(uint16_t *TextColor, uint16_t *BackColor)
{
  *TextColor = CurrentTextColor;
  *BackColor = CurrentBackColor;
}

/**
  * @brief  ����LCD��ǰ��(����)��ɫ,RGB565
  * @param  Color: ָ��ǰ��(����)��ɫ 
  * @retval None
  */
void LCD_SetTextColor(uint16_t Color)
{
  CurrentTextColor = Color;
}

/**
  * @brief  ����LCD�ı�����ɫ,RGB565
  * @param  Color: ָ��������ɫ 
  * @retval None
  */
void LCD_SetBackColor(uint16_t Color)
{
  CurrentBackColor = Color;
}

/**
  * @brief  ���ĳ������
  * @param  Line: ָ��Ҫɾ������
  *   ��������ʹ�ú�LINE(0)��LINE(1)�ȷ�ʽָ��Ҫɾ�����У�
  *   ��LINE(x)����ݵ�ǰѡ�������������Y����ֵ����ɾ����ǰ����߶ȵĵ�x�С�
  * @retval None
  */
void NT35510_ClearLine(uint16_t Line)
{
  NT35510_Clear(0,Line,LCD_X_LENGTH,((sFONT *)LCD_GetFont())->Height);	/* ��������ʾȫ�� */

}
/*********************end of file*************************/



