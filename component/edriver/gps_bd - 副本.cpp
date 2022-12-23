#include "gps_bd.h"
#include "calendar.h"

callback_fun_type sec_user_event = 0;

BD bd(&uart1,&PA12);

void BD::rx_event()
{
    u8 ch;
    ch = usart->read();
    last_uart_event_time = millis();
	if(ch == '$')
	{
		cmd_num++;
	}		
	if(char_count>=BUF_SIZE)char_count = 0;//防止数据溢出
	cmd_buf[char_count++] = ch;
}

void BD::sec_interrupt_inner_event()
{
	bd.sec_interrupt_feed_dog();
	if(sec_user_event == 0)//防止中断跳转异常；
		return;
	sec_user_event();
}

void BD::begin(u32 baud_rate)
{
	state = OFFLINE;
	info_init();
	usart->begin(baud_rate);
    usart->attach(this,&BD::rx_event,RxIrq);
	Exti::begin();
	Exti::attach(this,&BD::sec_interrupt_inner_event);
	Exti::interrupt(ENABLE,2,2);
}



void BD::info_init()
{
	info.utc_time[10]='\0';
	info.latitude[9]='\0';
	info.longitude[10]='\0';
	info.utc_date[6]='\0';
}

void	BD::sec_interrupt_feed_dog()
{
	sec_interrupt_current_time = millis();
}
int		BD::sec_interrupt_check_dog()
{
	if(millis() - sec_interrupt_current_time > SEC_INTERRUPT_TIMEOUT)	
	{
		return -1;
	}
	else
		return 0;

}


void BD::attch_sec_interruput_user_event(void (*callbackFun)(void))
{
	sec_user_event = callbackFun;
}

int BD::find_str(u8 *str,u8 *ptr,u16 &seek)
{
	seek = 0;
	u8 *temp_str=NULL;
	u8 *temp_ptr=NULL;
	u8 *temp_char=NULL;
	if(0==str||0==ptr)
		return -2;
	for(temp_str=str;*temp_str!='\0';temp_str++)	 //依次查找字符串
	{
		temp_char=temp_str; //指向当前字符串
		//比较
		for(temp_ptr=ptr;*temp_ptr!='\0';temp_ptr++)
		{	
			if(*temp_ptr!=*temp_char)
			break;
			temp_char++;
		}
		if(*temp_ptr=='\0')  //出现了所要查找的字符，退出
		{
			return 0;
		}
		seek++;  //当前偏移量加1
	}
	return -1;
}

#include "stdio.h"
#include "string.h"
void BD::deal_bd_data()
{
    u8 item_count = 0;
	u8 bd_info_item_seek = 0;
	u16 seek = 0;
	u8 ret = 0;
	u8 flag = 0;
	u8 *p_cmd = NULL;
    
    //获取时间
    memset(&info,0x00,sizeof(info));//清除结构体	
    ret = find_str(cmd_buf,(u8 *)"$GNRMC",seek);
    if(ret == 0)
    {
        p_cmd = cmd_buf + seek;
        do
        {
            p_cmd++;
            switch(*p_cmd)
            {
                case ',':
                    item_count++;
                    bd_info_item_seek = 0;
                    break;
                default :
                    switch(item_count)
                    {
                        case 0:;break;
                        case 1:info.utc_time[bd_info_item_seek]		= *p_cmd;break;
                        case 2:info.statue                          = *p_cmd;break;
                        case 3:info.latitude[bd_info_item_seek]		= *p_cmd;break;
                        case 4:info.latitude_NS                     = *p_cmd;break;
                        case 5:info.longitude[bd_info_item_seek] 	= *p_cmd;break;
                        case 6:info.longitude_EW                    = *p_cmd;break;
                        case 7:info.speed[bd_info_item_seek]        = *p_cmd;break;
                        case 8:info.azimuth[bd_info_item_seek]		= *p_cmd;break;
                        case 9:info.utc_date[bd_info_item_seek]		= *p_cmd;break;
                        default:break;								
                    }		
                    bd_info_item_seek++;						
                    break;				
            }				
        }while(*p_cmd!='*');
    }
    //获取卫星数量
    ret = find_str(cmd_buf,(u8 *)"$GNGGA",seek);
    item_count = 0;
    if(ret == 0)
    {
        p_cmd = cmd_buf + seek;
        do
        {
            p_cmd++;
            switch(*p_cmd)
            {
                case ',':
                    item_count++;
                    bd_info_item_seek = 0;
                    break;
                default :
                    switch(item_count)
                    {
                        case 7:info.satellite_num[bd_info_item_seek] = *p_cmd;break;
                        default:break;								
                    }		
                    bd_info_item_seek++;						
                    break;				
            }				
        }while(*p_cmd!='*');
    }
    char_count = 0;
    cmd_num = 0;
}
int BD::process()
{

	if(sec_interrupt_check_dog() == -1)
	{
		state = OFFLINE;
	}
	else
	{
        state = ONLINE;
	}
    if(cmd_num > 0)
    {
        if(millis() - last_uart_event_time > CMD_TIMEOUT)
        {
            deal_bd_data();
            return 1;
        }
	}
	return 0;
}

