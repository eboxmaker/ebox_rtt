#include "mqtt.h"
#include "config.h"
#include "bsp_rtc.h"
#include "runValue.h"

#include "bsp_ebox.h"
#include "apps.h"

#include "modbus_addr.h"
#include "power.h"
#include "log.h"


//#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_MQTT_ENABLE       true
#define EBOX_DEBUG_MQTT_ENABLE_ERR   true
//#endif

#if EBOX_DEBUG_MQTT_ENABLE
#define mqtDebug(...)  rt_kprintf("[mqt:%03d]: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define mqtDebug(...)
#endif

#if EBOX_DEBUG_MQTT_ENABLE_ERR
#define mqtDebugErr(...)  rt_kprintf("[mqt:%03d]: ",__LINE__),rt_kprintf(__VA_ARGS__ )
#else
#define mqtDebugErr(...)
#endif


String event_register_post;
String event_register_postReplay;

String service_settings_get;
String service_settings_getReplay;
String service_settings_set;
String service_settings_setReplay;

String service_settings_mqtt_get;
String service_settings_mqtt_getReplay;
String service_settings_mqtt_set;
String service_settings_mqtt_setReplay;


String service_property_get;
String service_property_getReplay;
String service_property_set;
String service_property_setReplay;
String event_property_post;
String event_property_postReplay;


void MqttMsgBase::make_payload(cJSON *data)
{
    cJSON * root;
    timeStamp = (uint32_t)gdt.getTimeStamp();
    root=cJSON_CreateObject();                     
    cJSON_AddStringToObject(root,"src",dev.id); 
    cJSON_AddStringToObject(root,"timeStamp",timeStamp.c_str()); 
    cJSON_AddStringToObject(root,"bussId",timeStamp.c_str()); 
    cJSON_AddItemToObject(root,"data",data); 
    char *out = cJSON_Print(root);  
    payload = out;
    cJSON_Delete(root);  
    rt_free(out);  
    return ;
}

int MqttMsgReplay::add_ack(String &str)
{
    cJSON *root;
    cJSON *object;
    root=cJSON_Parse(str.c_str()); 
    if(root == NULL)
    {
        code = -1;
        errMsg = "json parse err";
    }
    else
    {
        object=cJSON_GetObjectItem(root,"bussId"); 
        if(root != NULL)
        {
            code = 0;
            errMsg = "";
            bussId = object->valuestring;
        }
        else
        {
            code = -2;
            errMsg = "bussId not exist";
        }

    }
    cJSON_Delete(root);  

    return code;
}

void MqttMsgReplay::make_payload(cJSON *data)
{
    cJSON * root;
    timeStamp = (uint32_t)gdt.getTimeStamp();
    root=cJSON_CreateObject();                     
    cJSON_AddStringToObject(root,"src",dev.id); 
    cJSON_AddStringToObject(root,"timeStamp",timeStamp.c_str()); 
    cJSON_AddStringToObject(root,"bussId",bussId.c_str()); 
    cJSON_AddNumberToObject(root,"code",code); 
    cJSON_AddStringToObject(root,"errMsg",errMsg.c_str());
    cJSON_AddItemToObject(root,"data",data); 
    char *out = cJSON_Print(root);  
    payload = out;
    cJSON_Delete(root);  
    rt_free(out);  
    return ;
}
String sys_thing(String slave)
{
    String topic = "/sys/";
    topic += dev.id;
    topic += slave;
    return topic;
}

