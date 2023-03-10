/*
    ModbusSerial.cpp - Source for Modbus Serial Library
    Copyright (C) 2014 André Sarmento Barbosa
*/
#include "ModbusSerial.h"

ModbusSerial::ModbusSerial()
{

}

bool ModbusSerial::setSlaveId(byte slaveId)
{
    _slaveId = slaveId;
    return true;
}

byte ModbusSerial::getSlaveId()
{
    return _slaveId;
}

bool ModbusSerial::config(Uart *port, long baud, Gpio *txPin)
{
    this->_port = port;
    this->_txPin = txPin;
    (*port).begin(baud,RxIt);

    delay_ms(10);

    if (txPin != NULL)
    {
        txPin->mode(OUTPUT_PP_PN);
        txPin->write(LOW);
    }

    if (baud > 19200)
    {
        _t15 = 750;
        _t35 = 1750;
    }
    else
    {
        _t15 = 15000000 / baud; // 1T * 1.5 = T1.5
        _t35 = 35000000 / baud; // 1T * 3.5 = T3.5
    }

    return true;
}


bool ModbusSerial::receive(byte *frame)
{
    //first byte of frame = address
    byte address = frame[0];
    //Last two bytes = crc
    u_int crc = ((frame[_len - 2] << 8) | frame[_len - 1]);

    //Slave Check
    if (address != 0xFF && address != this->getSlaveId())
    {
        return false;
    }

    //CRC Check
    if (crc != this->calcCrc(_frame[0], _frame + 1, _len - 3))
    {
        return false;
    }

    //PDU starts after first byte
    //framesize PDU = framesize - address(1) - crc(2)
    this->receivePDU(frame + 1);
    //No reply to Broadcasts
    if (address == 0xFF) _reply = MB_REPLY_OFF;
    return true;
}

bool ModbusSerial::send(byte *frame)
{
    byte i;

    if (this->_txPin != NULL)
    {
        this->_txPin->write(HIGH);
        delay_ms(1);
    }

    for (i = 0 ; i < _len ; i++)
    {
        (*_port).write(frame[i]);
    }

    (*_port).flush();
    delay_us(_t35);

    if (this->_txPin != NULL)
    {
        this->_txPin->write(LOW);
    }
    return true;
}

bool ModbusSerial::sendPDU(byte *pduframe)
{
    if (this->_txPin != NULL)
    {
        this->_txPin->write(HIGH);
        delay_ms(1);
    }

    //Send slaveId
    (*_port).write(_slaveId);

    //Send PDU
    byte i;
    for (i = 0 ; i < _len ; i++)
    {
        (*_port).write(pduframe[i]);
    }

    //Send CRC
    word crc = calcCrc(_slaveId, _frame, _len);
    (*_port).write(crc >> 8);
    (*_port).write(crc & 0xFF);

    (*_port).flush();
    delay_us(_t35);

    if (this->_txPin != NULL)
    {
        this->_txPin->write(LOW);
    }
    return true;
}

void ModbusSerial::task()
{
    _len = 0;

    while ((*_port).available() > _len)
    {
        _len = (*_port).available();
//        delay_us(_t15);
        delay_ms(1);
    }

    if (_len == 0) return;
    byte i;
    _frame = (byte *) rt_malloc(_len);
    for (i = 0 ; i < _len ; i++) _frame[i] = (*_port).read();

    if (this->receive(_frame))
    {
        if (_reply == MB_REPLY_NORMAL)
            this->sendPDU(_frame);
        else if (_reply == MB_REPLY_ECHO)
            this->send(_frame);
    }

    rt_free(_frame);
    _len = 0;
}

word ModbusSerial::calcCrc(byte address, byte *pduFrame, byte pduLen)
{
    byte CRCHi = 0xFF, CRCLo = 0x0FF, Index;

    Index = CRCHi ^ address;
    CRCHi = CRCLo ^ _auchCRCHi[Index];
    CRCLo = _auchCRCLo[Index];

    while (pduLen--)
    {
        Index = CRCHi ^ *pduFrame++;
        CRCHi = CRCLo ^ _auchCRCHi[Index];
        CRCLo = _auchCRCLo[Index];
    }

    return (CRCHi << 8) | CRCLo;
}





