#ifndef __CALIBRATION_H
#define __CALIBRATION_H

#include "ebox_core.h"
#include "ebox_uart.h"
class ConfigCalibration
{
    public:
//        ConfigCalibration(){};
        void begin();
        int buf_to_para();
        int para_to_buf();
        void print(Uart &uart);

        uint8_t buf[300];
        uint16_t len;
    public:
        float k[4];
        float b[4];
    

};
void calibration_callback();


#endif