void mqtt_init()
{
    
///ota/${deviceID}/device/upgrade
///ota/${deviceID}/device/progress
    
    event_register_post         = sys_thing("/event/register/post");
    event_register_postReplay   = sys_thing("/event/register/post_replay");

    event_property_post        = sys_thing("/event/property/post");
    event_property_postReplay  = sys_thing("/event/property/post_replay");
    
    service_property_get        = sys_thing("/service/property/get");
    service_property_getReplay  = sys_thing("/service/property/get_replay");
    
    service_property_set        = sys_thing("/service/property/set");
    service_property_setReplay  = sys_thing("/service/property/set_replay");
    
    service_settings_get        = sys_thing("/service/settings/get");
    service_settings_getReplay  = sys_thing("/service/settings/get_replay");
    
    service_settings_set        = sys_thing("/service/settings/set");
    service_settings_setReplay  = sys_thing("/service/settings/set_replay");

    service_settings_mqtt_get       = sys_thing("/service/settings/mqtt/get");
    service_settings_mqtt_getReplay = sys_thing("/service/settings/mqtt/get_replay");
    
    service_settings_mqtt_set       = sys_thing("/service/settings/mqtt/set");
    service_settings_mqtt_setReplay = sys_thing("/service/settings/mqtt/set_replay");
    
    mqtDebug("%s\n",event_register_post.c_str());
    mqtDebug("%s\n",event_register_postReplay.c_str());
    
    mqtDebug("%s\n",event_property_post.c_str());
    mqtDebug("%s\n",event_property_postReplay.c_str());
    mqtDebug("%s\n",service_property_get.c_str());
    mqtDebug("%s\n",service_property_getReplay.c_str());
    mqtDebug("%s\n",service_property_set.c_str());
    mqtDebug("%s\n",service_property_setReplay.c_str());
    mqtDebug("%s\n",event_register_post.c_str());
    
    mqtDebug("%s\n",service_settings_get.c_str());
    mqtDebug("%s\n",service_settings_getReplay.c_str());
    mqtDebug("%s\n",service_settings_set.c_str());
    mqtDebug("%s\n",service_settings_setReplay.c_str());

    mqtDebug("%s\n",service_settings_mqtt_get.c_str());
    mqtDebug("%s\n",service_settings_mqtt_getReplay.c_str());
    mqtDebug("%s\n",service_settings_mqtt_set.c_str());
    mqtDebug("%s\n",service_settings_mqtt_setReplay.c_str());

}

bool topic_event_register_post_replay(String &topic,String &payload)
{

    if (topic != event_register_postReplay)
    {
        return false;
    }
    
    cJSON *root=cJSON_Parse(payload.c_str()); 

    cJSON *object=cJSON_GetObjectItem(root,"code"); 
    int code = object->valueint;
    object=cJSON_GetObjectItem(root,"errMsg"); 
    String errMsg = object->valuestring;
    cJSON_Delete(root);  

//    if(code)
    {
        UART.printf("----register post replay----\n");
        UART.printf("code:%d,errMsg:%s\n",code,errMsg.c_str());
    }
//    UART.print(str);
    return true;
}
bool topic_event_property_post_replay(String &topic,String &payload)
{
    if (topic != event_property_postReplay)
    {
        return false;
    }
    
    cJSON *root=cJSON_Parse(payload.c_str()); 

    cJSON *object=cJSON_GetObjectItem(root,"code"); 
    int code = object->valueint;
    object=cJSON_GetObjectItem(root,"errMsg"); 
    String errMsg = object->valuestring;
    cJSON_Delete(root);  
    if(code)
    {
        UART.printf("----property_post_replay----\n");
        UART.printf("code:%d,errMsg:%s\n",code,errMsg.c_str());
    }
    return true;
}

