#ifndef __4G_H
#define __4G_H


#include "ebox_core.h"
#include "ebox_uart.h"


class StringArray
{
    public:
        StringArray(){_size = 0;};
        bool add(String s)
        {
            if(_size < 10)
            {
                str[_size] = s;
                _size++;
                return true;
            }
            return false;
        };
        String get(int index)
        {
            return str[index];
        };
        String get(const char* start)
        {
            for(int i = 0; i < _size; i++)
            {
                if(str[i].startsWith(start))
                return str[i];
            }
            return "";            
        };
        void print(Uart &uart)
        {
            for(int i = 0; i < _size; i++)
            {
                uart.printf("[%d]:%s\n",i,str[i].c_str());
            }
        };
        int size(){return _size;};
    private:
        String str[10];
        int _size;
};
StringArray strSplit(String &str);
StringArray strSplit(String &str,const char *key);

typedef struct
{
    String server;
    uint16_t port;
    String clientid;
    uint16_t keeplive;
    bool clearsession;
    String name;
    String password;
}MqttConnInfo_t;

typedef enum 
{
    Gm5NetConn,
    Gm5MqttConn,
}GM5EventType_t;

typedef enum 
{
    Gm5StateNone,
    Gm5StateInterfaceInited,
    Gm5StateStarted ,
    Gm5StateMqttConnecting,
    Gm5StateMqttConnected,
    Gm5StateMqttSubOK ,
}GM5State_t;

typedef void (*UsrGm5Callback)(String &package);
typedef void (*UsrGm5ConnChanged)(GM5EventType_t type,bool state);
class UsrGM5
{
    public:
        UsrGM5(Gpio *rst,Gpio *reload,Uart *u);
        void begin();
        int kick();
        String wkmod(String wkmod = "");
        int save();
        int restart_soft();
        int restart_hard();
        int set_cgact(uint8_t state,uint8_t cid);//pdp context active
        int get_cgact();//pdp context active
        int get_iccid();//
        int get_imei();//
        int get_csq( );//RSSI =CSQ *2-113
        int get_sysinfo();//
        int get_cclk();//
        int mqttconn(MqttConnInfo_t *info);
        int mqttpub(String topic,String msg,uint8_t qos = 0,uint8_t duplicate = 0,uint8_t retain = 0);
        int mqttsubunsub(String topic,uint8_t subflag = 1,uint8_t qos = 0);
        int mqttsta();

        void loop();
        void paraseLoop(String &str);
        int paraseErr(StringArray &sa);
        int reset();
        bool is_ready();
        int wait_ready();
        int wait_subok();
        int get_state();
    
        void attach(UsrGm5Callback hand);
        void attach_conn_changed(UsrGm5ConnChanged hand);
    public:
        Gpio *rst;
        Gpio *reload;
        Uart *uart;
    
    
        GM5State_t mState;
        uint32_t last_state_change_time;
        uint32_t is_stable;
        bool mqtt_state;
        bool net_connected;   
    
        int cscon;
        int creg;
        int cereg;
        int ctzv;
        uint8_t cgact_state;
        uint8_t cgact_cid;
        String iccid;
        String imei;
        int csq;
        String sysinfo_mode;
        String clk_dt;

    private:
        rt_mutex_t mutex;
        UsrGm5Callback handler;
        UsrGm5ConnChanged conn_handler;
};


#endif

