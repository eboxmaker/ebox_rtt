#include "soft_i2c.h"


//#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define EBOX_DEBUG_SOFTI2C_ENABLE       true
#define EBOX_DEBUG_SOFTI2C_ENABLE_ERR   true
//#endif


#if EBOX_DEBUG_SOFTI2C_ENABLE
#define softI2cDebug(...)  rt_kprintf("[sI2C DBG]:%d: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define softI2cDebug(...)
#endif

#if EBOX_DEBUG_SOFTI2C_ENABLE_ERR
#define softI2cDebugErr(...)  rt_kprintf("[sI2C err]:%d: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define softI2cDebugErr(fmt, ...)
#endif



SoftI2c::SoftI2c(Gpio *sclPin, Gpio *sdaPin)
{
    _scl = sclPin;
    _sda = sdaPin;
    setTimeout(100);
}


/**
  *@brief    ����i2cʱ�Ӻ���������speed����timing,Ĭ��80K
  *@param    speed:  ���� 10,100,200,300,400 �ֱ����10k��100k��200k,300k,400k
  *@retval   None
  */
void SoftI2c::begin(uint8_t address)
{
    SoftI2c::begin();
}
void SoftI2c::begin(int address)
{
    SoftI2c::begin();
}
void SoftI2c::begin()
{
    _sda->mode(OUTPUT_OD_PU);
    _scl->mode(OUTPUT_OD_PU);
    _scl->reset();
    _sda->reset();
    delay_us(1);
    _scl->set();
    delay_us(1);
    _sda->set();
    
    softI2cDebug("scl_pin: 0x%x ; sda_pin: 0x%x\n",_scl->id, _sda->id);
    setClock();
}
/**
 * @brief ����i2cͨ���ٶ�.
 *
 * @param[in] speed i2c�ٶȣ�����ֵΪ400K��300K��200K��100Kʱ��i2cͨ��Ƶ��Ϊ��ֵ.
 *
 * @return 0.
 */
void SoftI2c::setClock(Speed_t speed)
{
    switch(speed)
    {
    case K1000:
    case K400:
    case K300:
        _bitDelay = 1;    // Լ200k
        break;
    case K200:
        _bitDelay = 2;    // Լ250k
        break;
    case K100:
        _bitDelay = 3;    // Լ100k
        break;
    default:
        _bitDelay = 10;    // Լ80k
        break;
    }
    softI2cDebug("bit delay:%d;\n",speed);
}

i2c_err_t SoftI2c::_write(const uint8_t *data, size_t quantity)
{
    i2c_err_t ret;
    for(int i = 0; i < quantity; i++) {
        ret = _sendByte(data[i]);
        if(ret != I2C_ERROR_OK)
        {
            return ret;
        }
    }
    return ret;
}

/*
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 *          5 .. timeout
 */
i2c_err_t SoftI2c::_write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop)
{
    rt_interrupt_enter();
    i2c_err_t ret;
    _start();
    ret = _send7bitsAddress(address ,0); // clr read bit
    if(ret != I2C_ERROR_OK)
    {
        ret =  I2C_ERROR_ADDR_NACK;
        softI2cDebugErr("I2C_ERROR_ADDR_NACK_NO_RECV:%d\n",ret);
    }
    if(quantity > 1)
    {
        ret = _sendByte_first(data[0]);
        if(ret != I2C_ERROR_OK)
        {
            ret = I2C_ERROR_DATA_NACK;
            softI2cDebugErr("I2C_ERROR_DATA_NACK_NO_RECV:%d\n",ret);
        }
        ret = _write(&data[1], quantity - 1);                
        if(ret != I2C_ERROR_OK)
        {
            ret = I2C_ERROR_DATA_NACK;
            softI2cDebugErr("I2C_ERROR_DATA_NACK_NO_RECV:%d\n",ret);
        }
    }
    else if(quantity == 1)
    {
        ret = _sendByte_first(data[0]);
        if(ret != I2C_ERROR_OK)
        {
            ret = I2C_ERROR_DATA_NACK;
            softI2cDebugErr("I2C_ERROR_DATA_NACK_NO_RECV:%d\n",ret);
        }
    }
    if(sendStop)
    {
        _stop();
    }
    rt_interrupt_leave();
    return ret;
}
/**
  *@brief    ָ���Ĵ���������ȡ. start->WslaveAddr->regAddr->RSlaverAddr->data...->nRead==1->Nack->stop->data
  *@param    uint8_t slaveAddr:  �ӻ���ַ
  *          uint16_t regAddr: �Ĵ�����ַ
  *          uint8_t *data: ��ȡ��������
  *          uint16_t nRead��Ҫ��ȡ�����ݳ���
  *          : ��ʱ
  *@retval   EOK��EWAIT
  */