bool topic_service_property_get(String &topic,String &payload)
{
    if (topic != service_property_get)
    {
        UART.printf("topic = %s\n",topic.c_str());
        UART.printf("service_property_get :%s\n",service_property_get.c_str());
        return false;
    }
    MqttMsgReplay replayMsg;
    
    replayMsg.add_ack(payload);
    
    replayMsg.topic = service_property_getReplay;
    cJSON * data = cJSON_CreateObject();  
    cJSON_AddNumberToObject(data,"rd_a_dc_v",ebox_round(run.r6_adv,3)); 
    cJSON_AddNumberToObject(data,"rd_a_ac_v",ebox_round(run.r8_aav,3));  
    cJSON_AddNumberToObject(data,"rd_a_dc_i",ebox_round(run.r10_adc,3));     
    cJSON_AddNumberToObject(data,"rd_a_ac_i",ebox_round(run.r12_aac,3)); 
    cJSON_AddNumberToObject(data,"rd_b_dc_v",ebox_round(run.r14_bdv,3)); 
    cJSON_AddNumberToObject(data,"rd_b_ac_v",ebox_round(run.r16_bav,3));  
    cJSON_AddNumberToObject(data,"rd_b_dc_i",ebox_round(run.r18_bdc,3));     
    cJSON_AddNumberToObject(data,"rd_b_ac_i",ebox_round(run.r20_bac,3)); 
    cJSON_AddNumberToObject(data,"rd_off_n_dc_v",ebox_round(run.r22_ndv,3)); 
    cJSON_AddNumberToObject(data,"rd_off_a_dc_v",ebox_round(run.r24_nadv,3));  
    cJSON_AddNumberToObject(data,"rd_off_b_dc_v",ebox_round(run.r26_nbdv,3));     
    cJSON_AddNumberToObject(data,"rd_off_zn_dc_v",ebox_round(run.r28_ncuznv,3));    
    
    cJSON_AddNumberToObject(data,"rd_battery",ebox_round(run.bat_vol,3)); 
    cJSON_AddNumberToObject(data,"rd_cpu",ebox_round(sys.cpu,2)); 
    cJSON_AddNumberToObject(data,"rd_mem",ebox_round(sys.mem.percent,2)); 
    cJSON_AddNumberToObject(data,"rd_temp",ebox_round(adc1.read_temp(),1)); 
    
    cJSON_AddNumberToObject(data,"rd_lng",gps.location.lng()); 
    cJSON_AddNumberToObject(data,"rd_lat",gps.location.lat()); 
    cJSON_AddNumberToObject(data,"rd_csq",gm5.csq); 
    
    String str = run.dt.toString();
    cJSON_AddStringToObject(data,"rd_datetime",str.c_str());

    replayMsg.make_payload(data);
//    UART.print(replayMsg.topic);
//    UART.print(replayMsg.payload);
    gm5.mqttpub(replayMsg.topic.c_str(),replayMsg.payload.c_str());
    return true;
}
bool topic_service_settings_get(String &topic,String &payload)
{
    if (topic != service_settings_get)
    {
        UART.printf("topic = %s\n",topic.c_str());
        UART.printf("service_settings_get :%s\n",service_settings_get.c_str());
        return false;
    }
    MqttMsgReplay msgReplay;
    
    
    msgReplay.add_ack(payload);  
    
    
    
    msgReplay.topic = service_settings_getReplay;
    cJSON * data = cJSON_CreateObject();  
    cJSON_AddNumberToObject(data,"conf_modbus_id",config.modbus_id); 
    cJSON_AddNumberToObject(data,"conf_sample_type",config.sampleMode); 
    cJSON_AddNumberToObject(data,"conf_regu_interval",config.sample_regu_interval); 
    cJSON_AddNumberToObject(data,"conf_regu_round",config.sample_regu_round); 
    cJSON_AddNumberToObject(data,"conf_cont_duration",config.sample_cont_duration); 
    cJSON_AddNumberToObject(data,"conf_cont_interval",config.sample_cont_interval); 
    cJSON_AddNumberToObject(data,"conf_gps_ctr",config.gps_ctr); 
    cJSON_AddNumberToObject(data,"conf_pipe_on_time",config.pipe_on_time); 
    cJSON_AddNumberToObject(data,"conf_pipe_off_time",config.pipe_off_time); 
    cJSON_AddNumberToObject(data,"conf_pipe_off_after",config.pipe_off_after); 
    cJSON_AddNumberToObject(data,"conf_off_dc_v_min",ebox_round(config_threshold.off_dc_min,3)); 
    cJSON_AddNumberToObject(data,"conf_off_dc_v_max",ebox_round(config_threshold.off_dc_max,3)); 
    cJSON_AddNumberToObject(data,"conf_on_dc_v_max",ebox_round(config_threshold.on_dc_max,3)); 
    cJSON_AddNumberToObject(data,"conf_on_ac_v_max",ebox_round(config_threshold.on_ac_max,3)); 

    
    
    msgReplay.make_payload(data);
//    UART.print(replayMsg.topic);
//    UART.print(replayMsg.payload);
    gm5.mqttpub(msgReplay.topic.c_str(),msgReplay.payload.c_str());
    return true;
}


