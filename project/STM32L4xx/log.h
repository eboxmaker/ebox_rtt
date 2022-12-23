#ifndef __LOG_H
#define __LOG_H


#include "bsp_ebox.h"
#include "datetime.h"
#include "runValue.h"

class Log
{
    public:
        int begin();
        int creat_dir(DateTime &dt);
//        int add(String &str);
        int add(String &str,DateTime &dt);
        int add(RunSample *node);
        String make_string(RunSample *node);

        String get();
        bool find(const char *path);
        bool empty();
        void test();
    private:
        int scan();
        bool _empty;
        String data;
        bool over;

    public:
        char buf[1024];
        rt_mutex_t mutex;

};

extern Log elog;

#endif

