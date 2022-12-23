#ifndef __EBOX_MATH_H
#define __EBOX_MATH_H
#include "ebox_type.h"

namespace eBox{
    

template <typename T>
T max_in_buf(T *ptr, uint16_t size)
{
T temp;
temp = ptr[0];
for(uint16_t i = 1; i < size; i++)
{
   if(ptr[i] > temp) temp = ptr[i] ;
}
return temp;
}    

template <typename T>
T min_in_buf(T *ptr, uint16_t size)
{
    T temp;
    temp = ptr[0];
    for(uint16_t i = 1; i < size; i++)
    {
       if(ptr[i] < temp) temp = ptr[i] ;
    }
    return temp;
}    

//???
double avarage(float *ptr, size_t size);

//?? s= ?(x-u)^2/N;;;
double var(float *ptr, size_t size);

//???(???) o = sqrt(var);
double stdev(float *ptr, size_t size);

};

#endif


