#ifndef __BASICRTC_H
#define __BASICRTC_H

#include "string.h"
#include "DateTime.h"


extern const unsigned char day_code1[];
extern const unsigned int day_code2[];
extern const unsigned char year_code[];
extern const unsigned char  table_week[]; //月修正数据表
extern const  char  *sky[];//天干
extern const  char  *earth[];//地支
extern const  char  *zodiac_table[12];
extern const  char  *monthcode[];//农历月份
extern const float CENTURY_ARRAY[2][24];
extern const char *JieQiStr[];


        
class BasicRtc
{
public:
    BasicRtc(){dateTime = DateTime();}
    virtual void    begin()  = 0;
    virtual void    begin(DateTime &dt)  = 0;
    virtual void    update()  = 0;
    virtual void    set(DateTime &dt) = 0;
    virtual DateTime    now() = 0;

#if DATETIME_USE_PRINT
    void            print(Stream &uart);            
#endif
    
public:
    DateTime      dateTime;
};

#endif
