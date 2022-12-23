#include "log.h"
#include "./storage/filesystem/FileSystem.h"
#include "./storage/filesystem/FAT/FATFileSystem.h"
#include "./storage/filesystem/file.h"
#include "./storage/filesystem/dir.h"
#include "mqtt.h"
Log elog;
#include "rthw.h"


#define EBOX_DEBUG_LOG_ENABLE       true


#if EBOX_DEBUG_LOG_ENABLE
#define logDebug(...)  rt_kprintf("[elog]:%d: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define logDebug(...)
#endif
#define SDCARD ssd

int Log::begin()
{
    int ret;
    Dir dir;
    logDebug("==========log begin====================\n");
    mutex = rt_mutex_create("log", RT_IPC_FLAG_FIFO);
    if (mutex == RT_NULL)
    {
        logDebug("create dynamic mutex failed.\n");
    }   

    return ret;
}
int Log::creat_dir(DateTime &dt)
{
    int ret;
    Dir dir;
    uint32_t last = millis();
    
    String path("/log");
    path += "/";
    path += dt.toString(DateTime::YY_MM_DD,DateTime::Sep4);
    path += "/";
    path += dt.hour;
    //create dir
    ret = dir.open(&fs,path.c_str());
    logDebug("[%d]dir.open[%s]\n",ret,path.c_str());
    if(ret)
    {
        //create dir
        path = ("/log");
        ret = dir.open(&fs,path.c_str());
        logDebug("[%d]dir.open[%s]\n",ret,path.c_str());
        if(ret)
        {
            ret = fs.mkdir(path.c_str(),O_CREAT);
            logDebug("[%d]fs.mkdir[%s]\n", ret,path.c_str()); 
        }
        dir.close();
        logDebug("[%d]dir.close[%s]\n", ret,path.c_str());

        
        path += "/";
        path += dt.toString(DateTime::YY_MM_DD,DateTime::Sep4);
        //create dir
        ret = dir.open(&fs,path.c_str());
        logDebug("[%d]dir.open[%s]\n",ret,path.c_str());
        if(ret)
        {
            ret = fs.mkdir(path.c_str(),O_CREAT);
            logDebug("[%d]fs.mkdir[%s]\n", ret,path.c_str()); 
        }
        dir.close();
        logDebug("[%d]dir.close[%s]\n", ret,path.c_str());

        
        path += "/";
        path += dt.hour;
        //create dir
        ret = dir.open(&fs,path.c_str());
        logDebug("[%d]dir.open[%s]\n",ret,path.c_str());
        if(ret)
        {
            ret = fs.mkdir(path.c_str(),O_CREAT);
            logDebug("[%d]fs.mkdir[%s]\n", ret,path.c_str()); 
        }
        dir.close();
        logDebug("[%d]dir.close[%s]\n", ret,path.c_str());

    }
    ret = dir.close();
    logDebug("[%d]dir.close[%s]\n", ret,path.c_str());
    logDebug("===== dir cost: [%d]======\n", millis() - last);

    return ret;


}
String Log::make_string(RunSample *node)
{
    int i = 0;
    MqttMsgBase msgRun;
    msgRun.topic = event_property_post;
    cJSON * data = cJSON_CreateObject();  
    
    String str = node->dt.toString();
    cJSON_AddStringToObject(data,"rd_datetime",str.c_str());
    cJSON_AddNumberToObject(data,"rd_type",node->type); 
    cJSON_AddNumberToObject(data,"rd_round",node->round); 

    cJSON_AddNumberToObject(data,"rd_a_dc_v",ebox_round(node->val[i++],3)); 
    cJSON_AddNumberToObject(data,"rd_a_ac_v",ebox_round(node->val[i++],3));  
    cJSON_AddNumberToObject(data,"rd_a_dc_i",ebox_round(node->val[i++],3));     
    cJSON_AddNumberToObject(data,"rd_a_ac_i",ebox_round(node->val[i++],3)); 
    
    cJSON_AddNumberToObject(data,"rd_b_dc_v",ebox_round(node->val[i++],3)); 
    cJSON_AddNumberToObject(data,"rd_b_ac_v",ebox_round(node->val[i++],3));  
    cJSON_AddNumberToObject(data,"rd_b_dc_i",ebox_round(node->val[i++],3));     
    cJSON_AddNumberToObject(data,"rd_b_ac_i",ebox_round(node->val[i++],3)); 
    
    cJSON_AddNumberToObject(data,"rd_off_n_dc_v",ebox_round(node->val[i++],3)); 
    cJSON_AddNumberToObject(data,"rd_off_a_dc_v",ebox_round(node->val[i++],3));  
    cJSON_AddNumberToObject(data,"rd_off_b_dc_v",ebox_round(node->val[i++],3));     
    cJSON_AddNumberToObject(data,"rd_off_zn_dc_v",ebox_round(node->val[i++],3)); 
    
    cJSON_AddNumberToObject(data,"rd_battery",ebox_round(run.bat_vol,3)); 
    cJSON_AddNumberToObject(data,"rd_cpu",ebox_round(sys.cpu,2)); 
    cJSON_AddNumberToObject(data,"rd_mem",ebox_round(sys.mem.percent,2)); 
    cJSON_AddNumberToObject(data,"rd_temp",ebox_round(adc1.read_temp(),1)); 
    
    cJSON_AddNumberToObject(data,"rd_lng",gps.location.lng()); 
    cJSON_AddNumberToObject(data,"rd_lat",gps.location.lat()); 
    cJSON_AddNumberToObject(data,"rd_csq",gm5.csq); 

    
    msgRun.make_payload(data);

    return msgRun.payload;
}
int Log::add(String &str,DateTime &dt)
{
    int ret;
    int br;
    int bw;
    char buf[128];
    File file;
    Dir dir;
    uint32_t mark = millis();
    rt_err_t err = rt_mutex_take(mutex, 200);
    if(err)
    {
        logDebug("===========try get rt_mutex_take timeout=============\n");
        return -1;
    }    

    logDebug("===========try add=============\n");
    ret = fs.mount(&SDCARD);
    logDebug("[%d]fs.mount\n",ret);
    if(!ret)
    {

        creat_dir(dt);
        
        
        //create file
        String path("/log");
        path += "/";
        path += dt.toString(DateTime::YY_MM_DD,DateTime::Sep4);
        path += "/";
        path += dt.hour;
        path += "/";
        path += dt.toString(DateTime::YYYY_MM_DD_HH_MM_SS,DateTime::Sep4);
        path += ".txt";
        ret = file.open(&fs,path.c_str(),O_WRONLY | O_CREAT);
        logDebug("[%d]file.open create[%s]\n", ret,path.c_str());
        if(!ret)
        {
            bw = file.write(str.c_str(),str.length());
            logDebug("[%d]byte file.write\n", bw);
            if(bw == 0){
                ret = -2;
            }
            else
            {
                logDebug("log added:[%s]\n",path.c_str());
            }
            ret = file.close();
            logDebug("[%d]file.close\n",ret);
        }
    }
    ret = fs.unmount();
    logDebug("[%d]fs.unmount\n",ret);
    rt_mutex_release(mutex);
    logDebug("cost:%d\n",millis() - mark);
    return ret;
    
    
}

