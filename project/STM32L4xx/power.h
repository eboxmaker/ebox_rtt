#ifndef __POWER_H
#define __POWER_H

#include "ebox.h"


typedef enum {
    PwrDigital,
    PwrAnalog,
    PwrGps,
    PwrBatCheck,
    PwrAll
}Power_t;

void power_init();
void power_on(Power_t p);
void power_off(Power_t p);
void power_ctr(Power_t p,bool state);


#endif

