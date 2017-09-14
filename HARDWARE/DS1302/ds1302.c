#include "ds1302.h"  
#include "stdio.h"
#include "oled.h"
#include "key.h"
#include "Nixie_tube.h"
#include "remote.h"
#include "delay.h"
#include "text.h"

TIME time;

//����ʱ��ı���
uint8_t time_hour=0;
uint8_t time_minute=0;
uint8_t time_second=0;
uint8_t time_year=0;
uint8_t time_month=1;
uint8_t time_date=1;
uint8_t time_week=1;
uint8_t time_k=1;
uint8_t time_getnum=0;

uint8_t time_get_flag=0;
uint8_t time_twinkle=0;

uint8_t ds1302_alarm_hour=0;	
uint8_t ds1302_alarm_minute=0;
uint8_t ds1302_alarm_second=0;
uint8_t ds1302_alarm_ring_flag=0;
uint32_t ds1302_alarm_ring_time=0;
uint8_t ds1302_alarm_enable=0;
uint8_t ds1302_alarm_set_hour=0;
uint8_t ds1302_alarm_set_min=0;
uint8_t ds1302_alarm_set_sec=0;
uint8_t ds1302_alarm_k=0;
uint8_t alarm_enable=0;
uint8_t alarm_getnum=0;

void DS1302_Init(void)  
{    
  Get_Time(&time); 
	if(time.hour>23)
	{
		ds1302_Write(DS1302_ENABLE,0);
		ds1302_Write(DS1302_HOUR,0);
		ds1302_Write(DS1302_ENABLE,0x80);
	}
	if(ds1302_alarm_hour>23||ds1302_alarm_minute>59||ds1302_alarm_second>59)
	{
		ds1302_Write(DS1302_ENABLE,0);
		ds1302_Write(DS1302_ALARM_HOUR,0);
		ds1302_Write(DS1302_ALARM_MIN,0);
		ds1302_Write(DS1302_ALARM_SEC,0);
		ds1302_Write(DS1302_ENABLE,0x80);
	}
} 

//ds1302ʱ��������
enum time_state{time_set,time_run,time_load};
void time_ds1302(uint8_t key,uint8_t remote_key)
{
	static enum time_state time_status=time_run; //��ʼ״̬Ϊrunģʽ
	char time_buffer[24]={0};
	if(time_get_flag==1)
		Get_Time(&time);           //ˢ��ʱ��
//	OLED_Show_Font(0,0,7);    //ʱ
//	OLED_Show_Font(16,0,8);   //��
//	OLED_Show_Font(0,16,9);   //��
//	OLED_Show_Font(16,16,10); //��
	OLED_Show_Font(16,32,11);  //��
	OLED_Show_Font(16+16,32,10); //��
	switch(time_status)
	{
		case time_run:  //runģʽ
			time_display();				
			if(ds1302_alarm_enable==1)//����������������ʹ�ܱ�־
			{
				sprintf(time_buffer,"*");
				OLED_ShowString(110,0,(uint8_t*)time_buffer,16);	
			}
			else
			{
				sprintf(time_buffer," ");
				OLED_ShowString(110,0,(uint8_t*)time_buffer,16);
			}
			
			if(key==KEY0_PRES||remote_key==PLAY)   //�л�������ʱ��ģʽ
			{
				time_status=time_set;
				time_getnum=1;
			}		
			break;
			
		case time_set:  //����ģʽ
			Time_set(key,remote_key);
		  time_set_display();
			if(key==KEY0_PRES||remote_key==PLAY)//�л�дģʽ
			{
				time_status=time_load;
			}
			break;
			
		case time_load: //дģʽ
	    time_write();
			time_status=time_run;  //д���Զ��л�runģʽ
			break;
		
		default:
			break;
	}
}




