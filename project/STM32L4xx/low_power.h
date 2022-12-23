#ifndef __LOW_POWER_H
#define __LOW_POWER_H



#include "ebox_core.h"

#define PWR_BIT_INDEX_REGU      1
#define PWR_BIT_INDEX_CONT      2
#define PWR_BIT_INDEX_TEST      3
#define PWR_BIT_INDEX_CAL       4
#define PWR_BIT_INDEX_HISTORY   5

void lowpower_standby_lock(uint8_t index);
void lowpower_standby_unlock(uint8_t index);
void lowpower_init();
void lowpower_standby_enter(uint8_t index);

uint32_t pwr_flag_get();

#endif 


