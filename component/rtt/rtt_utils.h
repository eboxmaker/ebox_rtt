#ifndef __RTTUTILS_H
#define __RTTUTILS_H

#include "rtthread.h"



typedef struct 
{
    rt_uint32_t total;
    rt_uint32_t used;
    rt_uint32_t max_used;
    float percent;
    float max_percent;
}MemInfo_t;

typedef struct 
{
    MemInfo_t mem;
    float cpu;
}SysInfo_t;

extern SysInfo_t sys;

long list_thread(void);
void thread_info(rt_thread_t rtt);
void print_memory_info(void);
MemInfo_t get_memory_info(void);

    
    
extern "C"{

    void cpu_usage_init(void);
    void cpu_usage_get(rt_uint8_t *major, rt_uint8_t *minor);
    float cpu_usage_get_float();
        
}
#endif