i2c_err_t SoftI2c::_read(uint8_t address,uint8_t *data, uint16_t length,uint8_t sendStop)
{
    i2c_err_t ret;

    _start();
    ret = _send7bitsAddress(address , 1); // clr read bit
    if(ret) 
    {
        softI2cDebugErr("Address No Ack:%d\n",ret);
        ret = I2C_ERROR_ADDR_NACK;
        return ret;
    }
    uint16_t cnt = 0;
    while(1)//�ȴ��ӻ��ͷ�SDA����ʾ�ӻ�����׼�����
    {
        if (!_sda->read())//�ӻ�����׼�����
        {
            break;
        }
        else if(cnt >= 500)
        {
            ret = I2C_ERROR_TIMEOUT;
            softI2cDebugErr("I2C_ERROR_TIMEOUT(slave not relea sda)\n" );
            break;
        }
        delay_us(10);
        cnt++;
    }
    while (length--)
    {
        ret = _receiveByte(data);
        data++;
        if (length == 0)
        {
            break;
        }
        _sendAck();
    }
    if(sendStop)
    {
        _sendNack();
        _stop();
    }
    return ret;
}
//---------------------------------------------------------------------------


/**
  * @brief ����һ��START�ź�.SCLΪ�ߵ�ƽʱ��SDA�ɸߵ�ƽ��͵�ƽ���䣬��ʼ��������
  * @param ��.
  * @return ��.
  */
i2c_err_t SoftI2c::_start(void)
{   
    _sda->set();
    _scl->set();          // SCL��
    delay_us(_bitDelay);
    _sda->reset();        // SDA����
    delay_us(_bitDelay);
    return I2C_ERROR_OK;
}

/**
  * @brief ����һ��STOP�ź�. SCLΪ�ߵ�ƽʱ��SDA�ɵ͵�ƽ��ߵ�ƽ���䣬������������
  * @param ��.
  * @return ��.
  */
i2c_err_t SoftI2c::_stop(void)
{   
    _sda->reset();      // SDA�͵�ƽ
    _scl->set();        // SCL���߲�����
    delay_us(_bitDelay);
    _sda->set();        // SDA����
    delay_us(_bitDelay);
    return I2C_ERROR_OK;
}



/**
 * @brief �ȴ�һ��ACKӦ��. ��9��clock������IC��ACK��SDA�ᱻ����
 * @param ��.
 * @return 
 *          0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 *          5 .. timeout
 */
#include <bsp_ebox.h>
i2c_err_t SoftI2c::_waitAck()
{
    i2c_err_t ret;
    _sda->set();
    delay_us(_bitDelay);
    _scl->set();
    delay_us(_bitDelay);
    uint16_t cnt = 0;
    while(1)
    {
        if (!_sda->read())	//SDAΪ������豸����Ack������û����
        {
            ret = I2C_ERROR_OK;
            break;
        }
        else if(cnt >= 5)
        {
            ret = I2C_ERROR_TIMEOUT;
            softI2cDebugErr("I2C_ERROR_TIMEOUT(wait ack)\n" );
            break;
        }
        delay_us(_bitDelay);
        cnt++;
    }
    _scl->reset();
    delay_us(_bitDelay);

    
    return ret;
}
/*
  * @brief ��������.
  * @param[in] byte �������͵�����.
  * @return 
 *          0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 *          5 .. timeout
 */
