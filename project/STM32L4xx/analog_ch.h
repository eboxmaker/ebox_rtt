#ifndef __ANALOG_CH_H
#define __ANALOG_CH_H

#include "relay.h"


float analog_read(Relay::Mode_t ch);
float analog_get(Relay::Mode_t ch,uint16_t timeout = 1500);


#endif

