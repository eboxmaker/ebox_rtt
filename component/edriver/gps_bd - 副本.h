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
	u8 utc_time[11]; //ʱ��
	u8 statue;   //��λ״̬
	u8 latitude[10];	//γ��
	u8 latitude_NS; //γ�Ȱ���
	u8 longitude[11];	 //����	
	u8 longitude_EW;//���Ȱ���
	u8 speed[6];	     //��������
	u8 azimuth[6];	 //����
	u8 utc_date[7]; //����	  

	//GGA֡����
	u8 altitude[8];   //�߶�
	u8 satellite_num[3];//������ 
}INFO_STRUCT; 

#define CMD_TIMEOUT 5//����10msû�л���������ݣ���Ϊ����������
#define SEC_INTERRUPT_TIMEOUT 1500//����1.5sû�����жϣ���Ϊ����

#define BUF_SIZE 2048//���������С�������Լ��������塣

class BD:public Exti
{
	public:
		STATE state;//��������״̬��
		INFO_STRUCT info;
	public:
        
		BD(Uart *usart,Gpio *sec_pin):Exti(sec_pin,EXTI_Trigger_Falling)//�������жϵ�io�ӿ�
		{
			this->usart = usart;
			this->sec_pin = sec_pin;
		}
		void	begin(u32 baud_rate);//��ʼ��
		int	    process();//ѭ��������յ���֡���ݣ�����н�����ɵ������ʼ��������
		void	attch_sec_interruput_user_event(void (*callbackFun)(void));//���жϷ����û����������ж���ִ���Լ��ĺ���
		void    deal_bd_data();


	private:
        u8	    cmd_buf[BUF_SIZE];
		u8		cmd_num;
		u16		char_count;
		u32		sec_interrupt_current_time;
		u32		last_uart_event_time;
		Gpio	*sec_pin;
		Uart *usart;
	
		void	rx_event(void);//�����ж����BD����
        void    sec_interrupt_inner_event();//���ж�
		void	info_init();
		int		find_str(u8 *str,u8 *ptr,u16 &seek);
	
		int		sec_interrupt_check_dog();//������жϳ�ʱ�������ʱ˵��BD����
		void	sec_interrupt_feed_dog();//���ж�ι��	�����ڱ�����������	

		void	attch_sec_interruput_inner_event(void (*callbackFun)(void));
		
	    friend void sec_interrupt_inner_event();//��Ԫ��������Ҫ����˽�г�Ա����sec_interrupt_feed_dog();
		friend void uart_interrupt_event();//��Ԫ��������Ҫ����˽�г�Աget_char();

};
extern BD bd;



#endif
