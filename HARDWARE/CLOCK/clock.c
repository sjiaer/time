#include "clock.h"
#include "key.h"
#include "remote.h"
#include "oled.h"
#include "stop_watch.h"
#include "cutdown.h"
#include "ds1302.h"  
#include "ds18b20.h"
#include "Nixie_tube.h"

enum state{Stop_watch,Cutdown,DS1302_time,DS1302_alarm};
uint8_t Nixie_tube_minute;
uint8_t Nixie_tube_hour;
void clock(void)
{
	static uint8_t key,remote_key=0;
	static enum state s=DS1302_time;
	uint8_t temp;
	//数码管时和分读取
	temp=ds1302_Read(DS1302_MINUTE|1); 
	Nixie_tube_minute=(temp&0x0f)+(temp>>4)*10;  	
	temp=ds1302_Read(DS1302_HOUR|1);  
	Nixie_tube_hour=(temp&0x0f)+(temp>>4)*10; 

	
  key=key_read();
	remote_key=Remote_Scan();
	switch(s)
	{
		case DS1302_time:        //时钟
			time_ds1302(key,remote_key);
			temp_meature();
			if(key==WKUP_PRES||remote_key==RIGHT)
			{
				s=DS1302_alarm;
				OLED_Clear();
			}
			if(remote_key==LEFT)
				{
					s=Cutdown;
					OLED_Clear();
				}
			break;
				
		case DS1302_alarm:       //闹钟设定
			ds1302_alarm(key,remote_key);
			if(key==WKUP_PRES||remote_key==RIGHT)
			{
				s=Stop_watch;
				OLED_Clear();
			}
			if(remote_key==LEFT)
				{
					s=DS1302_time;
					OLED_Clear();
				}
			break;
				
		case Stop_watch:          //秒表
			stop_watch(key,remote_key);
			if(key==WKUP_PRES||remote_key==RIGHT)
				{
					s=Cutdown;
					OLED_Clear();
				}
				if(remote_key==LEFT)
				{
					s=DS1302_alarm;
					OLED_Clear();
				}
			break;
				
		case Cutdown:	           //定时器
			cutdown(key,remote_key);
			if(key==WKUP_PRES||remote_key==RIGHT)
				{
					s=DS1302_time;
					OLED_Clear();
				}
			if(remote_key==LEFT)
				{
					s=Stop_watch;
					OLED_Clear();
				}
			break;

		default:
			break;	
	}
	
	OLED_Refresh_Gram();//显示时间	
	
 	if(remote_key==104) //遥控器0
	{
		HAL_NVIC_SystemReset(); //系统复位
	}
	
	alarm_ring(key,remote_key); //闹钟
}

//数码管显示
void ds1302_Nixie_tube_display(void)
{
	Nixie_tube_display(Nixie_tube_hour,Nixie_tube_minute);	
}
