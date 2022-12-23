#include "config.h"



void ConfigServer::begin()
{
    rt_memset(name,0,32);
    rt_memset(password,0,16);
    rt_memset(ip,0,32);

    rt_snprintf(name,32,"system");
    rt_snprintf(password,32,"JNST@fengjieMQTT");
    rt_snprintf(ip,32,"222.222.74.158");
    port = 1883;
    timeout = 120;
    ssl_tls = 1;
    para_to_buf();

}

int ConfigServer::para_to_buf()
{
    DataU16_t XUint16;    
    uint16_t counter = 0;
    for(int i = 0; i < 32; i++)
    {
        buf[counter++] = name[i];
    }
    for(int i = 0; i < 32; i++)
    {
        buf[counter++] = password[i];
    }
    for(int i = 0; i < 32; i++)
    {
        buf[counter++] = ip[i];
    }

    XUint16.value = port;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];
    
    XUint16.value = timeout;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];
    
    XUint16.value = ssl_tls;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];
    
//    XUint16.value = qos;
//    for(int i = 0; i < 2; i++)
//        buf[counter++] = XUint16.byte[i];
//        
//    for(int i = 0; i < 64; i++)
//    {
//        buf[counter++] = topic_pub[i];
//    }
//    for(int i = 0; i < 64; i++)
//    {
//        buf[counter++] = topic_sub[i];
//    }
//    for(int i = 0; i < 64; i++)
//    {
//        buf[counter++] = msg_will[i];
//    }

//    print_buf(buf,len);
    cnt1 = counter;
    len = counter;
    return counter;
}




int ConfigServer::buf_to_para()
{
    DataU16_t XUint16;
    uint16_t counter = 0;

    for(int i = 0; i < 32; i++)
    {
        name[i] = (char)buf[counter++];
    }    
    for(int i = 0; i < 32; i++)
    {
        password[i] = (char)buf[counter++];
    }    
    for(int i = 0; i < 32; i++)
    {
        ip[i] = (char)buf[counter++];
    }    
    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = buf[counter++];
    port = XUint16.value;
    
    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = buf[counter++];
    timeout = XUint16.value;

    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = buf[counter++];
    ssl_tls = XUint16.value;
    
//    for(int i = 0; i < 2; i++)
//        XUint16.byte[i] = buf[counter++];
//    qos = XUint16.value;
//    
//    for(int i = 0; i < 64; i++)
//    {
//        topic_pub[i] = (char)buf[counter++];
//    }    
//    for(int i = 0; i < 64; i++)
//    {
//        topic_sub[i] = (char)buf[counter++];
//    }    
//    for(int i = 0; i < 64; i++)
//    {
//        msg_will[i] = (char)buf[counter++];
//    }  
    cnt2 = counter;
    len = counter;

    return counter;
}

void ConfigServer::print()
{
        
    cfgDebug("===========ConfigServer:%d===========\n",len);
    cfgDebug("cnt1:%d, cnt2:%d\n",cnt1,cnt2);      
    cfgDebug("name:%s\n",name);      
    cfgDebug("password:%s\n",password);      
    cfgDebug("ip:%s\n",ip);      
    cfgDebug("port:%d\n",port);      
    cfgDebug("timeout:%d\n",timeout);      
    cfgDebug("ssl_tls:%d\n",ssl_tls);      
//    rt_kprintf("qos:%d\n",qos);      
//    rt_kprintf("topic_pub:%s\n",topic_pub);      
//    rt_kprintf("topic_sub:%s\n",topic_sub);      
//    rt_kprintf("msg_will:%s\n",msg_will);      

    cfgDebug("================================\n");

    
}