bool topic_service_settings_set(String &topic,String &payload)
{
    int ret = 0;
    if (topic != service_settings_set)
    {
        UART.printf("topic = %s\n",topic.c_str());
        UART.printf("service_settings_set :%s\n",service_settings_set.c_str());
        return false;
    }
    MqttMsgReplay msgReplay;

    cJSON *root;
    cJSON *data;
    cJSON *object;
    
    ret = msgReplay.add_ack(payload);
    if(ret)
    {
        goto end;
    }
    
    root=cJSON_Parse(payload.c_str());     
    data = cJSON_GetObjectItem(root,"data");
    if(data == NULL)
    {
        msgReplay.code = -1;
        msgReplay.errMsg = "data not exist";
        goto end;
    }



    object = cJSON_GetObjectItem(data,"conf_modbus_id");
    if(object!=NULL)
    {
        config.modbus_id = object->valueint;
    }

    object = cJSON_GetObjectItem(data,"conf_sample_type");
    if(object!=NULL)
    {
        config.sampleMode = (Config::SampleMode_t)object->valueint;
        if(sample_mode < 2)
        {
            if(sample_mode != config.sampleMode)
            {
                set_sample_mode(config.sampleMode);
            }
        }
    }

    object = cJSON_GetObjectItem(data,"conf_regu_interval");
    if(object!=NULL)
    {
        config.sample_regu_interval = object->valueint;
    }
//
//
//
//

    object = cJSON_GetObjectItem(data,"conf_regu_round");
    if(object!=NULL)
    {
        config.sample_regu_round = object->valueint;
    }

    object = cJSON_GetObjectItem(data,"conf_cont_duration");
    if(object!=NULL)
    {
        config.sample_cont_duration = object->valueint;
    }

    object = cJSON_GetObjectItem(data,"conf_cont_interval");
    if(object!=NULL)
    {
        config.sample_cont_interval = object->valueint;
    }

    object = cJSON_GetObjectItem(data,"conf_gps_ctr");
    if(object!=NULL)
    {
        config.gps_ctr = (Config::GpsCtr_t)object->valueint;
        power_ctr(PwrGps,config.gps_ctr);
    }

    object = cJSON_GetObjectItem(data,"conf_pipe_on_time");
    if(object!=NULL)
    {
        config.pipe_on_time = object->valueint;
    }

    object = cJSON_GetObjectItem(data,"conf_pipe_off_time");
    if(object!=NULL)
    {
        config.pipe_off_time = object->valueint;
    }

    object = cJSON_GetObjectItem(data,"conf_pipe_off_after");
    if(object!=NULL)
    {
        config.pipe_off_after = object->valueint;
    }

    object = cJSON_GetObjectItem(data,"conf_off_dc_v_min");
    if(object!=NULL)
    {
        config_threshold.off_dc_min = object->valuedouble;
    }

    object = cJSON_GetObjectItem(data,"conf_off_dc_v_max");
    if(object!=NULL)
    {
        config_threshold.off_dc_max = object->valuedouble;
    }

    object = cJSON_GetObjectItem(data,"conf_on_dc_v_max");
    if(object!=NULL)
    {
        config_threshold.on_dc_max = object->valuedouble;
    }

    object = cJSON_GetObjectItem(data,"conf_on_ac_v_max");
    if(object!=NULL && object->type == cJSON_Number)
    {
        config_threshold.on_ac_max = object->valuedouble;
    }

    config.para_limit();
    config.para_to_frame();
    config.save();
    mb_frame_to_hreg();
    


    
    cJSON_Delete(root);  

//    UART.print(str);
    
    end: 
    UART.printf("----service_settings_setReplay----\n");
    
    msgReplay.topic = service_settings_setReplay;
    data = cJSON_CreateObject();  
    cJSON_AddNumberToObject(data,"conf_modbus_id",config.modbus_id); 
    cJSON_AddNumberToObject(data,"conf_sample_type",config.sampleMode); 
    cJSON_AddNumberToObject(data,"conf_regu_interval",config.sample_regu_interval); 
    cJSON_AddNumberToObject(data,"conf_regu_round",config.sample_regu_round); 
    cJSON_AddNumberToObject(data,"conf_cont_duration",config.sample_cont_duration); 
    cJSON_AddNumberToObject(data,"conf_cont_interval",config.sample_cont_interval); 
    cJSON_AddNumberToObject(data,"conf_gps_ctr",config.gps_ctr); 
    cJSON_AddNumberToObject(data,"conf_pipe_on_time",config.pipe_on_time); 
    cJSON_AddNumberToObject(data,"conf_pipe_off_time",config.pipe_off_time); 
    cJSON_AddNumberToObject(data,"conf_pipe_off_after",config.pipe_off_after); 
    cJSON_AddNumberToObject(data,"conf_off_dc_v_min",ebox_round(config_threshold.off_dc_min,3)); 
    cJSON_AddNumberToObject(data,"conf_off_dc_v_max",ebox_round(config_threshold.off_dc_max,3)); 
    cJSON_AddNumberToObject(data,"conf_on_dc_v_max",ebox_round(config_threshold.on_dc_max,3)); 
    cJSON_AddNumberToObject(data,"conf_on_ac_v_max",ebox_round(config_threshold.on_ac_max,3)); 

    msgReplay.make_payload(data);
    gm5.mqttpub(msgReplay.topic.c_str(),msgReplay.payload.c_str());
    return true;
    return true;
}
bool topic_service_settings_mqtt_get(String &topic,String &payload)
{
    if (topic != service_settings_mqtt_get)
    {
        UART.printf("topic = %s\n",topic.c_str());
        UART.printf("service_property_get :%s\n",service_settings_mqtt_get.c_str());
        return false;
    }

    MqttMsgReplay msgReplay;
    
    msgReplay.add_ack(payload);

 
    
    msgReplay.topic = service_settings_mqtt_getReplay;
    cJSON * data = cJSON_CreateObject();  
    cJSON_AddStringToObject(data,"mqtt_username",config_server.name); 
    cJSON_AddStringToObject(data,"mqtt_password",config_server.password); 
    cJSON_AddStringToObject(data,"mqtt_ip",config_server.ip); 
    cJSON_AddNumberToObject(data,"mqtt_port",config_server.port); 
    cJSON_AddNumberToObject(data,"mqtt_timeout",config_server.timeout); 
    cJSON_AddNumberToObject(data,"mqtt_ssl",config_server.ssl_tls); 

    
    
    msgReplay.make_payload(data);
    UART.print(msgReplay.topic);
    UART.print(msgReplay.payload);
    gm5.mqttpub(msgReplay.topic.c_str(),msgReplay.payload.c_str());
    return true;
}
bool topic_service_settings_mqtt_set(String &topic,String &payload)
{
    int ret;
    if (topic != service_settings_mqtt_set)
    {
        UART.printf("topic = %s\n",topic.c_str());
        UART.printf("service_property_get :%s\n",service_settings_mqtt_set.c_str());
        return false;
    }
    MqttMsgReplay msgReplay;

    cJSON *root;
    cJSON *data;
    cJSON *object;
    String bussId ;
    
    ret = msgReplay.add_ack(payload);
    if(ret)
    {
        goto end;
    }
    root=cJSON_Parse(payload.c_str());     
    data = cJSON_GetObjectItem(root,"data");
    if(data == NULL)
    {
        msgReplay.code = -1;
        msgReplay.errMsg = "params err";
        goto end;
    }
    
    object = cJSON_GetObjectItem(data,"mqtt_username");
    if(object!=NULL)
    {
        rt_memset(config_server.name,0,32);
        rt_memcpy(config_server.name,object->valuestring,rt_strlen(object->valuestring));
    }

    object = cJSON_GetObjectItem(data,"mqtt_password");
    if(object!=NULL)
    {
        rt_memset(config_server.password,0,16);
        rt_memcpy(config_server.password,object->valuestring,rt_strlen(object->valuestring));
    }

    object = cJSON_GetObjectItem(data,"mqtt_ip");
    if(object!=NULL)
    {
        rt_memset(config_server.ip,0,32);
        rt_memcpy(config_server.ip,object->valuestring,rt_strlen(object->valuestring));
    }

    object = cJSON_GetObjectItem(data,"mqtt_port");
    if(object!=NULL)
    {
        config_server.port = object->valueint;
    }

    object = cJSON_GetObjectItem(data,"mqtt_timeout");
    if(object!=NULL)
    {
        config_server.timeout = object->valueint;
    }

    object = cJSON_GetObjectItem(data,"mqtt_ssl");
    if(object!=NULL)
    {
        config_server.ssl_tls = object->valueint;
    }
    
    config.para_limit();
    config.para_to_frame();
    config.save();
    mb_frame_to_hreg();
    
    cJSON_Delete(root);  

//    UART.print(str);
    
    end: 
    UART.printf("----service_settings_mqtt_setReplay----\n");
    
    msgReplay.topic = service_settings_mqtt_setReplay;
    data = cJSON_CreateObject();  
    cJSON_AddStringToObject(data,"mqtt_username",config_server.name); 
    cJSON_AddStringToObject(data,"mqtt_password",config_server.password); 
    cJSON_AddStringToObject(data,"mqtt_ip",config_server.ip); 
    cJSON_AddNumberToObject(data,"mqtt_port",config_server.port); 
    cJSON_AddNumberToObject(data,"mqtt_timeout",config_server.timeout); 
    cJSON_AddNumberToObject(data,"mqtt_ssl",config_server.ssl_tls); 
    msgReplay.make_payload(data);
    gm5.mqttpub(msgReplay.topic.c_str(),msgReplay.payload.c_str());
    return true;
}

