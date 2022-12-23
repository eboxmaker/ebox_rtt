#include "BasicRtc.h"


#if DATETIME_USE_PRINT

void BasicRtc::print(Stream &uart)
{
    uart.printf("=========system date time==========\n");
    uart.printf("%04d-%02d-%02d %02d:%02d:%02d weed:%d\n",dateTime.year,dateTime.month,dateTime.day,dateTime.hour,dateTime.minute,dateTime.second,dateTime.dayOfWeek());
    uart.printf("===================================\n");
}
#endif
