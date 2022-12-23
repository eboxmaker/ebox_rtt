#include "relay.h"


Relay relay;

#define K1P 0
#define K1N 1
#define K2P 2
#define K2N 3
#define K3P 4
#define K3N 5
#define K4P 6
#define K4N 7
#define K5P 8
#define K5N 9
#define K6P 10
#define K6N 11
#define K7P 12
#define K7N 13
#define K8  14
#define K9  15
#define KMAX  15




//#define TM0DEFAULT 0X9958
//#define TM1ADV 0X5598
//#define TM2AAV 0X5599
//#define TM3ADC 0X565A
//#define TM4AAC 0X565B
//#define TM5BDV 0XA658
//#define TM6BAV 0XA659
//#define TM7BDC 0XA59A
//#define TM8BAC 0XA59B
//#define TM9NDV 0X9568
//#define TM10NADV 0X9598
//#define TM11NBDV 0X9658
//#define TM12NCuZnDV 0X9554
//#define TM13OPEN 0X00

//#define TM0DEFAULT 0X9958
//#define TM1ADV 0X5998
//#define TM2AAV 0X5999
//#define TM3ADC 0X565A
//#define TM4AAC 0X565B
//#define TM5BDV 0XAA58
//#define TM6BAV 0XAA59
//#define TM7BDC 0XA59A
//#define TM8BAC 0XA59B
//#define TM9NDV 0X9968
//#define TM10NADV 0X9998
//#define TM11NBDV 0X9A58
//#define TM12NCuZnDV 0X9954
//#define TM13OPEN 0X00


#define TM0DEFAULT 0X9958
#define TM1ADV 0X5598
#define TM2AAV 0X5599
#define TM3ADC 0X569A
#define TM4AAC 0X569B
#define TM5BDV 0XA658
#define TM6BAV 0XA659
#define TM7BDC 0XA55A
#define TM8BAC 0XA55B
#define TM9NDV 0X9568
#define TM10NADV 0X9598
#define TM11NBDV 0X9658
#define TM12NCuZnDV 0X9554
#define TM13OPEN 0X00

uint16_t mode_table[] = 
{
TM0DEFAULT,
TM1ADV,
TM2AAV,
TM3ADC,
TM4AAC,
TM5BDV,
TM6BAV,
TM7BDC,
TM8BAC,
TM9NDV,
TM10NADV,
TM11NBDV,
TM12NCuZnDV,
TM13OPEN,
};



Relay::Relay()
{
    mlast = Mode13OPEN;
}
void Relay::begin()
{   
    k[0] = &PB10;
    k[1] = &PB9;
    k[2] = &PB12;
    k[3] = &PB11;
    k[4] = &PB14;
    k[5] = &PB13;
    k[6] = &PD8;
    k[7] = &PB15;
    k[8] = &PD10;
    k[9] = &PD9;
    k[10] = &PD12;
    k[11] = &PD11;
    k[12] = &PD14;
    k[13] = &PD13;
    k[14] = &PE0;
    k[15] = &PE1;
    for(int i = 0; i <= KMAX; i++)
    {
        k[i]->mode(OUTPUT_PP_PD);
        k[i]->reset();
    }
}
void Relay::on(uint8_t index)
{
   if(index > KMAX) 
       return ;
   k[index]->set();
}
void Relay::off(uint8_t index)
{
   if(index > KMAX) 
       return ;
   k[index]->reset();
}
void Relay::ctr(uint8_t index, bool value)
{
   if(index > KMAX) 
       return ;
   k[index]->write(value);
}

void Relay::mode(Mode_t mod)
{
    if(mlast == mod) return;
    uint16_t value = mode_table[mod];
//    rt_kprintf("index=%d bit:0X%04X\n",mod,value);
    for(int i = 0; i <= KMAX; i++)
    {
        k[i]->write(bit_read(value,(KMAX - i)));
//        UART.printf("k[%d]=%d\n",i,k[i]->read());
    }
    mlast = mod;
    changed = true;
    changed_time = millis();
}

uint16_t Relay::mode_bitmap(Mode_t mod)
{
    return mode_table[mod];
}
bool Relay::isValidSampleCh(Mode_t mod)
{
    if(mod < Mode1ADV || mod > Mode12NCuZnDV)
        return false;
    return true;
}
bool Relay::isAcCh(Mode_t mod)
{
    uint16_t value = mode_table[mod];
    if((value & 0x01) == 0)
        return false;
    return true;
}

Relay::Mode_t Relay::mode_read()
{
    return mlast;
}

void Relay::loop()
{
    if(changed && (millis() - changed_time) >= 10)
    {
        for(int i = 0; i <= KMAX - 2; i++)//the last 2 is analog mux
        {
            k[i]->reset();
        }
        changed = false;
//        rt_kprintf("relay enter power off\n");
    }
}
void Relay::test()
{
    for(int i = 0; i < 7; i++)
    {
        k[2 * i]->set();
        k[2 * i + 1]->reset();
        PB4.set();
        delay_ms(100);
        
        k[2 * i]->reset();
        k[2 * i + 1]->set();
        PB4.reset();
        delay_ms(500);
    }
}