int mqtt_register()
{
    MqttMsgBase msg_register;
    msg_register.topic = event_register_post;
    cJSON * data = cJSON_CreateObject();  
    cJSON_AddStringToObject(data,"base_deviceid",dev.id); 
    cJSON_AddStringToObject(data,"base_model",dev.model);  
    cJSON_AddNumberToObject(data,"base_type",dev.type);     
    cJSON_AddNumberToObject(data,"base_version",dev.version); 
    cJSON_AddStringToObject(data,"base_iccid",gm5.iccid.c_str()); 
    cJSON_AddStringToObject(data,"base_imei",gm5.imei.c_str()); 
    cJSON_AddStringToObject(data,"base_net_mode",gm5.sysinfo_mode.c_str());
    cJSON_AddStringToObject(data,"mqtt_ip",config_server.ip); 
    cJSON_AddNumberToObject(data,"mqtt_port",config_server.port); 
    msg_register.make_payload(data);
    return gm5.mqttpub(msg_register.topic.c_str(),msg_register.payload.c_str());
}

//int mqtt_property_post()
//{
//    MqttMsgBase msg;
//    msg.topic = event_property_post;
//    cJSON * data = cJSON_CreateObject();  
//    String str = run.dt.toString();
//    cJSON_AddStringToObject(data,"rd_datetime",str.c_str());

