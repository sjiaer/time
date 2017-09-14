#include "stop_watch.h"
#include "timer.h"
#include "oled.h"
#include "key.h"
#include "remote.h"
#include "text.h"

uint8_t stop_watch_h=0,stop_watch_m=0,stop_watch_s=0,stop_watch_t=0,stop_watch_count=0;
uint8_t i=0,stop_watch_flag=0;
char t[1000][4]; 
char time_buff[24]={0};

void stop_watch_running(void)
{
	if(stop_watch_flag == 0)
		return;
	
	stop_watch_t++;
	if(stop_watch_t>19) //50*20=1000ms
	{
		stop_watch_t=0;
		stop_watch_s++;
		if(stop_watch_s==60)
		{
			stop_watch_s=0;
			stop_watch_m++;
			if(stop_watch_m==60)
			{
				stop_watch_m=0;
				stop_watch_h++;
			}
		}
	}
}
		

enum Stop_watch_state{stop_watch_initial_state,stop_watch_run,stop_watch_pause};

void stop_watch(uint8_t key,uint8_t remote_key)
{
	static enum Stop_watch_state stop_watch_status=stop_watch_initial_state;
	static uint8_t display_t1;
	static uint8_t display_t2;
	
	//显示
	//OLED_ShowString(24,0,"stop_watch",16);
	OLED_Show_Font(44,0,0); //秒
	OLED_Show_Font(44+24,0,1); //表
	sprintf(time_buff,"%02d:%02d:%02d:%02d",stop_watch_h,stop_watch_m,stop_watch_s,stop_watch_t*5); 
	OLED_ShowString(20,16,(uint8_t*)time_buff,16);
	if(display_t1==1) //显示这次计次
	{
		sprintf(time_buff,"%02d:%02d:%02d:%02d",t[i-1][0],t[i-1][1],t[i-1][2],t[i-1][3]); 
		OLED_ShowString(20,32,(uint8_t*)time_buff,16);  
	}
	if(display_t2==1)//显示上次计次
	{
		sprintf(time_buff,"%02d:%02d:%02d:%02d",t[i-2][0],t[i-2][1],t[i-2][2],t[i-2][3]); 
		OLED_ShowString(20,48,(uint8_t*)time_buff,16);  
	}
	

	
	switch(stop_watch_status)
	{
		case stop_watch_initial_state:  //初始状态
			stop_watch_h=0,stop_watch_m=0,stop_watch_s=0,stop_watch_t=0;
		  i=0;display_t1=0;display_t2=0;
		  OLED_ShowString(20,32,"           ",16);
		  OLED_ShowString(20,48,"           ",16);
			if(key==KEY0_PRES||remote_key==PLAY)
			{
				stop_watch_status=stop_watch_run;
			}
			break;
		
		case stop_watch_run:   //计时
			stop_watch_flag=1;
		  if(key==KEY0_PRES||remote_key==PLAY)  //按键0转暂停
				{
					stop_watch_status=stop_watch_pause;
				}
		  if(key==KEY2_PRES||remote_key==EQ)  //按键2计次
			{
				display_t1=1;
				t[i][0]=stop_watch_h;
				t[i][1]=stop_watch_m;
				t[i][2]=stop_watch_s;
				t[i][3]=stop_watch_t*5;
				if(i!=0)
					display_t2=1;
				i++;		
			}
			break;
		
		case stop_watch_pause:
			stop_watch_flag=0;
			if(key==KEY0_PRES||remote_key==PLAY)
				{
					stop_watch_status=stop_watch_run;
				}
			if(key==KEY2_PRES||remote_key==CH)
				{
					stop_watch_status=stop_watch_initial_state;
				}
			break;
				
		default:
			break;
	}
}