//����������
enum ds1302_alarm_state{ds1302_alarm_set,ds1302_alarm_write,ds1302_alarm_display};
void ds1302_alarm(uint8_t key,uint8_t remote_key)
{
	static enum ds1302_alarm_state ds1302_alarm_status=ds1302_alarm_display;
	char time_buffer[24]={0};
//	OLED_ShowString(44,0,"alarm",16);
	OLED_Show_Font(44,0,5);      //��
	OLED_Show_Font(44+24,0,6);   //��
	if(time_get_flag==1)
		Get_Time(&time);   //ˢ������ʱ��
	
  switch(ds1302_alarm_status)
	{
		case ds1302_alarm_display: //������ʾģʽ
			sprintf(time_buffer,"%02d:%02d:%02d",ds1302_alarm_hour,ds1302_alarm_minute,ds1302_alarm_second);
			OLED_ShowString(32,16,(uint8_t*)time_buffer,16);	
			if(key==KEY2_PRES||remote_key==EQ)  //ʹ������
			{
				alarm_enable=!alarm_enable;
				ds1302_Write(DS1302_ENABLE,0);
				ds1302_Write(DS1302_ALARM_ENABLE,alarm_enable);
				ds1302_Write(DS1302_ENABLE,0x80);
			}
			if(key==KEY0_PRES||remote_key==PLAY)   //�л�����������ģʽ
			{
				ds1302_alarm_status=ds1302_alarm_set;
				alarm_getnum=1;
			}
			if(ds1302_alarm_enable==1) //���������Ƿ�����־
			{
				sprintf(time_buffer,"*");
				OLED_ShowString(100,0,(uint8_t*)time_buffer,16);	
			}
			else
			{
				sprintf(time_buffer," ");
				OLED_ShowString(100,0,(uint8_t*)time_buffer,16);
			}
			break;
			
		case ds1302_alarm_set:   //��������ģʽ
			DS1302_alarm_set(key,remote_key);
		  DS1302_alarm_set_display();
		  if(key==KEY0_PRES||remote_key==PLAY)   //�л�дģʽ
			  ds1302_alarm_status=ds1302_alarm_write;
		  break;
			
		case ds1302_alarm_write:  //����дģʽ
			ds1302_Write(DS1302_ENABLE,0);
	    ds1302_Write(DS1302_ALARM_HOUR,((ds1302_alarm_set_hour/10)<<4|ds1302_alarm_set_hour%10));
			ds1302_Write(DS1302_ALARM_MIN,((ds1302_alarm_set_min/10)<<4|ds1302_alarm_set_min%10));
			ds1302_Write(DS1302_ALARM_SEC,((ds1302_alarm_set_sec/10)<<4|ds1302_alarm_set_sec%10));
		  ds1302_Write(DS1302_ENABLE,0x80);
		  ds1302_alarm_status=ds1302_alarm_display; //д����Զ��л���ʾģʽ
		  break;
		
		default:
			break;
	}
}


//д�����ʱ��
uint16_t ds1302_Read(uint16_t read_cmd)
{
	uint8_t i=0;
	uint16_t read_dat=0;
	DS1302_RST=0;
	DS1302_SCLK=0;
	DS1302_RST=1;
	DS1302_SDA_OUT();
	delay_us(2);
	for(i=0;i<8;i++)
	{
		DS1302_DATA_OUT=read_cmd&0x01; //�����λ��ʼд
		delay_us(2);
		DS1302_SCLK=1;
		delay_us(2);
		DS1302_SCLK=0;
		read_cmd>>=1;
	}
	DS1302_SDA_IN();
	delay_us(2);
	for(i=0;i<8;i++)
	{
		read_dat>>=1;   
		if(DS1302_DATA_IN==1) 
			read_dat|=0x80;    
		DS1302_SCLK=1;
		delay_us(2);
		DS1302_SCLK=0;
		delay_us(2);
	}
	DS1302_RST=0;
 // DS1302_DATA_OUT=0;
	delay_us(10);
	return read_dat;
}
//д���дʱ��
void ds1302_Write(uint16_t write_cmd,uint16_t write_dat)
{
	uint8_t i=0;
	DS1302_RST=0;
	DS1302_SCLK=0;
	DS1302_RST=1;
	DS1302_SDA_OUT();
	delay_us(2);
	for(i=0;i<8;i++)
	{
		DS1302_DATA_OUT=write_cmd&0x01;
		delay_us(2);
		DS1302_SCLK=1;
		delay_us(2);
		DS1302_SCLK=0;
		write_cmd>>=1;
	}
	for(i=0;i<8;i++)
	{
		DS1302_DATA_OUT=write_dat&0x01;
		delay_us(2);
		DS1302_SCLK=1;
		delay_us(2);
		DS1302_SCLK=0;
		write_dat>>=1;
	}
	DS1302_RST=0;
	delay_us(4);
}