//    cJSON_AddNumberToObject(data,"rd_a_dc_v",ebox_round(run.r6_adv,3)); 
//    cJSON_AddNumberToObject(data,"rd_a_ac_v",ebox_round(run.r8_aav,3));  
//    cJSON_AddNumberToObject(data,"rd_a_dc_i",ebox_round(run.r10_adc,3));     
//    cJSON_AddNumberToObject(data,"rd_a_ac_i",ebox_round(run.r12_aac,3)); 
//    cJSON_AddNumberToObject(data,"rd_b_dc_v",ebox_round(run.r14_bdv,3)); 
//    cJSON_AddNumberToObject(data,"rd_b_ac_v",ebox_round(run.r16_bav,3));  
//    cJSON_AddNumberToObject(data,"rd_b_dc_i",ebox_round(run.r18_bdc,3));     
//    cJSON_AddNumberToObject(data,"rd_b_ac_i",ebox_round(run.r20_bac,3)); 
//    cJSON_AddNumberToObject(data,"rd_off_n_dc_v",ebox_round(run.r22_ndv,3)); 
//    cJSON_AddNumberToObject(data,"rd_off_a_dc_v",ebox_round(run.r24_nadv,3));  
//    cJSON_AddNumberToObject(data,"rd_off_b_dc_v",ebox_round(run.r26_nbdv,3));     
//    cJSON_AddNumberToObject(data,"rd_off_zn_dc_v",ebox_round(run.r28_ncuznv,3)); 
//    cJSON_AddNumberToObject(data,"rd_battery",ebox_round(run.bat_vol,3)); 
//    cJSON_AddNumberToObject(data,"rd_cpu",ebox_round(sys.cpu,2)); 
//    cJSON_AddNumberToObject(data,"rd_mem",ebox_round(sys.mem.percent,2)); 
//    cJSON_AddNumberToObject(data,"rd_lng",gps.location.lng()); 
//    cJSON_AddNumberToObject(data,"rd_lat",gps.location.lat()); 

