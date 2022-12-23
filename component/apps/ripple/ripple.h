#ifndef __RIPPLE_H
#define __RIPPLE_H

#include "ebox_core.h"

#define MAX_RIPPLE_SAMPLE 128

typedef void(*RippleCallBack)(float);
class Ripple{
    public:
        Ripple(int wnd = 10);
        void input(float var);
        void reset();
        float get();
        float get_avg();
        void attach(RippleCallBack func);
        void set_threshold(float th){threshold = th;}
        void set_keep(uint16_t kp){keep = kp;}
        bool is_stable()
        {
            return stable_state;
        };
    
    private:
        float threshold;
        uint16_t keep;
        uint32_t last_time;
        uint16_t counter;
        uint8_t window;
        RippleCallBack handler;
    
        float buf[MAX_RIPPLE_SAMPLE];
        float avg;
        float stdev;

        int state;
        uint32_t last_stable_time;
        bool stable_state;


    
};


//extern Ripple ripple;

#endif

