#ifndef __CONFIGLOADER_H
#define __CONFIGLOADER_H

#include "ebox_iflash.h"


class CfgLoader
{
public:
    CfgLoader(Flash *handler);
    void begin(uint32_t start,uint16_t size);
    void load(void *ptr);
    void save(void *ptr);
private:
    Flash *f;
        
};
#endif