//    cJSON_AddNumberToObject(data,"rd_csq",gm5.csq); 
//    
//    msg.make_payload(data);
//    

//    while(!gm5.is_stable)
//    {
//        mqtDebug("mqtt post:wait gm5.is_stable:%d\n",gm5.is_stable);
//        delay_ms(1000);
//    }
//  
//    int ret = gm5.mqttpub(msg.topic.c_str(),msg.payload.c_str());
//    if(ret)
//    {
//        elog.add(msg.payload,run.dt);
//    }
//    return ret;
//}


int mqtt_property_post(RunSample *node)
{
    int i = 0;
    MqttMsgBase msg;
    msg.topic = event_property_post;
    cJSON * data = cJSON_CreateObject();  
    
    String str = run.dt.toString();
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

    
    msg.make_payload(data);
    
  
    int ret = gm5.mqttpub(msg.topic.c_str(),msg.payload.c_str());
    return ret;
}

int mqtt_settings_mqtt_set_replay()
{
    MqttMsgBase msg_register;
    msg_register.topic = service_settings_mqtt_setReplay;
    cJSON * data = cJSON_CreateObject();  
    cJSON_AddStringToObject(data,"mqtt_username",config_server.name); 
    cJSON_AddStringToObject(data,"mqtt_password",config_server.password); 
    cJSON_AddStringToObject(data,"mqtt_ip",config_server.ip); 
    cJSON_AddNumberToObject(data,"mqtt_port",config_server.port); 
    cJSON_AddNumberToObject(data,"mqtt_timeout",config_server.timeout); 
    cJSON_AddNumberToObject(data,"mqtt_ssl",config_server.ssl_tls); 
    msg_register.make_payload(data);
    return gm5.mqttpub(msg_register.topic.c_str(),msg_register.payload.c_str());
}
