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


static void client_err(void *arg, err_t err)       //���ִ���ʱ���������������ӡ������Ϣ����������������
{
  uart1.printf("���Ӵ���!!\n");
	 uart1.printf("��������!!\n");
  
  //����ʧ�ܵ�ʱ���ͷ�TCP���ƿ���ڴ�
	 uart1.printf("�ر����ӣ��ͷ�TCP���ƿ��ڴ�\n");
  //tcp_close(client_pcb);
	  
  
  //��������
	 uart1.printf("���³�ʼ���ͻ���\n");
	TCP_Client_Init();
	
}


static err_t client_send(void *arg, struct tcp_pcb *tpcb)   //���ͺ�����������tcp_write����
{
  uint8_t send_buf[]= "���ǿͻ��ˣ�����ĺø��\n";
  
  //�������ݵ�������
  tcp_write(tpcb, send_buf, sizeof(send_buf), 1); 
  
  return ERR_OK;
}

static err_t client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  if (p != NULL) 
  {        
    /* ��������*/
    tcp_recved(tpcb, p->tot_len);
      
    /* ���ؽ��յ�������*/  
    tcp_write(tpcb, p->payload, p->tot_len, 1);
    memset(p->payload, 0 , p->tot_len);
    pbuf_free(p);
  } 
  else if (err == ERR_OK) 
  {
    //�������Ͽ�����
     uart1.printf("�������Ͽ�����!\n");
    tcp_close(tpcb);
    
    //��������
    TCP_Client_Init();
  }
  return ERR_OK;
}

static err_t client_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
   uart1.printf("connected ok!\n");
  
  //ע��һ�������Իص�����
  tcp_poll(pcb,client_send,2);
  
  //ע��һ�����պ���
  tcp_recv(pcb,client_recv);
  
  return ERR_OK;
}


void TCP_Client_Init(void)
{        
	struct tcp_pcb *client_pcb = NULL;   //��һ��һ��Ҫ�������棬�����û��
  ip4_addr_t server_ip;     //��Ϊ�ͻ���Ҫ����ȥ���ӷ�����������Ҫ֪����������IP��ַ
  /* ����һ��TCP���ƿ�  */
  client_pcb = tcp_new();	  

  IP4_ADDR(&server_ip, DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3);//�ϲ�IP��ַ

   uart1.printf("�ͻ��˿�ʼ����!\n");
  
  //��ʼ����
  tcp_connect(client_pcb, &server_ip, TCP_CLIENT_PORT, client_connected);
	ip_set_option(client_pcb, SOF_KEEPALIVE);	
	
	 uart1.printf("�Ѿ�������tcp_connect����\n");
  
  //ע���쳣����
  tcp_err(client_pcb, client_err);
	 uart1.printf("�Ѿ�ע���쳣������\n");	
}
