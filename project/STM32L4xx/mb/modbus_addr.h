#ifndef __MODBUS_ADD_H
#define __MODBUS_ADD_H



#include "modbusSerial.h"


//#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_MB_ENABLE       true
#define EBOX_DEBUG_MB_ENABLE_ERR   true
//#endif

#if EBOX_DEBUG_MB_ENABLE
#define mbDebug(...)  rt_kprintf("[mb:%03d]: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define mbDebug(...)
#endif

#if EBOX_DEBUG_MB_ENABLE_ERR
#define mbDebugErr(...)  rt_kprintf("[mb:%03d]: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define mbDebugErr(...)
#endif



extern bool hreg_writed_flag;

extern ModbusSerial mb;
extern uint8_t sample_mode;



void mb_init();
void mb_task();


void modbus_callback_no_para();
void modbus_callback(uint8_t fun,uint8_t regStart,uint8_t regCnt);

void init_mb_addr_table();

uint16_t mb_to_frame();
void mb_frame_to_hreg();
void mb_run_to_ireg();
void mb_update_ireg_read_only();


void mb_to_para_buf(uint8_t *ptr);
void set_sample_mode(uint8_t mode);



float hreg_to_float(uint16_t reg_start);
void float_to_Ireg(uint16_t reg_start,float value);
void float_to_hreg(uint16_t reg_start,float value);


#endif