//��ȡ���ʱ��������
void Get_Time(TIME* time)  
{  
	uint8_t temp=0; 
  temp=ds1302_Read(DS1302_SECOND|1);
	time->second=(temp&0x0f)+(temp>>4)*10;  
	
	temp=ds1302_Read(DS1302_MINUTE|1); 
	time->minute=(temp&0x0f)+(temp>>4)*10;  
	
	temp=ds1302_Read(DS1302_HOUR|1);  
  time->hour=(temp&0x0f)+(temp>>4)*10; 
		
	temp=ds1302_Read(DS1302_DATE|1);  
  time->date=(temp&0x0f)+(temp>>4)*10; 
		
	temp=ds1302_Read(DS1302_MONTH|1);  
	time->month=(temp&0x0f)+(temp>>4)*10;  
	
	temp=ds1302_Read(DS1302_WEEK|1);  
	time->week=(temp&0x0f)+(temp>>4)*10;  
		
	temp=ds1302_Read(DS1302_YEAR|1);  
	time->year=(temp&0x0f)+(temp>>4)*10;  
	
	//��������ram��
	temp=ds1302_Read(DS1302_ALARM_HOUR|1);
	ds1302_alarm_hour=(temp&0x0f)+(temp>>4)*10; 
	temp=ds1302_Read(DS1302_ALARM_MIN|1);
	ds1302_alarm_minute=(temp&0x0f)+(temp>>4)*10;
	temp=ds1302_Read(DS1302_ALARM_SEC|1);
	ds1302_alarm_second=(temp&0x0f)+(temp>>4)*10;
  ds1302_alarm_enable=ds1302_Read(DS1302_ALARM_ENABLE|1);
}  

//100ms���жϣ���ȡʱ���־����˸��־ȡ��
void time_get(void)
{
	static uint32_t count_100ms=0;
	count_100ms++;
	time_get_flag=!time_get_flag;
	if(count_100ms%2==0)
		time_twinkle=!time_twinkle;
}

//runģʽ��ʾ
void time_display(void)  
{	   
	char time_buffer[24]={0};  	
	sprintf(time_buffer,"20%02d-%02d-%02d",time.year,time.month,time.date); //��ʾ������
	OLED_ShowString(24,16,(uint8_t*)time_buffer,16);
	sprintf(time_buffer,"%02d:%02d:%02d",time.hour,time.minute,time.second); //��ʾʱ����
	OLED_ShowString(32,0,(uint8_t*)time_buffer,16);	
	sprintf(time_buffer,"%d",time.week);  //��ʾ��
	OLED_ShowString(16+32,32,(uint8_t*)time_buffer,16);	
}  

//����ʱ��
void Time_set(uint8_t key,uint8_t remote_key)
{	
	if(time_getnum==1) //��ȡ��ǰʱ��
	{
	  time_hour=time.hour;
  	time_minute=time.minute;
  	time_second=time.second;
  	time_year=time.year;
  	time_month=time.month;
  	time_date=time.date;
  	time_week=time.week;
		time_getnum=0;
	}
	if(key==KEY1_PRES||remote_key==CH_ADD)//�л�
	{
		time_k++;
		if(time_k==8)
			time_k=1;
	}
	if(remote_key==CH_CUT)//�л�
	{
		time_k--;
		if(time_k==0)
			time_k=7;
	}
	if(key==KEY2_PRES||remote_key==VOL_ADD)
	{
		if(time_k==1)  //ʱ+
			{
				time_hour++;
				if(time_hour==24)
					time_hour=0;
			}
		if(time_k==2) //��+
			{
				time_minute++;
				if(time_minute==60)
					time_minute=0;
			}
		if(time_k==3)  //��+
			{
				time_second++;
				if(time_second==60)
					time_second=0;
			}
		if(time_k==4)  //��+
			{
				time_year++;
				if(time_year==100)
					time_year=0;
			}
		if(time_k==5)  //��+ 
			{
				time_month++;
				if(time_month==13)
					time_month=1;
			}
		if(time_k==6)  //��+
			{
				time_date++;
				if(time_date==32)
					time_date=1;
			}
		if(time_k==7)  //��+
			{
				time_week++;
				if(time_week==8)
					time_week=1;
			}
	}
	if(remote_key==VOL_CUT)
	{
		if(time_k==1)  //ʱ-
			{
				if(time_hour==0)
					time_hour=24;
				time_hour--;
			}
		if(time_k==2) //��-
			{
				if(time_minute==0)
					time_minute=60;
				time_minute--;
			}
		if(time_k==3)  //��-
			{
				if(time_second==0)
					time_second=60;				
				time_second--;
			}
		if(time_k==4)  //��-
			{
				if(time_year==0)
					time_year=100;				
				time_year--;
			}
		if(time_k==5)  //��- 
			{
				if(time_month==1)
					time_month=13;				
				time_month--;
			}
		if(time_k==6)  //��-
			{
				if(time_date==1)
					time_date=32;
				time_date--;
			}
		if(time_k==7)  //��-
			{
				if(time_week==1)
					time_week=8;
				time_week--;				
			}
	}
	
}

