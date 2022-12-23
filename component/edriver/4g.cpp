#include "4g.h"
#include "ebox.h"


#include "bsp_ebox.h"
#include "bsp_rtc.h"

//#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_GM5_ENABLE       true
#define EBOX_DEBUG_GM5_ENABLE_ERR   true
//#endif

#if EBOX_DEBUG_GM5_ENABLE
#define gm5Debug(...)  rt_kprintf("[gm5:%03d]: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define gm5Debug(...)
#endif

#if EBOX_DEBUG_GM5_ENABLE_ERR
#define gm5DebugErr(...)  rt_kprintf("[gm5 err:%03d]: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define gm5DebugErr(...)
#endif


StringArray strSplit(String &str)
{
    StringArray sa;
//    gm5Debug("strSplit:[[%s]]\n",str.c_str());
    int start = 0;
    int len = str.length();
    while(1)
    {
        int index = str.indexOf("\r\n",start);
        if(index == -1) 
        {
            if(start < len)
            {
                sa.add(str.substring(start,len));
            }
            break;
        }
        String sub = str.substring(start,index);
        if(sub != NULL)
        {
            sa.add(sub);
        }
        start = index+2;
    }
    return sa;
}
StringArray strSplit(String &str,const char *key)
{
    StringArray sa;
//    gm5Debug("strSplit:[[%s]]\n",str.c_str());
    int start = 0;
    int len = str.length();
    int key_len = rt_strlen(key);
    while(1)
    {
        int index = str.indexOf(key,start);
        if(index == -1) 
        {
            if(start < len)
            {
                sa.add(str.substring(start,len));
            }
            break;
        }
        String sub = str.substring(start,index);
        if(sub != NULL)
        {
            sa.add(sub);
        }
        start = index+key_len;
    }
    return sa;
}


UsrGM5::UsrGM5(Gpio *_rst,Gpio *_reload,Uart *_uart)
{
    rst = _rst;
    reload = _reload;
    uart = _uart;
    handler = RT_NULL;
    mqtt_state = false;
    net_connected = false;
//    rt_mutex_delete(mutex);
    mState = Gm5StateNone;
}

void UsrGM5::begin()
{
    rst->mode(OUTPUT_PP_PD);
    reload->mode(OUTPUT_PP_PD);
    rst->reset();
    reload->reset();
    uart->begin(115200);
    uart->setTimeout(500);
//    PA9.mode(INPUT_PU);
//    PA10.mode(INPUT_PU);
    mutex = rt_mutex_create("dmutex", RT_IPC_FLAG_FIFO);
    if (mutex == RT_NULL)
    {
        rt_kprintf("create dynamic mutex failed.\n");
    }
    mState = Gm5StateInterfaceInited;
}



