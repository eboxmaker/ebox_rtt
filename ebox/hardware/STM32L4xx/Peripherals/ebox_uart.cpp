/**
  ******************************************************************************
  * @file    uart.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  *         2018/8/6  ��Tx pin ���÷��ڴ��ڳ�ʼ��֮�󣬷�ֹ����0x00
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

uint16_t _tx_buffer_size[UART_NUM];   // ���ͻ��λ�����size
uint16_t _tx_buffer_head[UART_NUM];   // ������ͷ,ÿд�루д�뻺������һ���ַ�������ƶ�1
uint16_t _tx_buffer_tail[UART_NUM];   // ������β,ÿд����д�봮��TX��һ���ַ�������ƶ�1

uint16_t _rx_buffer_size[UART_NUM];
uint16_t _rx_buffer_head[UART_NUM];
uint16_t _rx_buffer_tail[UART_NUM];

#ifdef UART_9_BIT
uint16_t *_tx_ptr[UART_NUM];          // ������ָ��
uint16_t *_rx_ptr[UART_NUM];
#else
uint8_t *_tx_ptr[UART_NUM];          // ������ָ��
uint8_t *_rx_ptr[UART_NUM];
#endif

/**
 *@name     Uart::Uart(USART_TypeDef *USARTx,Gpio *tx_pin,Gpio *rx_pin)
 *@brief    ���ڵĹ��캯��
 *@param    USARTx:  USART1,2,3��UART4,5
 *          tx_pin:  ��������Ӧ��tx����
 *          rx_pin:  ��������Ӧ��rx����
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
 *@brief    ���ڳ�ʼ�����������˲����ʿɿ��⣬��������Ĭ�ϣ�8bit���ݣ�1��ֹͣλ����У�飬��Ӳ��������
 *@param    baud_rate:  �����ʣ�����9600��115200��38400�ȵ�
 *@param    _use_dma:   �Ƿ�ʹ��DMA��Ĭ��ֵ1��ʹ��DMA��0��������DMA;
 *@retval   None
*/
void    Uart::begin(uint32_t baud_rate, RxMode_t mode)
{
    Uart::begin(baud_rate, 8, 0, 1, mode);
}


/**
 *@name     void    begin(uint32_t baud_rate,uint8_t data_bit,uint8_t parity,float stop_bit,uint8_t _use_dma);
 *@brief    ���ڳ�ʼ�������������и������ò���
 *@param    baud_rate:  �����ʣ�����9600��115200��38400�ȵ�
 *          data_bit:   ����λ����ֻ������8����9
 *          parity:     ����λ��0����У��λ��1��У�飬2żУ��
 *          stop_bit:   ֹͣλ��0.5,1,1.5,2�ĸ���ѡ����
 *          _use_dma:   �Ƿ�ʹ��DMA��Ĭ��ֵ1��ʹ��DMA��0��������DMA;
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
 *@brief    ��λ���ڣ������RX������
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
 *@brief    �����жϿ��ƺ���
 *@param    enable:  ENABLEʹ�ܴ��ڵķ�����ɺͽ����жϣ�DISABLE���ر��������ж�
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
 *@brief    �൱��read������ȡ�󲻴ӻ������������
 *@param    NONE
 *@retval   ����tailָ�������
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
 *@brief    �жϽ��ջ������Ƿ�������
 *@param    NONE
 *@retval   0 �����ݣ���0����ǰ���������ݳ���
*/
int Uart::available()
{
    if(mode == RxDMA)
        return ((unsigned int)(_rx_buffer_size[index] + (_rx_buffer_size[index] - hdma_rx.Instance->CNDTR) - _rx_buffer_tail[index])) % _rx_buffer_size[index];
    else
        return ((unsigned int)(_rx_buffer_size[index] + _rx_buffer_head[index] - _rx_buffer_tail[index])) % _rx_buffer_size[index];
}

/**
 *@brief    ��ȡһ���ֽڣ��������ǰ����
 *@param    NONE
 *@retval   -1  �����ݣ���������ǰ����
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
 *@brief    �Ƿ����д��
 *@param    NONE
 *@retval   ����д�������
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
 *@brief    �ֶ���tx����������ȫ��д��
 *@param    NONE
 *@retval   NONE
*/
void Uart::flush()
{
    uint8_t major, minor;
    interrupt(TxIrq, DISABLE); //�ڼ����رմ����ж�
    while(_tx_buffer_head[index] != _tx_buffer_tail[index] )
    {
        while(LL_USART_IsActiveFlag_TXE(_USARTx) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
        tx_bufferx_one(_USARTx, index);
    }
}


/**
 *@brief    ����һ���ֽ�
 *@param    c��Ҫ���͵��ַ�
 *@retval   1
*/
size_t Uart::write(uint8_t c)
{
    if(!_is_inited) return 0;
    uint16_t i = (_tx_buffer_head[index] + 1) % _tx_buffer_size[index];//����ͷ��λ��
    // head = tail, �������������ȷ���
//    while (i == _tx_buffer_tail[index])
//    {
//        interrupt(TxIrq, DISABLE); //�ڼ����رմ����ж�
//        while(LL_USART_IsActiveFlag_TXE(_USARTx) == RESET);//���ֽڵȴ����ȴ��Ĵ�����
//        tx_bufferx_one(_USARTx, index);
//        interrupt(TxIrq, ENABLE); //��������
//    }
    if(i == _tx_buffer_tail[index])
    {
        flush();
    }
    // ���������ݣ��ƶ�head
    _tx_ptr[index][_tx_buffer_head[index]] = c;
    _tx_buffer_head[index] = i;

    interrupt(TxIrq, ENABLE); //��������
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
      *@brief    �ӻ�����д��һ���ַ�
      *@param    uart�����ڣ� index ���� IT ����
      *@retval   1
      */
    void tx_bufferx_one(USART_TypeDef *uart, uint8_t index)
    {
        if (_tx_buffer_head[index] == _tx_buffer_tail[index])//�������ֱ�ӷ���
        {
            LL_USART_DisableIT_TXE(uart);
            return;
        }
        unsigned char c = _tx_ptr[index][_tx_buffer_tail[index]];   // ȡ���ַ�
        _tx_buffer_tail[index] = (_tx_buffer_tail[index] + 1) % _tx_buffer_size[index];
        uart->TDR = (c & (uint16_t)0x01FF);        
        if (_tx_buffer_head[index] == _tx_buffer_tail[index])//�����������������
        {
            // Buffer empty, so disable interrupts
            LL_USART_DisableIT_TXE(uart);
        }
    }
    /**
      *@brief    ����һ���ַ����뻺����
      *@param    uart�����ڣ� index ���� IT ����
      *@retval   1
      */
    void rx_buffer_one(USART_TypeDef *uart, uint8_t index)
    {
        uint16_t i = (_rx_buffer_head[index] + 1) % _rx_buffer_size[index];//����ͷ��λ��
        if(i == _rx_buffer_tail[index]) //������λ��������˾��޸�һ��tail�������������ϵ�һ������
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



