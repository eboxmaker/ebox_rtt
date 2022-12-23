#include "ripple.h"
#include "ebox_math.h"


Ripple::Ripple(int wnd)
{
    window = wnd;
    threshold = 20;
    keep = 100;
    handler = 0;
    reset();

}
void Ripple::input(float var)
{
    buf[counter++] = var;
    counter %= window; 

    stdev = eBox::stdev(buf,window);
    float rp = stdev ;
    // Serial.printf("\r\nstdev\t%0.3f\tripple:%0.3f\r\n",stdev,rp);
    switch(state)
    {
        case 0:
            if(rp < threshold)
            {
                state = 1;
                last_stable_time = millis();
            }
            break;
        case 1:
            if(rp > threshold)
            {
                state = 0;
            }
            else if(millis() - last_stable_time > keep)
            {
                if(handler)
                {
                    handler(rp);
                }
                stable_state = true;
                state = 2;
            }
        case 2:
            if(rp > threshold)
            {
                state = 0;
                stable_state = false;
            }
            break;
        default:
            state = 0;break;
    }

}
void Ripple::reset()
{
    for(int i = 0; i < window; i++)
    {
        buf[i] = random(0,1000);
    }
    state = 0;
    stable_state = false;
    counter = 0;
}
float Ripple::get()
{
//    float rp = roundf(stdev) ;
    float rp = (stdev);
    if(rp > 9999) rp = 9999;
    return rp;
}
float Ripple::get_avg()
{
    avg = eBox::avarage(buf,window);
    // Serial.printf("\r\navg\t%0.6f\r\n",avg);
    return avg;
}

void Ripple::attach(RippleCallBack func)
{
    handler = func;
}
