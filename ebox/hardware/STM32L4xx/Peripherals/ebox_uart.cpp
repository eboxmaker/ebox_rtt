/**
  ******************************************************************************
  * @file    uart.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  *         2018/8/6  将Tx pin 设置放在串口初始化之后，防止发送0x00
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
#include "ebox_uart.h"
#include "rcc.h"
#include "nvic.h"
#include "stm32l4xx_ll_usart.h"



UART_HandleTypeDef huart[UART_NUM];

static uint32_t serial_irq_ids[UART_NUM] = {0};

static uart_irq_handler irq_handler;

uint16_t _tx_buffer_size[UART_NUM];   // 发送环形缓冲区size
uint16_t _tx_buffer_head[UART_NUM];   // 缓冲区头,每写入（写入缓冲区）一个字符，向后移动1
uint16_t _tx_buffer_tail[UART_NUM];   // 缓冲区尾,每写出（写入串口TX）一个字符，向后移动1

uint16_t _rx_buffer_size[UART_NUM];
uint16_t _rx_buffer_head[UART_NUM];
uint16_t _rx_buffer_tail[UART_NUM];

#ifdef UART_9_BIT
uint16_t *_tx_ptr[UART_NUM];          // 缓冲区指针
uint16_t *_rx_ptr[UART_NUM];
#else
uint8_t *_tx_ptr[UART_NUM];          // 缓冲区指针
uint8_t *_rx_ptr[UART_NUM];
#endif

/**
 *@name     Uart::Uart(USART_TypeDef *USARTx,Gpio *tx_pin,Gpio *rx_pin)
 *@brief    串口的构造函数
 *@param    USARTx:  USART1,2,3和UART4,5
 *          tx_pin:  外设所对应的tx引脚
 *          rx_pin:  外设所对应的rx引脚
 *@retval   None
*/
Uart::Uart(USART_TypeDef *USARTx, Gpio *tx_pin, Gpio *rx_pin, uint16_t tx_buffer_size, uint16_t rx_buffer_size)
{

    _USARTx = USARTx;
    _tx_pin = tx_pin;
    _rx_pin = rx_pin;
    this->rx_buffer_size = rx_buffer_size;
    this->tx_buffer_size = tx_buffer_size;


}
/**
 *@name     void Uart::begin(uint32_t baud_rate,uint8_t _use_dma)
 *@brief    串口初始化函数，除了波特率可控外，其他参数默认：8bit数据，1个停止位，无校验，无硬件控制流
 *@param    baud_rate:  波特率，例如9600，115200，38400等等
 *@param    _use_dma:   是否使用DMA，默认值1：使用DMA，0：不适用DMA;
 *@retval   None
*/
void    Uart::begin(uint32_t baud_rate, RxMode_t mode)
{
    Uart::begin(baud_rate, 8, 0, 1, mode);
}


