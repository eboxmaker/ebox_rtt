#include "apps.h"
#include "mqtt.h"

#include "bsp_ebox.h"
#include "config.h"
#include "modbus_addr.h"
#include "log.h"
#include "low_power.h"

MqttConnInfo_t info;

void process_recv_topic(String &topic,String &payload)
{
//    UART.print("topic:");
//    UART.println(topic);
//    UART.print("payload:");
//    UART.println(payload);

    if(topic == event_register_postReplay)
    {
        topic_event_register_post_replay(topic,payload);
    }
    else if(topic == event_property_postReplay)
    {
        topic_event_property_post_replay(topic,payload);
    }
        
    else if(topic == service_property_get)
    {
        topic_service_property_get(topic,payload);
    }
    else if(topic == service_settings_get)
    {
        topic_service_settings_get(topic,payload);
    }
    else if(topic == service_settings_set)
    {
        topic_service_settings_set(topic,payload);
    }
    else if(topic == service_settings_mqtt_get)
    {
        topic_service_settings_mqtt_get(topic,payload);
    }
    else if(topic == service_settings_mqtt_set)
    {
        topic_service_settings_mqtt_set(topic,payload);
    }
    else
    {

    }
    
}

void mqtt_callback(String &package)
{
    int start = 0;
    int index = 0;
    String sub[3];

    index = package.indexOf(',',start);
    sub[0] = package.substring(start,index);
    start = index+1;
    
    index = package.indexOf(',',start);
    sub[1] = package.substring(start,index);
    start = index+1;
    
    sub[2] = package.substring(start,package.length());
    start = index+1;
//    appDebug("str:[%s]\n",package.c_str());
    appDebug("[0]:[%s]\n",sub[0].c_str());
    appDebug("[1]:[%s]\n",sub[1].c_str());
    appDebug("[2]:[%s]\n",sub[2].c_str());
    
    process_recv_topic(sub[0],sub[2]);
}


int gm5_mqtt_startup()
{
    int ret = 0;
//    info.server = "mqtt.eboxmaker.com";
//    info.name = "";
//    info.password = "";
//    info.port = 1883;
    
//    info.server = "222.222.74.158";
//    info.name = "system";
//    info.password = "JNST@fengjieMQTT";
//    info.port = 1883;

    info.server = config_server.ip;
    info.name = config_server.name;
    info.password = config_server.password;
    info.port = config_server.port;
    
    info.clientid = dev.id;
    info.keeplive = 60;
    info.clearsession = 0;
    appDebug("server:%s:[%d]\t",info.server.c_str(),info.port);
    appDebug("name:[%s]\t",info.name.c_str());
    appDebug("password:[%s]\n",info.password.c_str());
    if(gm5.mqttconn(&info))
    {
        appDebug("gm5.mqttconn(&info):failed\n");
        return -1;
    }
//    delay_ms(1000);
    gm5.mqttsubunsub(event_property_postReplay);
    gm5.mqttsubunsub(event_register_postReplay);

    gm5.mqttsubunsub(service_property_get);
    gm5.mqttsubunsub(service_settings_get);
    gm5.mqttsubunsub(service_settings_set);
    gm5.mqttsubunsub(service_settings_mqtt_get);
    gm5.mqttsubunsub(service_settings_mqtt_set);    

    mqtt_register();
    delay_ms(1000);
    gm5.mState = Gm5StateMqttSubOK;
    app_history_create();//create history
    return 0;
}

int gm5_startup()
{
    int ret = 0;
    appDebug("gm5_startup：wait 4g module.\n");
    gm5.restart_hard();
    ret = gm5.wait_ready();
    if(ret)
        return ret;
    if(gm5.wkmod() != "CMD")
    {
        gm5.wkmod("CMD");
        gm5.save();
        gm5.wait_ready();
    }
    gm5.get_cgact();
    uint32_t mark = millis();
    while(1)
    {
        gm5.set_cgact(1,1);
        if(gm5.cgact_state == 1 && gm5.cgact_cid == 1)
        {
            appDebug("CGACT PDP IS ACTIVED\n");
            break;
        }
        else
        {
            appDebug("wait net work active\n");
        }

        delay_ms(1000);
    }
    
    gm5.get_iccid();
    gm5.get_imei();
    gm5.get_sysinfo();
    gm5.get_csq();
    gm5.get_cclk();
    rt_snprintf(dev.iccid,32,"%s",gm5.iccid.c_str());
    rt_snprintf(dev.imei,32,"%s",gm5.imei.c_str());
    dev.para_to_buf();
    mb_update_ireg_read_only();

    return 0;
}


void online_loop()
{
    static uint32_t last = millis();
    static int state = 0;
    int ret;
    switch(state)
    {
        case 0:
            if(gm5_startup())
            {
                delay_ms(5000);
            }
            else
            {
                state = 1;
                last = millis();
            }
            break;
        case 1:
            if(gm5_mqtt_startup())
            {
                gm5.get_cgact();
                if(!gm5.net_connected)
                    state = 0;
                delay_ms(10*1000);
            }
            else
            {
                state = 2;
                last = millis();
            }
            break;
        case 2:
            if(millis() - last > 30*1000)
            {
                last = millis();
                ret = gm5.get_cgact();
                if(ret)
                {
                    appDebug("GM5 hard timeout!\n");
                    state = 0;
                }
                ret = gm5.mqttsta();
                if(ret)
                {
                    appDebug("GM5 hard timeout!\n");
                    state = 0;
                }
                if(!gm5.net_connected)
                {
                    appDebug("GM5 net conn : disconnected!\n");
                    state = 0;
                }
                else if(!gm5.mqtt_state && gm5.net_connected)
                {
                    appDebug("GM5 mqtt conn : disconnected!\n");
                    state = 1;
                }
                else if(gm5.mqtt_state && gm5.net_connected)
                {
                    //appDebug("GM5 mqtt conn check ok!\n");
                }        
            }
            break;
        default:
            break;
    }

}
static void app_mqtt_thread_entry(void* paremeter)
{
   
    static int state = 0;
    gm5.attach(mqtt_callback);
    static uint32_t last = millis();
    static uint32_t last2 = millis();
    while(1)
    {
        
        online_loop();
        if(gm5.net_connected)
        {
            if(millis() - last > 10*1000)
            {
                last = millis();
                gm5.get_csq();
            }
            if(millis() - last2 > 1*60*1000)
            {
                last2 = millis();
                gm5.get_cclk();
            }
        }

        delay_ms(100);
    }
}

int app_mqtt_create()
{
    rt_thread_t app1_thread = rt_thread_create( "mqtt",     /*name*/
                                    app_mqtt_thread_entry,/*入口*/
                                    RT_NULL,/*参数*/
                                    1024*8,    /*heap*/
                                    5 ,    /*优先级*/
                                     20);   /*时间片段*/
    if(app1_thread !=RT_NULL)
    {
        rt_thread_startup (app1_thread);
    }
    else 
    {
        appDebug("create failed\n");
        return -1;
    }
    return 0;
    
}