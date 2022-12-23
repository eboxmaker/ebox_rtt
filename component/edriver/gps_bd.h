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
	//RMC帧内容
	uint8_t utc_time[11]; //时间
	uint8_t statue;   //定位状态
	uint8_t latitude[10];	//纬度
	uint8_t latitude_NS; //纬度半球
	uint8_t longitude[11];	 //经度	
	uint8_t longitude_EW;//经度半球
	uint8_t speed[6];	     //地面速率
	uint8_t azimuth[6];	 //航向
	uint8_t utc_date[7]; //日期	  

	//GGA帧内容
	uint8_t altitude[8];   //高度
	uint8_t satellite_num[3];//卫星数 
}INFO_STRUCT; 

#define CMD_TIMEOUT 5//超过10ms没有获得命令数据，认为命令接收完成
#define SEC_INTERRUPT_TIMEOUT 1500//超过1.5s没有秒中断，认为离线

#define BUF_SIZE 2048//命令缓冲区大小，根据自己的需求定义。

class BD
{
	public:
		STATE state;//卫星在线状态。
		INFO_STRUCT info;
	public:
        
		BD(Uart *usart,Gpio *sec_pin)
		{
			this->usart = usart;
			this->sec_pin = sec_pin;
		}
		void	begin(uint32_t baud_rate);//初始化
		int	    process();//循环处理接收到的帧内容，如果有接收完成的命令，开始解析命令
		void    deal_bd_data();


	private:
        uint8_t	    cmd_buf[BUF_SIZE];
		uint8_t		cmd_num;
		uint16_t		char_count;
		uint32_t		sec_interrupt_current_time;
		uint32_t		last_uart_event_time;
		Gpio	*sec_pin;
		Uart *usart;
	
		void	rx_event(void);//串口中断填充BD数据
        void    sec_interrupt_inner_event();//秒中断
		void	info_init();
		int		find_str(uint8_t *str,uint8_t *ptr,uint16_t &seek);
	
		int		sec_interrupt_check_dog();//检查秒中断超时，如果超时说明BD离线
		void	sec_interrupt_feed_dog();//秒中断喂狗	，用于保持卫星在线	

		
	    friend void sec_interrupt_inner_event();//友元函数，需要访问私有成员函数sec_interrupt_feed_dog();
		friend void uart_interrupt_event();//友元函数，需要访问私有成员get_char();

};
extern BD bd;



#endif