void UsrGM5::paraseLoop(String &str)
{

    int len = str.length();
    if(str.startsWith("+CEREG"))
    {
        String sub = str.substring(str.indexOf(':') + 1,len);
        cereg = sub.toInt();
//        gm5Debug("CEREG:%d\n",cereg);
    }
    else if(str.startsWith("+CSQ"))
    {
        String sub = str.substring(str.indexOf(':') + 1,len);
        String sub1 = sub.substring(0,sub.indexOf(',') );
        String sub2 = sub.substring(sub.indexOf(',') + 1,sub.length());
        csq = sub1.toInt();
        int ber = sub2.toInt();
//        gm5Debug("CSQ:%d, ber:%d\n",csq,ber);
    }
    else if(str.startsWith("+SYSINFO"))
    {
        String sub = str.substring(str.indexOf(':') + 1,len);
        String sub1 = sub.substring(0,sub.indexOf(',') );
        sysinfo_mode = sub.substring(sub.indexOf(',') + 1,sub.length());
        int net = sub1.toInt();
//        gm5Debug("SYSINFO:net:%d, sysinfo mode:%s\n",net,sysinfo_mode.c_str());
    }
    else if(str.startsWith("+CGACT"))
    {
        bool state;
        String sub = str.substring(str.indexOf(':') + 1,len);
        String sub1 = sub.substring(0,sub.indexOf(',') );
        String sub2 = sub.substring(sub.indexOf(',') + 1,sub.length());
//        gm5Debug("sub:%s;sub1:%s;sub2:%s\n",sub.c_str(),sub1.c_str(),sub2.c_str());

        cgact_cid = sub1.toInt();
        cgact_state = sub2.toInt();
        if(cgact_state == 1)
        {
            state = true;
        }
        else
        {
            gm5DebugErr("net_connected fault: %d \n",cgact_state);
            state = false;
        }
        
        if(state != net_connected)
        {
            net_connected = state;
            if(conn_handler)
                conn_handler(Gm5NetConn,state);
        }        
        gm5Debug("cgact_cid:%d;cgact_state:%d,ip:%s\n",cgact_cid,cgact_state,sub.c_str());
    }
    else if(str.startsWith("+MQTTSTA"))
    {
        bool state = str.substring(str.indexOf(':') + 1,len).toInt();
        if(state != mqtt_state)
        {
            mqtt_state = state;
            if(conn_handler)
                conn_handler(Gm5MqttConn,state);
        }
        if(!mqtt_state)
            gm5DebugErr("mqtt_connected fault: %d ",mqtt_state);

//        gm5Debug("mqtt_state:%s\n",str.c_str());
    }
    else if(str.startsWith("+MQTTPUB:"))
    {
        gm5Debug("MQTTPUB:%s\n",str.c_str());
    }
    else if(str.startsWith("+MQTTPUBLISH"))
    {
//        gm5Debug("+MQTTPUBLISH:%s\n",str.c_str());
        String sub = str.substring(str.indexOf(':') + 1,len);
        if(handler)
            handler(sub);    
    }
    else if(str.startsWith("+MQTTSUBUNSUB"))
    {
        gm5Debug("MQTTSUBUNSUB:%s\n",str.c_str());
    }

    else if(str.startsWith("+CSCON"))
    {
        String sub = str.substring(str.indexOf(':') + 1,len);
        cscon = sub.toInt();
//        gm5Debug("CSCON:%d\n",cscon);
    }
    else if(str.startsWith("+CREG"))
    {
        String sub = str.substring(str.indexOf(':') + 1,len);
        creg = sub.toInt();
//        gm5Debug("CREG:%d\n",creg);
    }
    else if(str.startsWith("+CCLK"))
    {
        String sub = str.substring(str.indexOf('\"') + 1,len);
        clk_dt = "20";
        clk_dt += sub.substring(0,sub.indexOf('+') );
        gdt.parse(clk_dt);
        rtc_set_dt(gdt);
        gm5Debug("CCLK:%s\n",clk_dt.c_str());
        gdt.print(UART);
    }
    else if(str.startsWith("+ICCID"))
    {
        iccid = str.substring(str.indexOf(':') + 1,len);
        gm5Debug("ICCID:%s\n",str.c_str());
    }
    else if(str.startsWith("+IMEI"))
    {
        imei = str.substring(str.indexOf(':') + 1,len);
        gm5Debug("IMEI:%s\n",str.c_str());
    }
    else if(str.startsWith("+E_UTRAN"))
    {
//        gm5Debug("E_UTRAN:xxx\n");
    }
    else if(str.startsWith("WH-LTE"))
    {
        gm5Debug("WH-LTE-7S1-CT:restart!!!!!!!!!!!!!!\n");
        mState = Gm5StateStarted;
    }
    else if(str.startsWith("+CTZV"))
    {
        String sub = str.substring(str.indexOf(':') + 1,len);
        ctzv = sub.toInt();
        gm5Debug("CTZV:%d\n",ctzv);
    }
    else
    {
//        gm5Debug("[paraseLoop]:-------[%s]\n",str.c_str());
    }
    
}


