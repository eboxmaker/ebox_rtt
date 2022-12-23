#ifndef      __BSP_NT35510_LCD_H
#define	     __BSP_NT35510_LCD_H


#include "stm32f4xx.h"
#include "./font/fonts.h"


/***************************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

ѡ��BANK1-BORSRAM3 ���� TFT����ַ��ΧΪ0X6800 0000 ~ 0X6BFF FFFF
FSMC_A0 ��LCD��DC(�Ĵ���/����ѡ��)��
�Ĵ�������ַ = 0X6C00 0000
RAM����ַ = 0X6D00 0000 = 0X6C00 0000+2^0*2 = 0X6800 0000 + 0X2 = 0X6800 0002
��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���  
****************************************************************************************/

/******************************* NT35510 ��ʾ���� FSMC �������� ***************************/
//FSMC_Bank1_NORSRAM����LCD��������ĵ�ַ
#define      FSMC_Addr_NT35510_CMD         ( ( uint32_t ) 0x68000000 )

//FSMC_Bank1_NORSRAM����LCD���ݲ����ĵ�ַ      
#define      FSMC_Addr_NT35510_DATA        ( ( uint32_t ) 0x68000002 )

//��Ƭѡ���ž�����NOR/SRAM��
#define      FSMC_Bank1_NORSRAMx           FSMC_Bank1_NORSRAM3


/******************************* NT35510 ��ʾ��8080ͨѶ���Ŷ��� ***************************/
/******�����ź���******/
#define      FSMC_AF                       GPIO_AF_FSMC
//Ƭѡ��ѡ��NOR/SRAM��
#define      NT35510_CS_CLK                RCC_AHB1Periph_GPIOG  
#define      NT35510_CS_PORT               GPIOG
#define      NT35510_CS_PIN                GPIO_Pin_10
#define      NT35510_CS_PinSource          GPIO_PinSource10

//DC���ţ�ʹ��FSMC�ĵ�ַ�źſ��ƣ������ž����˷���LCDʱʹ�õĵ�ַ
//PF0ΪFSMC_A0
#define      NT35510_DC_CLK                RCC_AHB1Periph_GPIOF  
#define      NT35510_DC_PORT               GPIOF
#define      NT35510_DC_PIN                GPIO_Pin_0
#define      NT35510_DC_PinSource          GPIO_PinSource0

//дʹ��
#define      NT35510_WR_CLK                RCC_AHB1Periph_GPIOD   
#define      NT35510_WR_PORT               GPIOD
#define      NT35510_WR_PIN                GPIO_Pin_5
#define      NT35510_WR_PinSource          GPIO_PinSource5

//��ʹ��
#define      NT35510_RD_CLK                RCC_AHB1Periph_GPIOD   
#define      NT35510_RD_PORT               GPIOD
#define      NT35510_RD_PIN                GPIO_Pin_4
#define      NT35510_RD_PinSource          GPIO_PinSource4

//��λ����
#define      NT35510_RST_CLK               RCC_AHB1Periph_GPIOF 
#define      NT35510_RST_PORT              GPIOF
#define      NT35510_RST_PIN               GPIO_Pin_11

//��������
#define      NT35510_BK_CLK                RCC_AHB1Periph_GPIOF   
#define      NT35510_BK_PORT               GPIOF
#define      NT35510_BK_PIN                GPIO_Pin_9

/********�����ź���***************/
#define      NT35510_D0_CLK                RCC_AHB1Periph_GPIOD   
#define      NT35510_D0_PORT               GPIOD
#define      NT35510_D0_PIN                GPIO_Pin_14
#define      NT35510_D0_PinSource          GPIO_PinSource14

#define      NT35510_D1_CLK                RCC_AHB1Periph_GPIOD   
#define      NT35510_D1_PORT               GPIOD
#define      NT35510_D1_PIN                GPIO_Pin_15
#define      NT35510_D1_PinSource          GPIO_PinSource15

