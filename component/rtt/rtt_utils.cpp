#include <rthw.h>
#include <rtthread.h>
#include <string.h>
#include "rtt_utils.h"


SysInfo_t sys;
void thread_info(rt_thread_t rtt)
{
    rt_uint8_t *ptr;

    ptr = (rt_uint8_t *)rtt->stack_addr;
    while (*ptr == '#')ptr ++;
    int use = (rtt->stack_size - ((rt_ubase_t) ptr - (rt_ubase_t) rtt->stack_addr)) * 100 / rtt->stack_size;

    rt_kprintf("***************************\n");
    rt_kprintf("name       :%s\n",rtt->name);
    rt_kprintf("stack size :%d\n",rtt->stack_size);
    rt_kprintf("sp         :0X%08X\n",rtt->sp);
    rt_kprintf("stack_addr :0X%08X\n",rtt->stack_addr);
    rt_kprintf("max used   :%d%%\n",use);
    rt_kprintf("***************************\n");
}


void print_memory_info(void)
{

    rt_uint32_t total,used,max_used;
    rt_memory_info(&total,&used,&max_used);    
    float percent = 100*(float)max_used/(float)total;
    rt_kprintf("***************************\n");
    rt_kprintf("total       :%d\n",total);
    rt_kprintf("used        :%d\n",used);
    rt_kprintf("max_used    :%d\n",max_used);
    rt_kprintf("percent    :%d%%\n",(int)percent);
    rt_kprintf("***************************\n");
}
MemInfo_t get_memory_info(void)
{
    rt_memory_info(&sys.mem.total,&sys.mem.used,&sys.mem.max_used);    
    sys.mem.percent = 100*(float)sys.mem.used/(float)sys.mem.total;
    sys.mem.max_percent = 100*(float)sys.mem.max_used/(float)sys.mem.total;
    return sys.mem;
}
    
    