//������ʾ
void time_set_display(void)  
{	   
	char time_buffer[24]={0};  
  if(time_twinkle==0)	
	{
		sprintf(time_buffer,"20%02d-%02d-%02d",time_year,time_month,time_date);
		OLED_ShowString(24,16,(uint8_t*)time_buffer,16);
		sprintf(time_buffer,"%02d:%02d:%02d",time_hour,time_minute,time_second);
		OLED_ShowString(32,0,(uint8_t*)time_buffer,16);	
		sprintf(time_buffer,"%d",time_week);
		OLED_ShowString(16+32,32,(uint8_t*)time_buffer,16);	
	}		
	else
	{
		if(time_k==1) //ʱ
		{
			sprintf(time_buffer,"  :%02d:%02d",time_minute,time_second);
			OLED_ShowString(32,0,(uint8_t*)time_buffer,16);	
		}
		if(time_k==2) //��
		{
			sprintf(time_buffer,"%02d:  :%02d",time_hour,time_second);
			OLED_ShowString(32,0,(uint8_t*)time_buffer,16);	
		}
		if(time_k==3) //��
		{
			sprintf(time_buffer,"%02d:%02d:  ",time_hour,time_minute);
			OLED_ShowString(32,0,(uint8_t*)time_buffer,16);	
		}
		if(time_k==4) //��
		{
			sprintf(time_buffer,"    -%02d-%02d",time_month,time_date);
			OLED_ShowString(24,16,(uint8_t*)time_buffer,16);
		}
		if(time_k==5) //��
		{
			sprintf(time_buffer,"20%02d-  -%02d",time_year,time_date);
			OLED_ShowString(24,16,(uint8_t*)time_buffer,16);
		}
		if(time_k==6) //��
		{
			sprintf(time_buffer,"20%02d-%02d-  ",time_year,time_month);
			OLED_ShowString(24,16,(uint8_t*)time_buffer,16);
		}
		if(time_k==7) //��
		{
			sprintf(time_buffer," ");
			OLED_ShowString(16+32,32,(uint8_t*)time_buffer,16);	
		}
	}
}  

//�����õ�ʱ��д���Ĵ���
void time_write(void)
{
	ds1302_Write(DS1302_ENABLE,0);  
	ds1302_Write(DS1302_SECOND,((time_second/10)<<4|time_second%10));  		
	ds1302_Write(DS1302_MINUTE,((time_minute/10)<<4|time_minute%10));  
	ds1302_Write(DS1302_HOUR,((time_hour/10)<<4|time_hour%10));  
	ds1302_Write(DS1302_DATE,((time_date/10)<<4|time_date%10));  
	ds1302_Write(DS1302_MONTH,((time_month/10)<<4|time_month%10));  
	ds1302_Write(DS1302_WEEK,((time_week/10)<<4|time_week%10));  
	ds1302_Write(DS1302_YEAR,((time_year/10)<<4|time_year%10));
	ds1302_Write(DS1302_ENABLE,0x80);
}


//����
void ds1302_alarm_ring(void)
{
	if(ds1302_alarm_ring_flag==1)
	{
		ds1302_alarm_ring_time--;
		HAL_GPIO_TogglePin(BEEP_GPIO_Port,BEEP_Pin);
		if(ds1302_alarm_ring_time==0) //��ʱ�� �����
		{
			HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
			ds1302_alarm_ring_flag=0;
		}
	}
}	