/**
 *@name     void    begin(uint32_t baud_rate,uint8_t data_bit,uint8_t parity,float stop_bit,uint8_t _use_dma);
 *@brief    串口初始化函数，并带有更多配置参数
 *@param    baud_rate:  波特率，例如9600，115200，38400等等
 *          data_bit:   数据位数，只能输入8或者9
 *          parity:     检验位；0：无校验位，1奇校验，2偶校验
 *          stop_bit:   停止位；0.5,1,1.5,2四个可选参数
 *          _use_dma:   是否使用DMA，默认值1：使用DMA，0：不适用DMA;
 *@retval   None
*/
void Uart::begin(uint32_t baud_rate, uint8_t data_bit, uint8_t parity, float stop_bit, RxMode_t mode)
{

    uint8_t gpio_af_usart;
    this->mode = mode;

/* USER CODE END USART1_Init 1 */
    hdma_rx.Instance = NULL;
    rcc_clock_cmd((uint32_t)_USARTx,ENABLE);
    switch((uint32_t)_USARTx)
    {
#if USE_UART1
    case (uint32_t)USART1_BASE:
        gpio_af_usart   = GPIO_AF7_USART1;
        hdma_rx.Instance = DMA1_Channel5;
        index = NUM_UART1;
        break;
#endif

#if USE_UART2
    case (uint32_t)USART2_BASE:
        gpio_af_usart   = GPIO_AF7_USART2;
        hdma_rx.Instance = DMA1_Channel6;
        index = NUM_UART2;
        break;
#endif

#if USE_UART3
    case (uint32_t)USART3_BASE:
        gpio_af_usart   = GPIO_AF7_USART3;
        hdma_rx.Instance = DMA1_Channel3;
        index = NUM_UART3;
        rx_buffer_size = 1024;
        break;
#endif

#if USE_UART4
    case (uint32_t)UART4_BASE:
        gpio_af_usart   = GPIO_AF8_UART4;
        index = NUM_UART4;
        hdma_rx.Instance = DMA2_Channel5;
        break;
#endif

#if USE_UART5
    case (uint32_t)UART5_BASE:
        gpio_af_usart   = GPIO_AF_USART6;
        hdma_rx.Instance = DMA1_Stream1;
        hdma_rx.Init.Channel = DMA_CHANNEL_4;
        index = NUM_UART5;
        break;
#endif

#if USE_LPUART1
    case (uint32_t)LPUART1_BASE:
        gpio_af_usart   = GPIO_AF8_LPUART1;
        index           = NUM_LPUART1;
        hdma_rx.Instance = DMA2_Channel7;
        break;
#endif
    }
    huart[index].Instance = _USARTx;

    _tx_buffer_size[index] = tx_buffer_size;
    _rx_buffer_size[index] = rx_buffer_size;

#ifdef UART_9_BIT
    _tx_ptr[index] = (uint16_t *)rt_malloc(_tx_buffer_size[index] * sizeof(uint16_t));
    _rx_ptr[index] = (uint16_t *)rt_malloc(_rx_buffer_size[index] * sizeof(uint16_t));
#else
    _tx_ptr[index] = (uint8_t *)rt_malloc(_tx_buffer_size[index] * sizeof(uint8_t));
    _rx_ptr[index] = (uint8_t *)rt_malloc(_rx_buffer_size[index] * sizeof(uint8_t));
#endif



    serial_irq_handler(index, Uart::_irq_handler, (uint32_t)this);

    huart[index].Init.BaudRate = baud_rate;

    switch(data_bit)
    {
    case 8:
        huart[index].Init.WordLength = UART_WORDLENGTH_8B;
        break;
    case 9:
        huart[index].Init.WordLength = UART_WORDLENGTH_9B;
        break;
    default :
        huart[index].Init.WordLength = UART_WORDLENGTH_8B;
        break;
    }
    switch(parity)
    {
    case 0:
        huart[index].Init.Parity = UART_PARITY_NONE;
        break;
    case 1:
        huart[index].Init.Parity = UART_PARITY_ODD;
        break;
    case 2:
        huart[index].Init.Parity = UART_PARITY_EVEN;
        break;
    default :
        huart[index].Init.Parity = UART_PARITY_NONE;
        break;
    }
    if(stop_bit == 1)
        huart[index].Init.StopBits = UART_STOPBITS_1;    
    else if(stop_bit == 2)
        huart[index].Init.StopBits = UART_STOPBITS_2;    
    else
        huart[index].Init.StopBits = UART_STOPBITS_1;   
    huart[index].Init.Mode = UART_MODE_TX_RX;
    huart[index].Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart[index].Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart[index]) != HAL_OK)
    {
    }

//    if((mode == RxDMA) && (hdma_rx.Instance != NULL))
//    {
//        rcc_clock_cmd((uint32_t)hdma_rx.Instance,ENABLE);
////        HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
//        HAL_NVIC_DisableIRQ(DMA1_Channel5_IRQn);
////        nvic_dev_disable((uint32_t)DMA1_Stream1,0);
//        
//        hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//        hdma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//        hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
//        hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//        hdma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//        hdma_rx.Init.Mode = DMA_CIRCULAR;
//        hdma_rx.Init.Priority = DMA_PRIORITY_LOW;
//        if (HAL_DMA_Init(&hdma_rx) != HAL_OK)
//        {
//        }
//        __HAL_LINKDMA(&huart[index],hdmarx,hdma_rx);
//        HAL_UART_Receive_DMA(&huart[index], _rx_ptr[index], _rx_buffer_size[index]);
//        interrupt(RxIrq, DISABLE);
//    }
//    else
    {
        interrupt(RxIrq, ENABLE);
    }

//    _tx_pin->mode(AF_PP);
//    _rx_pin->mode(INPUT);
    
    _tx_pin->mode(AF_PP, gpio_af_usart);
    _rx_pin->mode(AF_PP, gpio_af_usart);
    
    nvic(ENABLE, 0, 0);
    interrupt(TxIrq, DISABLE);

    _is_inited = true;
}