#define      NT35510_D2_CLK                RCC_AHB1Periph_GPIOD   
#define      NT35510_D2_PORT               GPIOD
#define      NT35510_D2_PIN                GPIO_Pin_0
#define      NT35510_D2_PinSource          GPIO_PinSource0

#define      NT35510_D3_CLK                RCC_AHB1Periph_GPIOD  
#define      NT35510_D3_PORT               GPIOD
#define      NT35510_D3_PIN                GPIO_Pin_1
#define      NT35510_D3_PinSource          GPIO_PinSource1

#define      NT35510_D4_CLK                RCC_AHB1Periph_GPIOE   
#define      NT35510_D4_PORT               GPIOE
#define      NT35510_D4_PIN                GPIO_Pin_7
#define      NT35510_D4_PinSource          GPIO_PinSource7

#define      NT35510_D5_CLK                RCC_AHB1Periph_GPIOE   
#define      NT35510_D5_PORT               GPIOE
#define      NT35510_D5_PIN                GPIO_Pin_8
#define      NT35510_D5_PinSource          GPIO_PinSource8

#define      NT35510_D6_CLK                RCC_AHB1Periph_GPIOE   
#define      NT35510_D6_PORT               GPIOE
#define      NT35510_D6_PIN                GPIO_Pin_9
#define      NT35510_D6_PinSource          GPIO_PinSource9

#define      NT35510_D7_CLK                RCC_AHB1Periph_GPIOE  
#define      NT35510_D7_PORT               GPIOE
#define      NT35510_D7_PIN                GPIO_Pin_10
#define      NT35510_D7_PinSource          GPIO_PinSource10

#define      NT35510_D8_CLK                RCC_AHB1Periph_GPIOE   
#define      NT35510_D8_PORT               GPIOE
#define      NT35510_D8_PIN                GPIO_Pin_11
#define      NT35510_D8_PinSource          GPIO_PinSource11

#define      NT35510_D9_CLK                RCC_AHB1Periph_GPIOE   
#define      NT35510_D9_PORT               GPIOE
#define      NT35510_D9_PIN                GPIO_Pin_12
#define      NT35510_D9_PinSource          GPIO_PinSource12

#define      NT35510_D10_CLK                RCC_AHB1Periph_GPIOE   
#define      NT35510_D10_PORT               GPIOE
#define      NT35510_D10_PIN                GPIO_Pin_13
#define      NT35510_D10_PinSource          GPIO_PinSource13

#define      NT35510_D11_CLK                RCC_AHB1Periph_GPIOE   
#define      NT35510_D11_PORT               GPIOE
#define      NT35510_D11_PIN                GPIO_Pin_14
#define      NT35510_D11_PinSource          GPIO_PinSource14

#define      NT35510_D12_CLK                RCC_AHB1Periph_GPIOE   
#define      NT35510_D12_PORT               GPIOE
#define      NT35510_D12_PIN                GPIO_Pin_15
#define      NT35510_D12_PinSource          GPIO_PinSource15

#define      NT35510_D13_CLK                RCC_AHB1Periph_GPIOD   
#define      NT35510_D13_PORT               GPIOD
#define      NT35510_D13_PIN                GPIO_Pin_8
#define      NT35510_D13_PinSource          GPIO_PinSource8

#define      NT35510_D14_CLK                RCC_AHB1Periph_GPIOD   
#define      NT35510_D14_PORT               GPIOD
#define      NT35510_D14_PIN                GPIO_Pin_9
#define      NT35510_D14_PinSource          GPIO_PinSource9

#define      NT35510_D15_CLK                RCC_AHB1Periph_GPIOD   
#define      NT35510_D15_PORT               GPIOD
#define      NT35510_D15_PIN                GPIO_Pin_10
#define      NT35510_D15_PinSource          GPIO_PinSource10

