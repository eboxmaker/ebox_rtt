/**
  ******************************************************************************
  * @file    iflash.h
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef   __CONFIG_H__
#define   __CONFIG_H__


#include "ebox_core.h"
#include "ebox_flash.h"
#include "ebox_uart.h"

#include "calibration.h"

//#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_CFG_ENABLE       true
#define EBOX_DEBUGCFG_ENABLE_ERR   true
//#endif

#if EBOX_DEBUG_CFG_ENABLE
#define cfgDebug(...)  rt_kprintf("[cfg:%03d]: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define cfgDebug(...)
#endif

#if EBOX_DEBUG_CFG_ENABLE_ERR
#define cfgDebugErr(...)  rt_kprintf("[cfg:%03d]: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define cfgDebugErr(...)
#endif


class ConfigServer
{
public:
///////////258 byte///////////////////////////////
    char name[32];
    char password[32];
    char ip[32];
    uint16_t port;
    uint16_t timeout;
    uint16_t ssl_tls;
//    uint16_t qos;
//    char topic_pub[64];
//    char topic_sub[64];
//    char msg_will[64];
///////////////////////////////////////////
    int buf_to_para();
    int para_to_buf();
    
    void begin();
    void print();

    uint8_t buf[300];
    uint16_t len;
    uint16_t cnt1,cnt2;
};
class ConfigThreshold
{
public:
///////////258 byte///////////////////////////////
    float off_dc_min;
    float off_dc_max;
    float on_dc_max;
    float on_ac_max;

///////////////////////////////////////////
    int buf_to_para();
    int para_to_buf();
    
    void begin();
    void print(Uart &uart);

    uint8_t buf[300];
    uint16_t len;
};
class Config
{
public:
    typedef enum{
        SampleRegu = 0,
        SampleContinuous,
    }SampleMode_t; 
    
    typedef enum{
        GpsOff = 0,
        GpsOn ,
    }GpsCtr_t;
public:
    Config(){};
    void begin();
    void load_default();
    void save();
    void read();
    int frame_to_para();
    int para_to_frame();
    void para_limit();
    uint16_t frame_get_flag();
    bool frame_check(uint8_t *buf,uint16_t len);
    void print(Uart &uart);
        
    void update_backup();
        


public:
    
    uint8_t frame_backup[1024];
    uint8_t frame[1024];
    uint16_t frame_len;


    uint16_t frame_z0_len;
    DataU16_t flag;//存储标志位

    

    //z1  config
    uint16_t frame_z1_len;
    SampleMode_t sampleMode;
    uint16_t sample_regu_interval;
    uint16_t sample_regu_round;
    uint16_t sample_cont_duration;
    uint16_t sample_cont_interval;
    GpsCtr_t gps_ctr;
    uint16_t pipe_on_time;
    uint16_t pipe_off_time;
    uint16_t pipe_off_after;
    uint16_t modbus_id;

    //z2  factory config
    uint16_t frame_z2_len;
    //z3   mqtt server config
    uint16_t frame_z3_len;
    //z4 calibration config
    uint16_t frame_z4_len;
    
private:
//    PFlash *flash;
};


class Dev
{
    public:
        void begin();
        int para_to_buf();
        void print(Stream &uart);

    public:
        char id[32];
        char model[32];
        uint16_t type;
        uint16_t version;
        char iccid[32];
        char imei[32];    
        uint8_t buf[256];
        uint16_t len;
};

extern Dev dev;
extern Config config;
extern ConfigServer config_server;
extern ConfigThreshold config_threshold;
extern ConfigCalibration config_cal;
#endif
