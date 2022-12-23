#ifndef __GPS_BD_H
#define __GPS_BD_H
#include "ebox.h"
#include "calendar.h"

typedef enum{
	OFFLINE	= 0,
	ONLINE	= 1
}STATE;
//RMC
typedef struct
{
	//RMC֡����
	uint8_t utc_time[11]; //ʱ��
	uint8_t statue;   //��λ״̬
	uint8_t latitude[10];	//γ��
	uint8_t latitude_NS; //γ�Ȱ���
	uint8_t longitude[11];	 //����	
	uint8_t longitude_EW;//���Ȱ���
	uint8_t speed[6];	     //��������
	uint8_t azimuth[6];	 //����
	uint8_t utc_date[7]; //����	  

	//GGA֡����
	uint8_t altitude[8];   //�߶�
	uint8_t satellite_num[3];//������ 
}INFO_STRUCT; 

#define CMD_TIMEOUT 5//����10msû�л���������ݣ���Ϊ����������
#define SEC_INTERRUPT_TIMEOUT 1500//����1.5sû�����жϣ���Ϊ����

#define BUF_SIZE 2048//���������С�������Լ��������塣

class BD
{
	public:
		STATE state;//��������״̬��
		INFO_STRUCT info;
	public:
        
		BD(Uart *usart,Gpio *sec_pin)
		{
			this->usart = usart;
			this->sec_pin = sec_pin;
		}
		void	begin(uint32_t baud_rate);//��ʼ��
		int	    process();//ѭ��������յ���֡���ݣ�����н�����ɵ������ʼ��������
		void    deal_bd_data();


	private:
        uint8_t	    cmd_buf[BUF_SIZE];
		uint8_t		cmd_num;
		uint16_t		char_count;
		uint32_t		sec_interrupt_current_time;
		uint32_t		last_uart_event_time;
		Gpio	*sec_pin;
		Uart *usart;
	
		void	rx_event(void);//�����ж����BD����
        void    sec_interrupt_inner_event();//���ж�
		void	info_init();
		int		find_str(uint8_t *str,uint8_t *ptr,uint16_t &seek);
	
		int		sec_interrupt_check_dog();//������жϳ�ʱ�������ʱ˵��BD����
		void	sec_interrupt_feed_dog();//���ж�ι��	�����ڱ�����������	

		
	    friend void sec_interrupt_inner_event();//��Ԫ��������Ҫ����˽�г�Ա����sec_interrupt_feed_dog();
		friend void uart_interrupt_event();//��Ԫ��������Ҫ����˽�г�Աget_char();

};
extern BD bd;



#endif
