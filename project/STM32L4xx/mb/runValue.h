#ifndef _RUNVALUE_H
#define _RUNVALUE_H


#include "ebox_core.h"
#include "DateTime.h"
#include "./linkedlist/linkedlist.h"

class RunSample
{
    public:
        float val[12];//
        DateTime dt;
        uint16_t round;
        int     type;
};
class RunList
{
    public:
        RunList();
        void begin();
        void add(RunSample *node);
        RunSample *get();
        int size();
    public:
        LinkedList<RunSample*> list;
        rt_mutex_t mutex;

};
extern RunList runList;



class RunValue
{
public:
    RunValue(){};
    
    uint16_t r0_hex;//1
    uint16_t r1_gain;//14
    float   r2_voltage;//
    float   r4_rpl;//
        
    float r6_adv;//
    float r8_aav;//
    float r10_adc;//
    float r12_aac;//
        
    float r14_bdv;//
    float r16_bav;//
    float r18_bdc;//
    float r20_bac;//
        
    float r22_ndv;//
    float r24_nadv;//
    float r26_nbdv;//
    float r28_ncuznv;//
        
    DateTime dt;
    
    DataU16_t net_state;
    DataU16_t net_server;
    float bat_vol;//

        
    void begin();
//    int buf_to_para();
    int para_to_buf();
    bool warnning_check();

public:
    uint8_t buf[256];
    uint8_t len;
};

extern RunValue run;

#endif
