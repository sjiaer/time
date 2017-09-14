#include "cutdown.h"
#include "sys.h"
#include "timer.h"
#include "oled.h"
#include "key.h"
#include "stdio.h"
#include "remote.h"
#include "text.h"

uint8_t cutdown_h=0;
uint8_t cutdown_m=0;
uint8_t cutdown_s=0;
uint8_t cutdown_start=0;
uint8_t flag_twinkle=0;
uint8_t beep_enable=0;
uint8_t beep_time=0;
uint8_t cutdown_k=1;

void cutdown_running(void)
{
	static uint32_t cutdown_time_50ms = 0;
	cutdown_time_50ms++;
	
	if(cutdown_time_50ms%5==0)
	{
		flag_twinkle=!flag_twinkle;
	}
	
	if(cutdown_start == 1)
	{
		if(cutdown_time_50ms%20 == 0)
		{
			if(cutdown_s>0)
			{
				cutdown_s--;
			}
			else if(cutdown_m>0)
			{
				cutdown_m--;
				cutdown_s=59;
			}
			else if(cutdown_h>0)
			{
				cutdown_h--;
				cutdown_m=59;
				cutdown_s=59;
			}
		}
	}	
		
 	if(beep_enable&&beep_time) 
	{
		beep_time--;
		if(cutdown_time_50ms%4==0)
			HAL_GPIO_TogglePin(BEEP_GPIO_Port,BEEP_Pin);
	} 
}



//主函数
enum Cutdown_state{cutdown_set,cutdown_run,cutdown_finish,cutdown_initial_state};
void cutdown(uint8_t key,uint8_t remote_key)
{	
	static enum Cutdown_state cutdown_status=cutdown_initial_state;
	static char time_buff_cutdown[24]={0};
//	OLED_ShowString(36,0,"cutdown",16);
	OLED_Show_Font(32,0,2);     //定
	OLED_Show_Font(32+24,0,3);  //时
	OLED_Show_Font(32+48,0,4);  //器
	
	switch(cutdown_status)
	{
		case cutdown_initial_state:
			cutdown_h=0;
			cutdown_m=0;
			cutdown_s=0;
		  sprintf(time_buff_cutdown,"%02d:%02d:%02d",cutdown_h,cutdown_m,cutdown_s);  
			OLED_ShowString(32,16,(uint8_t*)time_buff_cutdown,16);
      if(key==KEY0_PRES||remote_key==PLAY)
      {
				cutdown_status=cutdown_set;
			}				
			break;
		case cutdown_set:   //设置
			cutdown_set_display(key,remote_key);
			if((key==KEY0_PRES||remote_key==PLAY)&&(cutdown_h||cutdown_m||cutdown_s)) //切换状态
			{
				cutdown_status=cutdown_run;
				cutdown_start=1;
			}
			if((key==KEY0_PRES||remote_key==PLAY)&&(cutdown_h==0)&&(cutdown_m==0)&&(cutdown_s==0))
				cutdown_status=cutdown_initial_state;
			break;
						
		case cutdown_run:  //倒计时
			sprintf(time_buff_cutdown,"%02d:%02d:%02d",cutdown_h,cutdown_m,cutdown_s);  
			OLED_ShowString(32,16,(uint8_t*)time_buff_cutdown,16); 
			if(key==KEY0_PRES||remote_key==PLAY)
				cutdown_start=!cutdown_start;
			if(cutdown_start==0&&(key==KEY2_PRES||remote_key==CH))
				{
					cutdown_status=cutdown_initial_state;
				}
			else if((cutdown_h==0)&&(cutdown_m==0)&&(cutdown_s==0)) //切换状态
				{
					cutdown_status=cutdown_finish;
					cutdown_start=0;
					beep_time=200;
				}
		  break;
					
		case cutdown_finish:  //倒计时完成
			sprintf(time_buff_cutdown,"%02d:%02d:%02d",cutdown_h,cutdown_m,cutdown_s);  
			OLED_ShowString(32,16,(uint8_t*)time_buff_cutdown,16); 
			beep_enable=1;
			if(key==KEY2_PRES||remote_key==CH||beep_time==0)  
			{
				cutdown_status=cutdown_initial_state;
				beep_enable=0;
				HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
			}
			break;
			
		default:
			break;
	}
}

void cutdown_set_display(uint8_t key,uint8_t remote_key)
{
	static char time_buff_cutdown[24]={0};
	if(flag_twinkle==0)
	{    
		sprintf(time_buff_cutdown,"%02d:%02d:%02d",cutdown_h,cutdown_m,cutdown_s);  
		OLED_ShowString(32,16,(uint8_t*)time_buff_cutdown,16); 
	}
	else
	{
		if(cutdown_k==1) //时闪烁
		{
			sprintf(time_buff_cutdown,"  :%02d:%02d",cutdown_m,cutdown_s); 
			OLED_ShowString(32,16,(uint8_t*)time_buff_cutdown,16); 
		}
		if(cutdown_k==2) //分闪烁
		{
			sprintf(time_buff_cutdown,"%02d:  :%02d",cutdown_h,cutdown_s); 
			OLED_ShowString(32,16,(uint8_t*)time_buff_cutdown,16); 
		}
		if(cutdown_k==3) //秒闪烁
		{
			 sprintf(time_buff_cutdown,"%02d:%02d:  ",cutdown_h,cutdown_m); 
			 OLED_ShowString(32,16,(uint8_t*)time_buff_cutdown,16); 
		}
	} 
	
	if(key==KEY1_PRES||remote_key==CH_ADD) //切换时分秒
	{
		cutdown_k++;
		if(cutdown_k==4)
			cutdown_k=1;
	}
	if(remote_key==CH_CUT)
	{
		if(cutdown_k==1)
			cutdown_k=4;
		cutdown_k--;
	}
	
	if(key==KEY2_PRES||remote_key==VOL_ADD) 
	{
		if(cutdown_k==1)   //时+1
		 {
			 cutdown_h++;
			 if(cutdown_h==24)
				 cutdown_h=0;
		 }
		 if(cutdown_k==2)  //分+1
		 {
			 cutdown_m++;
			 if(cutdown_m==60)
				 cutdown_m=0;
		 }
		 if(cutdown_k==3)  //秒+1
		 {
			 cutdown_s++;
			 if(cutdown_s==60)
				 cutdown_s=0;
		 }
	}	
	if(remote_key==VOL_CUT) 
	{
		if(cutdown_k==1)   //时-1
		{
		 if(cutdown_h==0)
			 cutdown_h=24;
		 cutdown_h--;
		}
		if(cutdown_k==2)  //分-1
		{
		 if(cutdown_m==0)
			 cutdown_m=60;
		 cutdown_m--;
		}
		if(cutdown_k==3)  //秒-1
		{
		 if(cutdown_s==0)
			 cutdown_s=60;
		 cutdown_s--;
		}
	}		
}

