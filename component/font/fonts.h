#ifndef __FONT_H
#define __FONT_H       

#include "stm32f4xx.h"
#include "./font/fonts.h"


#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))
#define LINEY(x) ((x) * (((sFONT *)LCD_GetFont())->Width))

/** @defgroup FONTS_Exported_Types
  * @{
  */ 
typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
  
} sFONT;


extern sFONT Font24x48;
extern sFONT Font16x32;
extern sFONT Font8x16;

//Ҫ֧��������Ҫʵ�ֱ��������ɲο���Һ����ʾ��Ӣ�ģ��ֿ����ⲿFLASH��������
#define      GetGBKCode( ucBuffer, usChar ) 


/*******************����********** ����ʾ������ʾ���ַ���С ***************************/
#define      WIDTH_CH_CHAR		                32	    //�����ַ���� 
#define      HEIGHT_CH_CHAR		              	32		  //�����ַ��߶� 

#endif /*end of __FONT_H    */