int Log::add(RunSample *node)
{
    int ret=0;
    int br;
    int bw;
    char buf[128];
    File file;
    Dir dir;
    uint32_t mark = millis();
    rt_err_t err = rt_mutex_take(mutex, 200);
    if(err)
    {
        logDebug("===========try get rt_mutex_take timeout=============\n");
        return -1;
    }    

    
    String msg = make_string(node);

    logDebug("===========try add=============\n");
    ret = fs.mount(&SDCARD);
    logDebug("[%d]fs.mount\n",ret);
    if(!ret)
    {
       
        creat_dir(node->dt);
        
        
        //create file
        String path("/log");
        path += "/";
        path += node->dt.toString(DateTime::YY_MM_DD,DateTime::Sep4);
        path += "/";
        path += node->dt.hour;
        path += "/";
        path += node->dt.toString(DateTime::YYYY_MM_DD_HH_MM_SS,DateTime::Sep4);
        path += ".txt";

        ret = file.open(&fs,path.c_str(),O_WRONLY | O_CREAT);
        logDebug("[%d]file.open create[%s]\n", ret,path.c_str());
        if(!ret)
        {
            bw = file.write(msg.c_str(),msg.length());
            logDebug("[%d]byte file.write\n", bw);
            if(bw == 0){
                ret = -2;
            }
            else
            {
                logDebug("log added:[%s]\n",path.c_str());
            }
            ret = file.close();
            logDebug("[%d]file.close\n",ret);
        }
    }
    ret = fs.unmount();
    logDebug("[%d]fs.unmount\n",ret);
    rt_mutex_release(mutex);
    logDebug("========log add cost [%d]=========\n",millis() - mark);
    return ret;
    
}
bool Log::find(const char *path)
{
    int br;
    int ret;
    Dir dir;
    File file;
    int file_cnt = 0;
    ret = dir.open(&fs,path);
    logDebug("[%d]dir.open[%s]\n",ret,path);
    if(ret)
    {
        UART.printf("open dir err:%d\n",ret);
        return false;
    }
    else
    {
    }
    struct dirent de;
    while(1)
    {
        retry:
        String str = "";
        ret = dir.read(&de);
        if(ret && over == false)
        {   
            file_cnt++;
            str += path;
            str += "/";
            str += de.d_name;
            if(de.d_type == DT_DIR)
            {
                logDebug("%s\n", de.d_name);
                find(str.c_str());
            }
            else if(de.d_type == DT_REG)
            {
                ret = file.open(&fs,str.c_str());
                logDebug("[%d]file.open:[%s]\n",ret,str.c_str());
                br = file.read(buf,1024);
                if(br < 1024)
                {
                    data = buf;
                    logDebug("read data:[%d]\n",data.length());
                    over = true;
                }
                else
                {
                    logDebug("file too long :[%s]\n");
                }
                file.close();
                logDebug("[%d]file.close[%s]\n",ret,str.c_str());
                ret = fs.remove(str.c_str());
                logDebug("[%d]fs.remove :[%s]\n",ret,str.c_str());
                if(over)
                {
                   break;
                }
            }
        }
        else
        {
            if(!file_cnt)
            {
                ret = fs.remove(path);
                logDebug("[%d]fs.remove :[%s]\n",ret,path);
            }
            break;
        }
    } 
    ret = dir.close();
    logDebug("[%d]dir.close[%s]\n",ret,path);
    return over;
}

