#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/tcp.h"
#include "lwip/init.h"
#include "netif/etharp.h"
#include "lwip/udp.h"
#include "lwip/pbuf.h"
#include <stdio.h>	
#include <string.h>
#include "tcpclient.h"
#include "bsp_ebox.h"

#define DEST_IP_ADDR0 192
#define DEST_IP_ADDR1 168
#define DEST_IP_ADDR2 0
#define DEST_IP_ADDR3 100


static void client_err(void *arg, err_t err)       //出现错误时调用这个函数，打印错误信息，并尝试重新连接
{
  uart1.printf("连接错误!!\n");
	 uart1.printf("尝试重连!!\n");
  
  //连接失败的时候释放TCP控制块的内存
	 uart1.printf("关闭连接，释放TCP控制块内存\n");
  //tcp_close(client_pcb);
	  
  
  //重新连接
	 uart1.printf("重新初始化客户端\n");
	TCP_Client_Init();
	
}


static err_t client_send(void *arg, struct tcp_pcb *tpcb)   //发送函数，调用了tcp_write函数
{
  uint8_t send_buf[]= "我是客户端，是你的好哥哥\n";
  
  //发送数据到服务器
  tcp_write(tpcb, send_buf, sizeof(send_buf), 1); 
  
  return ERR_OK;
}

static err_t client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  if (p != NULL) 
  {        
    /* 接收数据*/
    tcp_recved(tpcb, p->tot_len);
      
    /* 返回接收到的数据*/  
    tcp_write(tpcb, p->payload, p->tot_len, 1);
    memset(p->payload, 0 , p->tot_len);
    pbuf_free(p);
  } 
  else if (err == ERR_OK) 
  {
    //服务器断开连接
     uart1.printf("服务器断开连接!\n");
    tcp_close(tpcb);
    
    //重新连接
    TCP_Client_Init();
  }
  return ERR_OK;
}

static err_t client_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
   uart1.printf("connected ok!\n");
  
  //注册一个周期性回调函数
  tcp_poll(pcb,client_send,2);
  
  //注册一个接收函数
  tcp_recv(pcb,client_recv);
  
  return ERR_OK;
}


void TCP_Client_Init(void)
{        
	struct tcp_pcb *client_pcb = NULL;   //这一句一定要放在里面，否则会没用
  ip4_addr_t server_ip;     //因为客户端要主动去连接服务器，所以要知道服务器的IP地址
  /* 创建一个TCP控制块  */
  client_pcb = tcp_new();	  

  IP4_ADDR(&server_ip, DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3);//合并IP地址

   uart1.printf("客户端开始连接!\n");
  
  //开始连接
  tcp_connect(client_pcb, &server_ip, TCP_CLIENT_PORT, client_connected);
	ip_set_option(client_pcb, SOF_KEEPALIVE);	
	
	 uart1.printf("已经调用了tcp_connect函数\n");
  
  //注册异常处理
  tcp_err(client_pcb, client_err);
	 uart1.printf("已经注册异常处理函数\n");	
}
