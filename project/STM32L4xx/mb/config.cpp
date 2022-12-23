#include "config.h"
#include "crc.h"
#include "bsp_ebox.h"

Dev dev;
Config config;
ConfigServer config_server;
ConfigThreshold config_threshold;



void Config::begin()
{
//    flash = new PFlash();
//    flash->begin();
    w25.begin();
    
    load_default();
    config_server.begin();
    config_threshold.begin();
    config_cal.begin();
    
    frame_len = para_to_frame();// update frame_len
//    flash->get(frame,frame_len);//update frame
    w25.read(frame,0,frame_len);
//    print_buf(frame,frame_len);
    cfgDebug("frame len:%d\n",frame_len);

    if(flag.value == frame_get_flag())//read flash flag
    {
        cfgDebug("标志位正确 flag:%d,frame len:%d\n",flag.value);
    }
    else
    {
        cfgDebug("标志位已经更改，加载预设值 0X%04X != 0X%04X\r\n",frame_get_flag(),flag.value);
        load_default();
        frame_len = para_to_frame();
        w25.write(frame,0,frame_len);
        w25.read(frame,0,frame_len);
//        flash->put(frame,frame_len);
//        flash->get(frame,frame_len);
//    print_buf(frame,frame_len);

    }

    if(frame_check(frame,frame_len) == false)
    {
        cfgDebug("设备信息加载失败,将使用预设值\r\n");
        frame_len = para_to_frame();
//        flash->put(frame,frame_len);
        w25.write(frame,0,frame_len);
    }
    else
    {
        frame_to_para();
        cfgDebug("设备信息加载成功\r\n");
    }
    //update backup
    update_backup();
//    print_buf(frame,frame_len);

}
void Config::load_default()   
{
    flag.value = 1234;
    
    sampleMode = SampleRegu;
    sample_regu_interval = 2;
    sample_regu_round = 3;
    sample_cont_duration = 2;
    sample_cont_interval = 10;
    gps_ctr = GpsOn;
    pipe_on_time = 12;
    pipe_off_time = 3;
    pipe_off_after = 50;
    modbus_id = 1;
    config_threshold.begin();
    
    //z2
    dev.begin();

    //z3
    config_server.begin();
    config_cal.begin();
}

void Config::save()
{
    uint32_t last = millis();
    for(int i = 0; i <frame_len; i++)
    {
        if(frame[i] != frame_backup[i])
        {
            w25.write(frame,0,frame_len);
//            flash->put(frame,frame_len);
            update_backup();
        }
    }
    cfgDebug("======save cost:[%d]=========================\n",millis() - last);

}
void Config::update_backup()
{
    for(int i = 0 ; i <frame_len; i++)
    {
        frame_backup[i] = frame[i];
    }
}

void Config::read()
{

}

int Config::frame_to_para()
{
    DataU16_t XUint16;
    uint16_t counter = 0;
    DataU16_t _flag;


    //---------z0 flag------------------
    _flag.byte[0] = frame[counter++];
    _flag.byte[1] = frame[counter++];
    
    //---------z1------------------
//    cfgDebug("z1 start:%d\n",counter);


    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = frame[counter++];
    modbus_id = XUint16.value;
    
    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = frame[counter++];
    sampleMode = (SampleMode_t)XUint16.value;
    
    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = frame[counter++];
    sample_regu_interval = XUint16.value;
    
    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = frame[counter++];
    sample_regu_round = XUint16.value;
    
    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = frame[counter++];
    sample_cont_duration = XUint16.value;
    
    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = frame[counter++];
    sample_cont_interval = XUint16.value;
    
    
    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = frame[counter++];
    gps_ctr = (GpsCtr_t)XUint16.value;
    
    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = frame[counter++];
    pipe_on_time = XUint16.value;
    
    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = frame[counter++];
    pipe_off_time = XUint16.value;
    
    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = frame[counter++];
    pipe_off_after = XUint16.value;

    for(int i = 0; i < config_threshold.len; i++)
        config_threshold.buf[i] = frame[counter++];
    config_threshold.buf_to_para();
    

    
    frame_z1_len= counter - (frame_z0_len );

    //---------z2 factory config---------
    for(int i = 0; i < 32; i++)
        dev.model[i] = frame[counter++];
    for(int i = 0; i < 2; i++)
        XUint16.byte[i] = frame[counter++];
    dev.type = XUint16.value;
        
    frame_z2_len= counter - (frame_z0_len + frame_z1_len);
    
    //---------z3 mqtt server config---------

    for(int i = 0; i < config_server.len; i++)
        config_server.buf[i] = frame[counter++];
    config_server.buf_to_para();
    frame_z3_len= counter - (frame_z0_len + frame_z1_len+ frame_z2_len);



    //---------z4 calibration config------------------
    for(int i = 0; i < config_cal.len; i++)
        config_cal.buf[i] = frame[counter++];
    config_cal.buf_to_para();


    return counter;
}