String Log::get()
{  
    int br;
    int ret;
    Dir dir;
    File file;
    uint32_t last = millis();
    data = "";
    rt_err_t err = rt_mutex_take(mutex, 200);
    if(err)
    {
        logDebug("===========try get rt_mutex_take timeout=============\n");
        return "";
    }    
    ret = fs.mount(&SDCARD);
    logDebug("[%d]fs.mount\n",ret);
    if(!ret)
    {
        over = false;
        if(find("/log"))
        {
        }
        else
        {
            data = "";
        }
    }
    ret = fs.unmount();
    logDebug("[%d]fs.unmount;datalen:[%d]\n",ret,data.length());
    logDebug("========log get cost [%d]=========\n",millis() - last);
    rt_mutex_release(mutex);

    return data;
}
bool Log::empty()
{
    return _empty;
}

void Log::test()
{
    int ret;
    String str = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
                  1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
    1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
    1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
    1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
    add(str,gdt);
    get();
    delay_ms(2000);
}

int Log::scan()
{
    int msg_cnt;
    int ret;
    Dir dir;
    ret = fs.mount(&SDCARD);
    if(ret)
    {
        logDebug("[err]SDCARD mount err:%d\n",ret);
        return -1;
    }

    ret = dir.open(&fs,"0:/log");
    if(ret)
    {
        logDebug("open dir err:%d\n",ret);
    }
    struct dirent de;
    msg_cnt = 0;
    while(1)
    {
        String str = "";
        ret = dir.read(&de);
        if(ret)
        {
            if(de.d_type == DT_REG)
            {
                msg_cnt++;
                logDebug("file[%d]:[%s]\n", msg_cnt,de.d_name);
            }
        }
        else
        {
            break;
        }
    }
    dir.close();
    fs.unmount();
    rt_mutex_release(mutex);
    return msg_cnt;
}