i2c_err_t SoftI2c::_sendByte( uint8_t c )
{
    uint8_t ii = 8;
    _scl->reset();        //SCL����
    delay_us(_bitDelay);
    while( ii-- )
    {
        _sda->write(c & 0x80);   // SCL�͵�ƽʱ�������͵�SDA
        delay_us(_bitDelay);
        _scl->set();                // ����һ��ʱ������
        delay_us(_bitDelay);
        c = c << 1;
        _scl->reset();
    }
    delay_us(_bitDelay);
    i2c_err_t ret = _waitAck();
    
    return ret;
}
i2c_err_t SoftI2c::_sendByte_first( uint8_t c)
{
    uint16_t cnt = 0;
    _scl->reset();        //SCL����
    delay_us(_bitDelay);
    _sda->write(c & 0x80);   // SCL�͵�ƽʱ�������͵�SDA
    delay_us(_bitDelay);
    _scl->set();                // ����һ��ʱ������
    delay_us(_bitDelay);
    while(!_scl->read())//�ȴ��ӻ��ͷ�����
    {
        cnt++;
        if(cnt > 0x2ff)
        {
            softI2cDebugErr("I2C_ERROR_TIMEOUT(at:%d)\n" );
            return I2C_ERROR_TIMEOUT;
        }
    }
    c = c << 1;
    _scl->reset();
    
    uint8_t ii = 7;
    while( ii-- )
    {
        _sda->write(c & 0x80);   // SCL�͵�ƽʱ�������͵�SDA
        delay_us(_bitDelay);
        _scl->set();                // ����һ��ʱ������
        delay_us(_bitDelay);
        c = c << 1;
        _scl->reset();
    }
    delay_us(_bitDelay);
    i2c_err_t ret = _waitAck();
    
    return ret;
}
/**
 * @brief ����7bit�ӻ���ַλ.
 *
 * @param[in] slave_address 7bit�ӻ���ַλ.
 *
 * @return ���ͽ��.����0��ʾ���ͳɹ�������-1��ʾ����ʧ��.
 */
i2c_err_t	SoftI2c::_send7bitsAddress(uint8_t slaveAddr, uint8_t WR)
{
    // д���ӵ�ַ���λ��0�������ӵ�ַ���λ��1��
    slaveAddr = (WR == 0) ? (slaveAddr & 0xfe) : (slaveAddr | 0x01);
    return _sendByte(slaveAddr);;
}
/**
  * @brief ��������.
  * @param ��.
  * @return ���յ�������.
  */
i2c_err_t SoftI2c::_receiveByte(uint8_t *data)
{
    uint8_t i = 8;
    uint8_t byte = 0;
    _sda->set();
    while(i--)
    {
        byte += byte;
        _scl->reset();      // ʱ�Ӹߵ�ƽ�ڼ��SDA
        delay_us(_bitDelay);
        _scl->set();
        delay_us(_bitDelay);
        byte |= _sda->read();
    }
    _scl->reset();
    delay_us(_bitDelay);
//    softI2cDebug("rx :0x%x\n",byte);

    *data = byte;
    return I2C_ERROR_OK;
}

/**
 * @brief ����һ��ACKӦ��. ��9��clock�ڼ䣬����SDA
 * @param ��.
 * @return 0.
 */
i2c_err_t SoftI2c::_sendAck()
{
    _sda->reset();          // ����SDA��
    delay_us(_bitDelay);
    _scl->set();            // ����һ��ʱ��
    delay_us(_bitDelay);
    _scl->reset();
    delay_us(_bitDelay);
    return I2C_ERROR_OK;
}

/**
  * @brief ����һ��NACKӦ��,��9��clock�ڼ䣬����SDA
  * @param ��
  * @return 0.
  */
i2c_err_t SoftI2c::_sendNack()
{
    _sda->set();
    delay_us(_bitDelay);
    _scl->set();
    delay_us(_bitDelay);
    _scl->reset();
    delay_us(_bitDelay);
    return I2C_ERROR_OK;
}