int UsrGM5::paraseErr(StringArray &sa)
{
    int ret = -1;
    String str;
    for(int i = 0; i < sa.size(); i++)
    {
        str = sa.get(i);
        if(str == "OK")
        {
//            gm5Debug("[paraseErr]:ok\n");
            ret = 0;
        }
        else if(str.startsWith("+CME"))
        {
            int len = str.length();
            String sub = str.substring(str.indexOf(':') + 1,len);
            gm5DebugErr("%s,%d\n",sub.c_str(),sub.toInt());
            ret = sub.toInt();
        }
        else
        {
//            gm5Debug("[paraseErr]:---->paraseLoop(\"%s\")\n",str.c_str());
            paraseLoop(str);
        }
    }
    return ret;

}

int UsrGM5::kick()
{
    int ret;
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    uart->print("AT\r\n");
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    ret = paraseErr(sa);
    rt_mutex_release(mutex);
    return ret;
}

String UsrGM5::wkmod(String mod)
{
    String mod_str;
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    if(mod=="")
        uart->print("AT+WKMOD?\r\n");
    else
    {
        uart->print("AT+WKMOD=");
        uart->print(mod);
        uart->print("\r\n");
    }
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    int ret = paraseErr(sa);
    
    if(mod=="")
    {
//        [0]:at+WKMOD?
//        [1]:+WKMOD:CMD
//        [2]:OK
        if(ret==0)
        {
            String str = sa.get("+WKMOD");
            mod_str = str.substring(str.indexOf(':') +1, str.length());
        }
    }
    else
    {
//        [0]:at+WKMOD=NET123
//        [1]:+CME ERROR:53
        if(ret==0)
        {
            mod_str = mod;
        }
        else
        {
            mod_str = sa.get("+CME");
        }
    }

    rt_mutex_release(mutex);
    return mod_str;

}
int UsrGM5::set_cgact(uint8_t state,uint8_t cid)//pdp context active
{
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    uart->printf("AT+CGACT=%d,%d\r\n",state,cid);
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    int ret = paraseErr(sa);
    if(ret)
    {
        gm5Debug("set_cgact err:%d\n",ret);
    }
    rt_mutex_release(mutex);
    return ret;
}
int UsrGM5::get_cgact()//pdp context active
{
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    uart->printf("AT+CGACT?\r\n");
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    int ret = paraseErr(sa);
    if(ret)
    {
        gm5Debug("get_cgact err:%d\n",ret);
    }
    rt_mutex_release(mutex);
    return ret;
}


int  UsrGM5::get_iccid()
{
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    uart->printf("AT+ICCID?\r\n");
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    int ret = paraseErr(sa);
    if(ret)
    {
        gm5Debug("get_iccid err:%d\n",ret);
    }
    rt_mutex_release(mutex);
    return ret;
}
int  UsrGM5::get_imei()
{
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    uart->printf("AT+IMEI?\r\n");
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    int ret = paraseErr(sa);
    if(ret)
    {
        gm5Debug("get_imei err:%d\n",ret);
    }
    rt_mutex_release(mutex);
    return ret;
}
int  UsrGM5::get_csq()
{
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    uart->printf("AT+CSQ\r\n");
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    int ret = paraseErr(sa);
    if(ret)
    {
        gm5Debug("get_csq err:%d\n",ret);
    }
    rt_mutex_release(mutex);
    return ret;
}int  UsrGM5::get_sysinfo()
{
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    uart->printf("AT+SYSINFO?\r\n");
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    int ret = paraseErr(sa);
    if(ret)
    {
        gm5Debug("get_sysinfo err:%d\n",ret);
    }
    rt_mutex_release(mutex);
    return ret;
}
int UsrGM5::get_cclk()
{
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    uart->printf("AT+CCLK?\r\n");
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    int ret = paraseErr(sa);
    if(ret)
    {
        gm5Debug("get_cclk err:%d\n",ret);
    }
    rt_mutex_release(mutex);
    return ret;
}