//��������
void DS1302_alarm_set(uint8_t key,uint8_t remote_key)
{
	if(key==KEY1_PRES||remote_key==CH_ADD)
	{
		ds1302_alarm_k++;
		if(ds1302_alarm_k==3)
			ds1302_alarm_k=0;
	}
	if(remote_key==CH_CUT)
	{
		if(ds1302_alarm_k==0)
			ds1302_alarm_k=3;
		ds1302_alarm_k--;
	}
	if(alarm_getnum==1)
	{
		ds1302_alarm_set_hour=ds1302_alarm_hour;
		ds1302_alarm_set_min=ds1302_alarm_minute;
		ds1302_alarm_set_sec=ds1302_alarm_second;
		alarm_getnum=0;
	}
	if(key==KEY2_PRES||remote_key==VOL_ADD)
	{
		if(ds1302_alarm_k==0)
		{
			ds1302_alarm_set_hour++;
			if(ds1302_alarm_set_hour==24)
				ds1302_alarm_set_hour=0;
		}
		if(ds1302_alarm_k==1)
		{
			ds1302_alarm_set_min++;
			if(ds1302_alarm_set_min==60)
				ds1302_alarm_set_min=0;
		}
		if(ds1302_alarm_k==2)
		{
			ds1302_alarm_set_sec++;
			if(ds1302_alarm_set_sec==60)
				ds1302_alarm_set_sec=0;
		}
	}
	if(remote_key==VOL_CUT)
	{
		if(ds1302_alarm_k==0)
		{
			if(ds1302_alarm_set_hour==0)
				ds1302_alarm_set_hour=24;
			ds1302_alarm_set_hour--;			
		}
		if(ds1302_alarm_k==1)
		{			
			if(ds1302_alarm_set_min==0)
				ds1302_alarm_set_min=60;
			ds1302_alarm_set_min--;
		}
		if(ds1302_alarm_k==2)
		{			
			if(ds1302_alarm_set_sec==0)
				ds1302_alarm_set_sec=60;
			ds1302_alarm_set_sec--;
		}
	}
}

//����������ʾ
void DS1302_alarm_set_display(void)
{
	char time_buffer[24]={0};  
  if(time_twinkle==0)	
	{
		sprintf(time_buffer,"%02d:%02d:%02d",ds1302_alarm_set_hour,ds1302_alarm_set_min,ds1302_alarm_set_sec);
		OLED_ShowString(32,16,(uint8_t*)time_buffer,16);	
	}	
  else
	{
		if(ds1302_alarm_k==0)
		{
			sprintf(time_buffer,"  :%02d:%02d",ds1302_alarm_set_min,ds1302_alarm_set_sec);
		  OLED_ShowString(32,16,(uint8_t*)time_buffer,16);	
		}
		if(ds1302_alarm_k==1)
		{
			sprintf(time_buffer,"%02d:  :%02d",ds1302_alarm_set_hour,ds1302_alarm_set_sec);
		  OLED_ShowString(32,16,(uint8_t*)time_buffer,16);	
		}
		if(ds1302_alarm_k==2)
		{
			sprintf(time_buffer,"%02d:%02d:  ",ds1302_alarm_set_hour,ds1302_alarm_set_min);
		  OLED_ShowString(32,16,(uint8_t*)time_buffer,16);	
		}
	}		
}
	

void alarm_ring(uint8_t key,uint8_t remote_key)
{
	if(ds1302_alarm_enable==1)      //�ж��Ƿ������趨ʱ��
		if((time.hour==ds1302_alarm_hour)&&(time.minute==ds1302_alarm_minute)&&(time.second==ds1302_alarm_second))
		{
			ds1302_alarm_ring_flag=1;     //ʹ������
			ds1302_alarm_ring_time=1800;   //����ʱ������3min
		}
	if(key==KEY2_PRES||remote_key==CH)//ֹͣ����
		{
			ds1302_alarm_ring_flag=0;
			HAL_GPIO_WritePin(BEEP_GPIO_Port,BEEP_Pin,GPIO_PIN_RESET);
		}
}

