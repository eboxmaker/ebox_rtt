#include "app3.h"
#include "lcd/bsp_nt35510_lcd.h"
#include "./camera/bsp_ov2640.h"
#include "./camera/bsp_i2c.h"


OV2640_IDTypeDef OV2640_Camera_ID;

uint8_t fps=0;
#define JPEG_BUF_SIZE  1024*50
uint8_t jpegBuffer[JPEG_BUF_SIZE];
uint8_t dma_over = 0;


uint32_t valid_data_len;
extern uint16_t img_width, img_height;

void ImagDisp(void)
{
		//扫描模式，横屏
    NT35510_GramScan(5);
    LCD_SetColors(RED,BLACK);
	
    NT35510_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
	
	img_width=640;
	img_height =480;
    /*DMA会把数据传输到液晶屏，开窗后数据按窗口排列 */
    NT35510_OpenWindow(0,0,img_width,img_height);	
    OV2640_DMA_Config((uint32_t) jpegBuffer, JPEG_BUF_SIZE);
    dma_over = 0;
}
void ImagToBuffer(void)
{
    OV2640_DMA_Config_buffer((uint32_t) jpegBuffer, JPEG_BUF_SIZE);
    dma_over = 0;
}

    
static void thread_entry(void* paremeter)
{
    
    I2CMaster_Init(); 
    OV2640_HW_Init();   
    OV2640_ReadID(&OV2640_Camera_ID);
	if(OV2640_Camera_ID.PIDH  == 0x26)
    {
        uart1.printf("%x%x",OV2640_Camera_ID.PIDH ,OV2640_Camera_ID.PIDL); 
    }
    else
    {
        uart1.printf("没有检测到OV2640摄像头，请重新检查连接。");

        while(1);  
    }
    
    ov2640_start();
//    OV2640_UXGAConfig();
//    OV2640_JPEGConfig(JPEG_160x120);
//    OV2640_JPEGConfig(JPEG_640x480);
//    OV2640_JPEGConfig(JPEG_800x600);
//    OV2640_JPEGConfig(JPEG_1024x768);
//    OV2640_JPEGConfig(JPEG_1280x960);
    OV2640_JPEGConfig(JPEG_800x600);

	OV2640_Init();  
    fps =0;
    ImagToBuffer();
    
    while(1)
    {
        
        if(dma_over == 1)
         {
             dma_over = 0;
            uint8_t *ptr = jpegBuffer;
            uint32_t len = (JPEG_BUF_SIZE - valid_data_len)*4;
//            uart1.write(ptr, len);
             HAL_DCMI_Start_DMA(&DCMI_Handle, DCMI_MODE_SNAPSHOT, (uint32_t)jpegBuffer,JPEG_BUF_SIZE);
         }
//        uart1.printf("fps:%d,%d\n",fps,valid_data_len);
         fps =0;

        delay_ms(10);
    }
    return ;
}

int app3_create()
{
    rt_thread_t thread = rt_thread_create( "app3",     /*name*/
                                    thread_entry,/*入口*/
                                    RT_NULL,/*参数*/
                                    1024,    /*heap*/
                                    3 ,    /*优先级*/
                                     20);   /*时间片段*/
    if(thread !=RT_NULL)
    {
        rt_thread_startup (thread);
    }
    else 
    {
        rt_kprintf("app3 create failed\n");
        return -1;
    }
    return 0;
}



extern DMA_HandleTypeDef DMA_Handle_dcmi;
extern "C"
{

void DMA2_Stream1_IRQHandler(void)
{
    rt_interrupt_enter();
    valid_data_len = __HAL_DMA_GET_COUNTER(& DMA_Handle_dcmi);
    HAL_DMA_IRQHandler(&DMA_Handle_dcmi);
    dma_over = 1;
    rt_interrupt_leave();
}

/**
  * @brief  DCMI
  * @param  None
  * @retval None
  */
void DCMI_IRQHandler(void)
{
	rt_interrupt_enter();

  if(  __HAL_DCMI_GET_FLAG(&DCMI_Handle,DCMI_FLAG_FRAMERI) == SET )
    {
      fps++;
//        valid_data_len = __HAL_DMA_GET_COUNTER(& DMA_Handle_dcmi);
    __HAL_DMA_DISABLE(& DMA_Handle_dcmi);
    HAL_DCMI_Stop(&DCMI_Handle); //关闭捕获
//    rt_kprintf("dcmi over\n");
//   __HAL_DMA_ENABLE(& DMA_Handle_dcmi);
//    HAL_DCMI_Start_DMA(&DCMI_Handle, DCMI_MODE_CONTINUOUS, FSMC_LCD_ADDRESS,1);
//    HAL_DCMI_Start_DMA(&DCMI_Handle, DCMI_MODE_SNAPSHOT, (uint32_t)jpegBuffer,JPEG_BUF_SIZE);
//     uart1.printf("dcmi over\n");
    __HAL_DCMI_CLEAR_FLAG(&DCMI_Handle,DCMI_FLAG_FRAMERI);
//      DCMI->CR|=DCMI_CR_CAPTURE;          //DCMI捕获使能
    }
    else
    {
        HAL_DCMI_IRQHandler(&DCMI_Handle);
    }
        rt_interrupt_leave();
}
}