int UsrGM5::mqttconn(MqttConnInfo_t *info)
{
    if(!net_connected) return -1;
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    mState = Gm5StateMqttConnecting;
    uart->printf("AT+MQTTCONN=");
    if(info->name != "")
    {
        uart->printf("\"%s\",%d,\"%s\",%d,%d,\"%s\",\"%s\"\r\n",info->server.c_str(),info->port,\
                      info->clientid.c_str(),info->keeplive,info->clearsession,\
                        info->name.c_str(),info->password.c_str());
    }
    else
    {
        uart->printf("\"%s\",%d,\"%s\",%d,%d\r\n",info->server.c_str(),info->port,\
                      info->clientid.c_str(),info->keeplive,info->clearsession);
    }


    delay_ms(1000);
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    int ret = paraseErr(sa);
    if(ret)
    {
        mqtt_state = false;
        gm5Debug("mqttconn err:%d\n",ret);
    }
    else
    {
        mqtt_state = true;
        mState = Gm5StateMqttConnected;
        gm5Debug("mqttconn success\n");
    }
    rt_mutex_release(mutex);
    return ret;
}

int UsrGM5::mqttpub(String topic,String payload,uint8_t qos,uint8_t duplicate,uint8_t retain)
{
    if(!mqtt_state) return -1;
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    payload.replace("\"","\\\"");
    payload.replace("\r\n","\n");
    uart->printf("AT+MQTTPUB=\"%s\",\"",topic.c_str());
    uart->write(payload.c_str(),payload.length());
    uart->printf("\",%d,%d,%d\r\n",qos,duplicate,retain);
    
//    delay_ms(100);
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    int ret = paraseErr(sa);
    if(ret)
    {
        mqtt_state = false;
        gm5Debug("MQTTPUB err:%d\n",ret);
    }
    else
    {
        gm5Debug("MQTTPUB success\n");
    }
    rt_mutex_release(mutex);
    return ret;
}
int UsrGM5::mqttsubunsub(String topic,uint8_t subflag,uint8_t qos)
{
    if(!mqtt_state) return -1;
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    if(subflag)
    {
        uart->printf("AT+MQTTSUBUNSUB=\"%s\",%d,%d\r\n",topic.c_str(),subflag,qos);
        gm5Debug("AT+MQTTSUBUNSUB=\"%s\",%d,%d\r\n",topic.c_str(),subflag,qos);
    }
    else
    {
        uart->printf("AT+MQTTSUBUNSUB=\"%s\",0\r\n",topic.c_str());
        gm5Debug("AT+MQTTSUBUNSUB=\"%s\",0\r\n",topic.c_str());
    }
//    delay_ms(100);
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    int ret = paraseErr(sa);
    if(ret)
    {
        mqtt_state = false;
        gm5Debug("MQTTSUBUNSUB err:%d\n",ret);
    }
    else
    {
        gm5Debug("MQTTSUBUNSUB success\n");
    }
    rt_mutex_release(mutex);
    return ret;
}
int UsrGM5::mqttsta()//pdp context active
{
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    uart->printf("AT+MQTTSTA?\r\n");
    String str = uart->readString();
    StringArray sa = strSplit(str);
//    sa.print(UART);
    int ret = paraseErr(sa);
    if(ret)
    {
        gm5Debug("mqttsta err:%d\n",ret);
    }
    rt_mutex_release(mutex);
    return ret;
}

int UsrGM5::save()
{
    String mod_str;
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    uart->print("AT+S\r\n");

    String str = uart->readString();
    StringArray sa = strSplit(str);
    int ret = paraseErr(sa);
    if(!ret)
    {
        gm5Debug("save !!\n");
        mState = Gm5StateInterfaceInited;
    }
    rt_mutex_release(mutex);
    return ret;

}

