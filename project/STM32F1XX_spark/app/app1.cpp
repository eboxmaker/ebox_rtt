#include "app1.h"

#include "../Ethernet3/utility/w5500.h"
#include "../Ethernet3/Ethernet3.h"
#include "../Ethernet3/EthernetUdp3.h"         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"TCP client example"
#define EXAMPLE_DATE	"2018-08-11"


uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server_ip(192,168,3,203);
uint16_t server_port = 6000;


EthernetClient client;

uint32_t last_time = 0;


static void app1_thread_entry(void* paremeter)
{
    
    Ethernet.begin(mac);
    while(Ethernet.link() == 0)
    {
        uart1.printf(".");
        delay_ms(100);
    }
    if (client.connect(server_ip, server_port))
    {
        Serial.println("connected");
    }
    else
    {
        Serial.println("connection failed");
    }
    last_time = millis();
    while(1)
    {
        if (!client.connected())
        {
            client.stop();
            if (client.connect(server_ip, server_port))
            {
                Serial.println("connected");
            }
            else
            {
                Serial.println("connection failed");
            }
        }
        else
        {
            if (client.available())
            {
                char c = client.read();
                client.print(c);
            }

            if(millis() - last_time > 10)
            {
                last_time = millis();
                client.println("hello world");
                client.println();
            }
        }
        delay_ms(1000);
    }
}

int app1_create()
{
    rt_thread_t app1_thread = rt_thread_create( "app1",     /*name*/
                                    app1_thread_entry,/*入口*/
                                    RT_NULL,/*参数*/
                                    1024,    /*heap*/
                                    3 ,    /*优先级*/
                                     20);   /*时间片段*/
    if(app1_thread !=RT_NULL)
        rt_thread_startup (app1_thread);
    else 
        return -1;
    return 0;
    
}