/**
 *@brief    复位串口，并清空RX缓冲区
 *@param    none
 *@retval   None
*/
void Uart::end()
{
    HAL_UART_DeInit(&huart[index]);
    // clear any received data
    _rx_buffer_head[index] = _rx_buffer_tail[index];
}
void Uart::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
    switch(index)
    {
        case NUM_UART1:
            HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
            enable== true ? HAL_NVIC_EnableIRQ(USART1_IRQn):HAL_NVIC_DisableIRQ(USART1_IRQn);
            break;
        case NUM_UART2:
            HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
            enable== true ? HAL_NVIC_EnableIRQ(USART2_IRQn):HAL_NVIC_DisableIRQ(USART2_IRQn);
            break;
        case NUM_UART3:
            HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
            enable== true ? HAL_NVIC_EnableIRQ(USART3_IRQn):HAL_NVIC_DisableIRQ(USART3_IRQn);
            break;
        #if USE_UART4
        case NUM_UART4:
            HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
            enable== true ? HAL_NVIC_EnableIRQ(UART4_IRQn):HAL_NVIC_DisableIRQ(UART4_IRQn);
            break;
        #endif
        #if USE_UART5
        case NUM_UART5:
            HAL_NVIC_SetPriority(USART5_IRQn, 0, 0);
            enable== true ? HAL_NVIC_EnableIRQ(USART1_IRQn):HAL_NVIC_DisableIRQ(USART1_IRQn);
            break;
        #endif
        case NUM_LPUART1:
            HAL_NVIC_SetPriority(LPUART1_IRQn, 0, 0);
            enable== true ? HAL_NVIC_EnableIRQ(LPUART1_IRQn):HAL_NVIC_DisableIRQ(LPUART1_IRQn);
            break;
    }
    

}
///////////////////////////////////////////////////////////////

/**
 *@brief    串口中断控制函数
 *@param    enable:  ENABLE使能串口的发送完成和接收中断；DISABLE：关闭这两个中断
 *@retval   None
*/
void Uart::interrupt(IrqType type, FunctionalState enable)
{

    if(type == RxIrq)
    {
        if(enable == ENABLE)
            __HAL_UART_ENABLE_IT(&huart[index],UART_IT_RXNE);
        else
            __HAL_UART_DISABLE_IT(&huart[index],UART_IT_RXNE);

    }
    else if(type == TxIrq)
    {
        if(enable == ENABLE)
            __HAL_UART_ENABLE_IT(&huart[index],UART_IT_TXE);
        else
            __HAL_UART_DISABLE_IT(&huart[index],UART_IT_TXE);
    }

}

/**
 *@brief    相当与read，但读取后不从缓冲区清除数据
 *@param    NONE
 *@retval   返回tail指向的数据
*/
int Uart::peek(void)
{
    if ((_rx_buffer_size[index] - hdma_rx.Instance->CNDTR) == _rx_buffer_tail[index])
    {
        return -1;
    }
    else
    {
        return _rx_ptr[index][_rx_buffer_tail[index]];
    }
}
/**
 *@brief    判断接收缓冲区是否有数据
 *@param    NONE
 *@retval   0 无数据，非0，当前缓冲区数据长度
*/
int Uart::available()
{
    if(mode == RxDMA)
        return ((unsigned int)(_rx_buffer_size[index] + (_rx_buffer_size[index] - hdma_rx.Instance->CNDTR) - _rx_buffer_tail[index])) % _rx_buffer_size[index];
    else
        return ((unsigned int)(_rx_buffer_size[index] + _rx_buffer_head[index] - _rx_buffer_tail[index])) % _rx_buffer_size[index];
}

/**
 *@brief    读取一个字节，并清除当前数据
 *@param    NONE
 *@retval   -1  无数据，其他，当前数据
*/
int Uart::read()
{
    if(mode == RxDMA)
    {
        if (_rx_buffer_tail[index] == (_rx_buffer_size[index] - hdma_rx.Instance->CNDTR))
        {
            return -1;
        }
    }
    else
    {
        if (_rx_buffer_tail[index] == _rx_buffer_head[index])
        {
            return -1;
        }
    }
    int c = _rx_ptr[index][_rx_buffer_tail[index]];
    _rx_buffer_tail[index] = (_rx_buffer_tail[index] + 1) % _rx_buffer_size[index];
    return c;
}

