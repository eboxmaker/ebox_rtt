#ifndef __MQTT_H
#define __MQTT_H


#include "ebox_core.h"
#include "cJSON.h"
#include "runValue.h"


extern String event_register_post;
extern String event_register_postReplay;

extern String service_settings_get;
extern String service_settings_getReplay;
extern String service_settings_set;
extern String service_settings_setReplay;

extern String service_settings_mqtt_get;
extern String service_settings_mqtt_getReplay;
extern String service_settings_mqtt_set;
extern String service_settings_mqtt_setReplay;


extern String service_property_get;
extern String service_property_getReplay;
extern String service_property_set;
extern String service_property_setReplay;
extern String event_property_post;
extern String event_property_postReplay;

extern String sys_thing(String slave);




class MqttMsgBase
{
    public:
        MqttMsgBase()
        {
            
        }
        String timeStamp;
        String bussId;
        String topic;
        String payload;
        void make_payload(cJSON *data);
};
class MqttMsgReplay
{
    public:
        MqttMsgReplay()
        {
            code = 0;
        }
        String timeStamp;
        String bussId;
        int code;
        String errMsg;
        String topic;
        String payload;
        int add_ack(String &str);
        void make_payload(cJSON *data);
};




void mqtt_init();


bool topic_event_register_post_replay(String &topic,String &payload);
bool topic_event_property_post_replay(String &topic,String &payload);
bool topic_service_property_get(String &topic,String &payload);
bool topic_service_settings_get(String &topic,String &payload);
bool topic_service_settings_set(String &topic,String &payload);
bool topic_service_settings_mqtt_get(String &topic,String &payload);
bool topic_service_settings_mqtt_set(String &topic,String &payload);


int mqtt_register();
int mqtt_property_post();
int mqtt_property_post(RunSample *node);
int mqtt_settings_mqtt_set_replay();


class MqttHelper
{
    public:
        
    
    
    
    
    public:
    
};
#endif
