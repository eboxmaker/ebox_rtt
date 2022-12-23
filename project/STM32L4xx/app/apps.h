#ifndef __APP_H
#define __APP_H
#include "bsp_ebox.h"
#include "ADS1120.h"
#include "relay.h"
#include "ripple.h"



//#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_APP_ENABLE       true
#define EBOX_DEBUG_APP_ENABLE_ERR   true
//#endif


#if EBOX_DEBUG_APP_ENABLE
#define appDebug(...)  rt_kprintf("[app:%03d]: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define appDebug(...)
#endif

#if EBOX_DEBUG_APP_ENABLE_ERR
#define appDebugErr(...)  rt_kprintf("[app:%03d]: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define appDebugErr(...)
#endif


extern rt_sem_t sem_save_over;




int app_create();
int app_utils_create();
int app_mb_create();
int app_adc_create();
int app_worker_create();
int app_gps_create();
int app_mqtt_create();
int app_history_create();
int app_save_create();


int app_regu_create();
int app_cont_create();
int app_test_create();
int app_cal1_create();

void app_regu_delete();
void app_cont_delete();
void app_test_delete();
void app_cal1_delete();

//work mode
#define WORK_REGU 0
#define WORK_CONT 1
#define WORK_TEST 2
#define WORK_CAL  3


//mode reg 
#define REG_MODE 220

//cal reg
#define REG_CH 221
#define REG_SAVE 222
#define REG_ADD_DATA 223


//test reg
#define REG_TEST_CH 240


void set_workmode(uint8_t mode);
void set_relay_ch(Relay::Mode_t ch);


void loop_cal();
void cal_set_ch(uint8_t ch);
void cal_add_data(float value);
void cal_save(bool val);

void test_set_ch(uint8_t ch);


float cal_get(uint8_t index);//index: ch low two bit


#endif