/**
 *@brief    是否可以写入
 *@param    NONE
 *@retval   可以写入的数量
*/
int Uart::availableForWrite()
{
    uint16_t head = _tx_buffer_head[index];
    uint16_t tail = _tx_buffer_tail[index];

    if (head >= tail)
        return _tx_buffer_size[index] - 1 - head + tail;

    return tail - head - 1;
}

/**
 *@brief    手动将tx缓冲区内容全部写出
 *@param    NONE
 *@retval   NONE
*/
void Uart::flush()
{
    uint8_t major, minor;
    interrupt(TxIrq, DISABLE); //期间必须关闭串口中断
    while(_tx_buffer_head[index] != _tx_buffer_tail[index] )
    {
        while(LL_USART_IsActiveFlag_TXE(_USARTx) == RESET);//单字节等待，等待寄存器空
        tx_bufferx_one(_USARTx, index);
    }
}


/**
 *@brief    发送一个字节
 *@param    c：要发送的字符
 *@retval   1
*/
size_t Uart::write(uint8_t c)
{
    if(!_is_inited) return 0;
    uint16_t i = (_tx_buffer_head[index] + 1) % _tx_buffer_size[index];//计算头的位置
    // head = tail, 缓冲区过满，先发送
//    while (i == _tx_buffer_tail[index])
//    {
//        interrupt(TxIrq, DISABLE); //期间必须关闭串口中断
//        while(LL_USART_IsActiveFlag_TXE(_USARTx) == RESET);//单字节等待，等待寄存器空
//        tx_bufferx_one(_USARTx, index);
//        interrupt(TxIrq, ENABLE); //开启发送
//    }
    if(i == _tx_buffer_tail[index])
    {
        flush();
    }
    // 加入新数据，移动head
    _tx_ptr[index][_tx_buffer_head[index]] = c;
    _tx_buffer_head[index] = i;

    interrupt(TxIrq, ENABLE); //开启发送
    return 1;
}


void Uart::attach(void (*fptr)(void), IrqType type)
{
    if (fptr)
    {
        _irq[type].attach(fptr);
    }
}

void Uart::_irq_handler(uint32_t id, IrqType irq_type)
{
    Uart *handler = (Uart *)id;
    handler->_irq[irq_type].call();
}