/*************************************** ����Ԥ�� ******************************************/
#define      DEBUG_DELAY()               Delay(0x5000)

/***************************** ILI934 ��ʾ�������ʼ������������� ***************************/
#define      NT35510_DispWindow_X_Star		    0     //��ʼ���X����
#define      NT35510_DispWindow_Y_Star		    0     //��ʼ���Y����

#define 			NT35510_LESS_PIXEL	  		480			//Һ�����϶̷�������ؿ��
#define 			NT35510_MORE_PIXEL	 		854			//Һ�����ϳ���������ؿ��

//����Һ��ɨ�跽����仯��XY���ؿ��
//����NT35510_GramScan�������÷���ʱ���Զ�����
extern uint16_t LCD_X_LENGTH,LCD_Y_LENGTH; 

//Һ����ɨ��ģʽ
//������ѡֵΪ0-7
extern uint8_t LCD_SCAN_MODE;

/******************************* ���� ILI934 ��ʾ��������ɫ ********************************/
#define      BACKGROUND		                BLACK   //Ĭ�ϱ�����ɫ

#define      WHITE		 		                  0xFFFF	   //��ɫ
#define      BLACK                         0x0000	   //��ɫ 
#define      GREY                          0xF7DE	   //��ɫ 
#define      BLUE                          0x001F	   //��ɫ 
#define      BLUE2                         0x051F	   //ǳ��ɫ 
#define      RED                           0xF800	   //��ɫ 
#define      MAGENTA                       0xF81F	   //����ɫ�����ɫ 
#define      GREEN                         0x07E0	   //��ɫ 
#define      CYAN                          0x7FFF	   //����ɫ����ɫ 
#define      YELLOW                        0xFFE0	   //��ɫ 
#define      BRED                          0xF81F
#define      GRED                          0xFFE0
#define      GBLUE                         0x07FF



/******************************* ���� ILI934 �������� ********************************/
#define      CMD_SetCoordinateX		 		    0x2A	     //����X����
#define      CMD_SetCoordinateY		 		    0x2B	     //����Y����
#define      CMD_SetPixel		 		          0x2C	     //�������




/********************************** ���� ILI934 ���� ***************************************/
void                     NT35510_Init                    ( void );
void                     NT35510_Rst                     ( void );
void                     NT35510_BackLed_Control         ( FunctionalState enumState );
void                     NT35510_GramScan                ( uint8_t ucOtion );
void                     NT35510_OpenWindow              ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void                     NT35510_Clear                   ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );
void                     NT35510_SetPointPixel           ( uint16_t usX, uint16_t usY );
uint16_t                 NT35510_GetPointPixel           ( uint16_t usX , uint16_t usY );
void                     NT35510_DrawLine                ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 );
void                     NT35510_DrawRectangle           ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight,uint8_t ucFilled );
void                     NT35510_DrawCircle              ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint8_t ucFilled );
void                     NT35510_DispChar_EN             ( uint16_t usX, uint16_t usY, const char cChar );
void                     NT35510_DispStringLine_EN      ( uint16_t line, char * pStr );
void                     NT35510_DispString_EN      			( uint16_t usX, uint16_t usY, char * pStr );
void 											NT35510_DispString_EN_YDir 		(   uint16_t usX,uint16_t usY ,  char * pStr );

void 											LCD_SetFont											(sFONT *fonts);
sFONT 										*LCD_GetFont											(void);
void 											NT35510_ClearLine										(uint16_t Line);
void 											LCD_SetBackColor								(uint16_t Color);
void 											LCD_SetTextColor								(uint16_t Color)	;
void 											LCD_SetColors										(uint16_t TextColor, uint16_t BackColor);
void 											LCD_GetColors										(uint16_t *TextColor, uint16_t *BackColor);

#define 									LCD_ClearLine 						NT35510_ClearLine


#endif /* __BSP_NT35510_NT35510_H */


