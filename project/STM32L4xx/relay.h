#ifndef __RELAY_H
#define __RELAY_H

#include "ebox.h"


//自然电位电压
//1cm2试片通电直流电位
//1cm2试片断电直流电位
//1cm2试片交流干扰电位
//1cm2试片腐蚀试片交流电流
//1cm2试片腐蚀试片直流电流
//10cm2试片通电直流电位
//10cm2试片断电直流电位
//10cm2试片交流干扰电位
//10cm2试片腐蚀试片直流电流
//CuSO4参比Zn电极损耗电位
//大部分时间处于平常低功耗状态
//初始状态




class Relay
{
    public:
        typedef enum
        {
            Mode0DEFAULT,
            Mode1ADV,
            Mode2AAV,
            Mode3ADC,
            Mode4AAC,
            Mode5BDV,
            Mode6BAV,
            Mode7BDC,
            Mode8BAC,
            Mode9NDV,
            Mode10NADV,
            Mode11NBDV,
            Mode12NCuZnDV,
            Mode13OPEN,
        }Mode_t;
        
    public:
        Relay();
        void begin();
        void on(uint8_t index);
        void off(uint8_t index);
        void ctr(uint8_t index, bool value);
        void mode(Mode_t mod);
        uint16_t mode_bitmap(Mode_t mod);
        bool isValidSampleCh(Mode_t mod);
        bool isAcCh(Mode_t mod);
        Mode_t mode_read();
        void loop();
        void test();
    private:
        Gpio *k[16];
        bool changed;
        Mode_t mlast;
        uint32_t changed_time;


};
extern Relay relay;




//relay table 



#endif