int UsrGM5::restart_soft()
{
    String mod_str;
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    uart->print("AT+Z\r\n");

    String str = uart->readString();
    StringArray sa = strSplit(str);
    int ret = paraseErr(sa);
    if(!ret)
    {
        mState = Gm5StateInterfaceInited;
    }

    rt_mutex_release(mutex);
    return ret;
}
int UsrGM5::restart_hard()
{
    rt_mutex_take(mutex, RT_WAITING_FOREVER);
    mState = Gm5StateInterfaceInited;
    rst->set();
    delay_ms(100);
    rst->reset();
    rt_mutex_release(mutex);
    return 0;
}
bool UsrGM5::is_ready()
{
    if(mState >= Gm5StateStarted)
        return true;
    else
        return false;

}
int UsrGM5::wait_ready()
{
    uint32_t last = millis();
    while(!is_stable && mState < Gm5StateStarted)
    {
        gm5Debug("wait_ready:%d\n",millis());
        delay_ms(1000);
    }
        
    if(is_ready())
    {
        gm5Debug("gm5 is ready\n");
        return 0;
    }
    else
    {
        gm5Debug("gm5 is timeout!!!!!!\n");
    }
    return -1;
}
//int UsrGM5::wait_subok()
//{
//    return 0;
//}


void UsrGM5::loop()
{
    static GM5State_t last_state = Gm5StateNone;
    rt_err_t result;
    result = rt_mutex_take(mutex, 10);
    if (result != RT_EOK)
    {
//        rt_kprintf("loop take a dynamic mutex, failed.\n");
        return ;
    }
    if(uart->available())
    {
        String str = uart->readString();
        StringArray sa = strSplit(str);
//        sa.print(UART);
        for(int i = 0; i < sa.size(); i++)
        {
            String s = sa.get(i);
            paraseLoop(s);
        }
    }
    if(last_state != mState)
    {
        last_state_change_time = millis();
        gm5Debug("mstate changed to [%d]:[%d]\n",mState,last_state_change_time);
        last_state = mState;
        is_stable = false;
    
    }
    if(is_stable == false)
    {
        switch((uint8_t)mState)
        {
            case Gm5StateNone:
                if(millis() - last_state_change_time > 2000)
                {
                    gm5Debug("stable:Gm5StateNone[%d][%d]\n",Gm5StateNone,millis());
                    is_stable = true;
                }
                break;
            case Gm5StateInterfaceInited:
                if(millis() - last_state_change_time > 10*1000)
                {
                    gm5Debug("stable:Gm5StateInterfaceInited[%d][%d]\n",Gm5StateInterfaceInited,millis());
                    is_stable = true;
                }
                break;            
//            case Gm5StateStarting://2
//                if(millis() - last_state_change_time > 20*1000)
//                {
//                    gm5Debug("stable:Gm5StateStarting[%d][%d]\n",Gm5StateStarting,millis());
//                    is_stable = true;
//                }
//                break;
            case Gm5StateStarted://3
                if(millis() - last_state_change_time > 10*1000)
                {
                    gm5Debug("stable:Gm5StateStarted[%d][%d]\n",Gm5StateStarted,millis());
                    is_stable = true;
                }
                break;
            case Gm5StateMqttConnecting://4
                if(millis() - last_state_change_time > 10*1000)
                {
                    gm5Debug("stable:Gm5StateMqttConnecting[%d][%d]\n",Gm5StateMqttConnecting,millis());
                    is_stable = true;
                }
                break;
            case Gm5StateMqttConnected://5
                if(millis() - last_state_change_time > 10*1000)
                {
                    gm5Debug("stable:Gm5StateMqttConnected[%d][%d]\n",Gm5StateMqttConnected,millis());
                    is_stable = true;
                }
                break;
            case Gm5StateMqttSubOK://6
                gm5Debug("stable:Gm5StateMqttSubOK[%d][%d]\n",Gm5StateMqttSubOK,millis());
                is_stable = true;
                break;
        } 
    }
    rt_mutex_release(mutex);
}

void UsrGM5::attach(UsrGm5Callback hand)
{
    handler = hand;
}

void UsrGM5::attach_conn_changed(UsrGm5ConnChanged hand)
{
    conn_handler = hand;
}