int Config::para_to_frame()
{
    DataU16_t crcValue;
    DataU16_t XUint16;
    DataU32_t XUint32;
    uint16_t counter = 0;
    
    //---------flag------------------
    frame[counter++] = flag.byte[0];
    frame[counter++] = flag.byte[1];
    frame_z0_len = counter;
//    cfgDebug("frame_z0_len:%d\n",frame_z0_len);

    
    //---------z1------------------
    XUint16.value = modbus_id;
    for(int i = 0; i < 2; i++)
        frame[counter++] = XUint16.byte[i];    
    
    XUint16.value = 0;//samplemode not need save
    for(int i = 0; i < 2; i++)
        frame[counter++] = XUint16.byte[i];

    XUint16.value = sample_regu_interval;
    for(int i = 0; i < 2; i++)
        frame[counter++] = XUint16.byte[i];

    XUint16.value = sample_regu_round;
    for(int i = 0; i < 2; i++)
        frame[counter++] = XUint16.byte[i];

    XUint16.value = sample_cont_duration;
    for(int i = 0; i < 2; i++)
        frame[counter++] = XUint16.byte[i];

    XUint16.value = sample_cont_interval;
    for(int i = 0; i < 2; i++)
        frame[counter++] = XUint16.byte[i];

    XUint16.value = gps_ctr;
    for(int i = 0; i < 2; i++)
        frame[counter++] = XUint16.byte[i];

    XUint16.value = pipe_on_time;
    for(int i = 0; i < 2; i++)
        frame[counter++] = XUint16.byte[i];
    XUint16.value = pipe_off_time;
    for(int i = 0; i < 2; i++)
        frame[counter++] = XUint16.byte[i];
    XUint16.value = pipe_off_after;
    for(int i = 0; i < 2; i++)
        frame[counter++] = XUint16.byte[i];

    //threshold infomation
    config_threshold.para_to_buf();
    for(int i = 0; i < config_threshold.len; i++)
        frame[counter++] = config_threshold.buf[i];
        
        
    frame_z1_len= counter - frame_z0_len;
//    cfgDebug("frame_z1_len:%d\n",frame_z1_len);

    //---------z2------------------
    for(int i = 0; i < 32; i++)
        frame[counter++] = dev.model[i];
    XUint16.value = dev.type;
    for(int i = 0; i < 2; i++)
        frame[counter++] = XUint16.byte[i];

    frame_z2_len= counter - (frame_z0_len + frame_z1_len);


    //---------z3 mqtt config -----------------------------
    config_server.para_to_buf();
    for(int i = 0; i < config_server.len; i++)
        frame[counter++] = config_server.buf[i];
    frame_z3_len= counter - (frame_z0_len + frame_z1_len+ frame_z2_len);

    //---------z4 calibration--------------------------------
    config_cal.para_to_buf();
    for(int i = 0; i < config_cal.len; i++)
        frame[counter++] = config_cal.buf[i];
    frame_z4_len= counter - (frame_z0_len + frame_z1_len + frame_z2_len + frame_z3_len);
    
    //    cfgDebug("frame_z2_len:%d\n",frame_z2_len);

    //---------crc------------------------------------------
    crcValue.value = crc16(frame,counter);
    for(int i = 0; i < 2; i++)
    {
        frame[counter++] = crcValue.byte[i];
    }   
    
    frame_len = counter;
//    cfgDebug("frame_len:%d\n",frame_len);
    return counter;
}
void Config::para_limit()
{
   
    
//    limit(modbus_id,(uint16_t)1,(uint16_t)240);
//    limit(sampleMode,(SampleMode_t)0,(SampleMode_t)1);
//    limit(sample_regu_interval,(uint16_t)2,(uint16_t)10080);
//    limit(sample_regu_round,(uint16_t)3,(uint16_t)10);
//    limit(sample_cont_duration,(uint16_t)1,(uint16_t)999);
//    limit(sample_cont_interval,(uint16_t)1,(uint16_t)999);
//    limit(gps_ctr,(GpsCtr_t)0,(GpsCtr_t)1);
//    limit(pipe_on_time,(uint16_t)10,(uint16_t)30);
//    limit(pipe_off_time,(uint16_t)2,(uint16_t)5);
//    limit(pipe_off_after,(uint16_t)20,(uint16_t)100);

    
    limit(modbus_id,(uint16_t)1,(uint16_t)240);
    limit(sampleMode,(SampleMode_t)0,(SampleMode_t)1);
    limit(sample_regu_interval,(uint16_t)1,(uint16_t)10080);
    limit(sample_regu_round,(uint16_t)3,(uint16_t)999);
    limit(sample_cont_duration,(uint16_t)1,(uint16_t)999);
    limit(sample_cont_interval,(uint16_t)1,(uint16_t)999);
    limit(gps_ctr,(GpsCtr_t)0,(GpsCtr_t)1);
    limit(pipe_on_time,(uint16_t)10,(uint16_t)999);
    limit(pipe_off_time,(uint16_t)2,(uint16_t)999);
    limit(pipe_off_after,(uint16_t)20,(uint16_t)999);
}
    

