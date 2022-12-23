#ifndef __RELAY_H
#define __RELAY_H

#include "ebox.h"


//��Ȼ��λ��ѹ
//1cm2��Ƭͨ��ֱ����λ
//1cm2��Ƭ�ϵ�ֱ����λ
//1cm2��Ƭ�������ŵ�λ
//1cm2��Ƭ��ʴ��Ƭ��������
//1cm2��Ƭ��ʴ��Ƭֱ������
//10cm2��Ƭͨ��ֱ����λ
//10cm2��Ƭ�ϵ�ֱ����λ
//10cm2��Ƭ�������ŵ�λ
//10cm2��Ƭ��ʴ��Ƭֱ������
//CuSO4�α�Zn�缫��ĵ�λ
//�󲿷�ʱ�䴦��ƽ���͹���״̬
//��ʼ״̬




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