extern "C" {
    /**
      *@brief    从缓冲区写出一个字符
      *@param    uart：串口； index 串口 IT 索引
      *@retval   1
      */
    void tx_bufferx_one(USART_TypeDef *uart, uint8_t index)
    {
        if (_tx_buffer_head[index] == _tx_buffer_tail[index])//如果空则直接返回
        {
            LL_USART_DisableIT_TXE(uart);
            return;
        }
        unsigned char c = _tx_ptr[index][_tx_buffer_tail[index]];   // 取出字符
        _tx_buffer_tail[index] = (_tx_buffer_tail[index] + 1) % _tx_buffer_size[index];
        uart->TDR = (c & (uint16_t)0x01FF);        
        if (_tx_buffer_head[index] == _tx_buffer_tail[index])//如果发送完所有数据
        {
            // Buffer empty, so disable interrupts
            LL_USART_DisableIT_TXE(uart);
        }
    }
    /**
      *@brief    读入一个字符放入缓冲区
      *@param    uart：串口； index 串口 IT 索引
      *@retval   1
      */
    void rx_buffer_one(USART_TypeDef *uart, uint8_t index)
    {
        uint16_t i = (_rx_buffer_head[index] + 1) % _rx_buffer_size[index];//计算头的位置
        if(i == _rx_buffer_tail[index]) //如果环形缓冲区满了就修改一次tail，将会舍弃最老的一个数据
        {
            _rx_buffer_tail[index] = (_rx_buffer_tail[index] + 1) % _rx_buffer_size[index];
        }
        _rx_ptr[index][_rx_buffer_head[index]] = uart->RDR;
        _rx_buffer_head[index] = i;
    }
    
  


#if USE_UART1

    void USART1_IRQHandler(void)
    {
        rt_interrupt_enter();
        if(LL_USART_IsActiveFlag_RXNE(USART1) == SET)
        {
            rx_buffer_one(USART1, NUM_UART1);
//            irq_handler(serial_irq_ids[NUM_UART1], RxIrq);
            SET_BIT(USART1->RQR,USART_RQR_RXFRQ);
        }
        else if(LL_USART_IsActiveFlag_TXE(USART1) == SET)
        {
            tx_bufferx_one(USART1, NUM_UART1);
//            irq_handler(serial_irq_ids[NUM_UART1], TxIrq);
            LL_USART_ClearFlag_TC(USART1);
        }
        else
        {
            HAL_UART_IRQHandler(&huart[NUM_UART1]);
        }

        rt_interrupt_leave();
    }
#endif

#if USE_UART2
    void USART2_IRQHandler(void)
    {
        rt_interrupt_enter();
        if(LL_USART_IsActiveFlag_RXNE(USART2) == SET)
        {
            rx_buffer_one(USART2, NUM_UART2);
//            irq_handler(serial_irq_ids[NUM_UART2], RxIrq);
            SET_BIT(USART2->RQR,USART_RQR_RXFRQ);
        }
        else if(LL_USART_IsActiveFlag_TXE(USART2) == SET)
        {
            tx_bufferx_one(USART2, NUM_UART2);
//            irq_handler(serial_irq_ids[NUM_UART2], TxIrq);
            LL_USART_ClearFlag_TC(USART2);
        }
        else
        {
            HAL_UART_IRQHandler(&huart[NUM_UART2]);
        }
        
        rt_interrupt_leave();
    }
#endif


#if USE_UART3
    void USART3_IRQHandler(void)
    {
        rt_interrupt_enter();
        if(LL_USART_IsActiveFlag_RXNE(USART3) == SET)
        {
            rx_buffer_one(USART3, NUM_UART3);
//            irq_handler(serial_irq_ids[NUM_UART3], RxIrq);
            SET_BIT(USART3->RQR,USART_RQR_RXFRQ);
        }
        else if(LL_USART_IsActiveFlag_TXE(USART3) == SET)
        {
            tx_bufferx_one(USART3, NUM_UART3);
//            irq_handler(serial_irq_ids[NUM_UART3], TxIrq);
            LL_USART_ClearFlag_TC(USART3);
        }
//        else
//        {
//            HAL_UART_IRQHandler(&huart[NUM_UART3]);
//        }
//        
        rt_interrupt_leave();
        
    }
#endif


#if defined (STM32F10X_HD)
    #if USE_UART4
    void UART4_IRQHandler(void)
    {
        if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(UART4, NUM_UART4);
            irq_handler(serial_irq_ids[NUM_UART4], RxIrq);
            USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART4, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(UART4, NUM_UART4);
            irq_handler(serial_irq_ids[NUM_UART4], TxIrq);
        }
    }
    #endif

    #if USE_UART5

    void UART5_IRQHandler(void)
    {
        if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
        {
            rx_buffer_one(UART5, NUM_UART5);
            irq_handler(serial_irq_ids[NUM_UART5], RxIrq);
            USART_ClearITPendingBit(UART5, USART_IT_RXNE);
        }
        if(USART_GetITStatus(UART5, USART_IT_TXE) == SET)
        {
            tx_bufferx_one(UART5, NUM_UART5);
            irq_handler(serial_irq_ids[NUM_UART5], TxIrq);
        }
    }
    #endif

#endif
#if USE_LPUART1

    void LPUART1_IRQHandler(void)
    {
        rt_interrupt_enter();
        if(LL_USART_IsActiveFlag_RXNE(LPUART1) == SET)
        {
            rx_buffer_one(LPUART1, NUM_LPUART1);
//            irq_handler(serial_irq_ids[NUM_LPUART1], RxIrq);
            SET_BIT(LPUART1->RQR,USART_RQR_RXFRQ);
        }
        else if(LL_USART_IsActiveFlag_TXE(LPUART1) == SET)
        {
            tx_bufferx_one(LPUART1, NUM_LPUART1);
//            irq_handler(serial_irq_ids[NUM_LPUART1], TxIrq);
            LL_USART_ClearFlag_TC(LPUART1);
        }
        else
        {
            HAL_UART_IRQHandler(&huart[NUM_LPUART1]);
        }

        rt_interrupt_leave();
    }
#endif
    void serial_irq_handler(uint8_t index, uart_irq_handler handler, uint32_t id)
    {
        irq_handler = handler;
        serial_irq_ids[index] = id;
    }
}