uint16_t Config::frame_get_flag()
{
    DataU16_t _flag;
    
    _flag.byte[0] = frame[0];
    _flag.byte[1] = frame[1];
    return _flag.value;
}

bool Config::frame_check(uint8_t *buf,uint16_t len)
{
    DataU16_t crcValue1;
    DataU16_t crcValue2;
    crcValue1.byte[0] = buf[len - 2];
    crcValue1.byte[1] = buf[len - 1];
    
    crcValue2.value = crc16(buf,len - 2);
    cfgDebug("crc读取结果:0X%04X\r\n",crcValue1.value);
    cfgDebug("crc计算结果:0X%04X\r\n",crcValue2.value);

    if(crcValue1.value == crcValue2.value)
    {
        
        cfgDebug("crc正确\r\n");
        return true;
    }
    else
    {
        cfgDebug("crc错误\r\n");
        return false;
    }
}


void Config::print(Uart &uart)
{
   
    cfgDebug("===========设备配置信息===========\n");
    cfgDebug("标志位:0x%04x\n",flag.value);    
    cfgDebug("sampleMode:%d\n",sampleMode);    
    cfgDebug("sample_regu_interval:%d\n",sample_regu_interval);    
    cfgDebug("sample_regu_round:%d\n",sample_regu_round);    
    cfgDebug("sample_cont_duration:%d\n",sample_cont_duration);    
    cfgDebug("sample_cont_interval:%d\n",sample_cont_interval);    
    cfgDebug("gps_ctr:%d\n",gps_ctr);    
    cfgDebug("pipe_on_time:%d\n",pipe_on_time);    
    cfgDebug("pipe_off_time:%d\n",pipe_off_time);    
    cfgDebug("pipe_off_after:%d\n",pipe_off_after);    
    cfgDebug("modbus_id:%d\n",modbus_id);    
    cfgDebug("================================\n");
    
    config_threshold.print(uart);
    config_server.print();
    config_cal.print(uart);
    dev.print(uart);
}


void Dev::begin()
{
    rt_memset(id,0,32);
    rt_memset(model,0,32);
    for(int i = 0 ; i <12; i++)
    {
        rt_snprintf(&id[i*2],3,"%02X",cpu.chip_id[i]);
    }
//    for(int i = 0; i < 31; i++)
//    {
//        model[i] = '0' + i%10;
//    }
    rt_snprintf(model,32,"%s"," GW-Z-C-0-4G-0");
   
    type = 0;
    version = 100;
    
    
    rt_memset(iccid,0,32);
    rt_memset(imei,0,32);
//    for(int i = 0; i < 31; i++)
//    {
//        iccid[i] = '0' + i%10;
//    }
//    
//    for(int i = 0; i < 31; i++)
//    {
//        imei[i] = '0' + i%10;
//    }
    para_to_buf();
}
int Dev::para_to_buf()
{
    DataU16_t XUint16;    
    DataFloat_t xFloat;    
    uint16_t counter = 0;

    for(int i = 0; i < 32; i++)
        buf[counter++] = id[i];
    
    for(int i = 0; i < 32; i++)
        buf[counter++] = dev.model[i];
    
    XUint16.value = type;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];
    
    XUint16.value = version;
    for(int i = 0; i < 2; i++)
        buf[counter++] = XUint16.byte[i];

    for(int i = 0; i < 32; i++)
        buf[counter++] = iccid[i];
    
    for(int i = 0; i < 32; i++)
        buf[counter++] = imei[i];
    
    len = counter;
    return counter;
}


void Dev::print(Stream &uart)
{
    cfgDebug("======dev(%d)===========\n",len);
    cfgDebug("id:%s\n",id);    
    cfgDebug("model:%s\n",model);    
    cfgDebug("type:%d\n",type);    
    cfgDebug("version:%d\n",version);    
    cfgDebug("iccid:%s\n",iccid);    
    cfgDebug("imei:%s\n",imei);    

}

