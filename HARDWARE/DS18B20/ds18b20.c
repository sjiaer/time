#include "ds18b20.h"
#include "oled.h"
#include "stdio.h" 
#include "delay.h"
#include "text.h"

uint8_t temp_collect_flag;
short temperature;

//复位DS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT(); //SET OUTPUT
  DS18B20_DQ_OUT=0; //拉低DQ
  delay_us(750);    //拉低750us
  DS18B20_DQ_OUT=1; //DQ=1 
	delay_us(15);     //15US
}
//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
uint8_t DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
	DS18B20_IO_IN();//SET INPUT	 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
//从DS18B20读取一个位
//返回值：1/0
uint8_t DS18B20_Read_Bit(void) 			 // read one bit
{
  uint8_t data;
	DS18B20_IO_OUT();//SET  OUTPUT
  DS18B20_DQ_OUT=0; 
	delay_us(2);
  DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();//SET  INPUT
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
  else data=0;	 
  delay_us(50);           
  return data;
}
//从DS18B20读取一个字节
//返回值：读到的数据
uint8_t DS18B20_Read_Byte(void)    // read one byte
{        
    uint8_t i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}
//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(uint8_t dat)     
 {             
    uint8_t j;
    uint8_t testb;
	  DS18B20_IO_OUT();//SET OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;// Write 1
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2);                          
        }
    }
}
//开始温度转换
void DS18B20_Start(void)// ds1820 start convert
{   						               
    DS18B20_Rst();	   
	  DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
} 
//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
uint8_t DS18B20_Init(void)
{
 	DS18B20_Rst();
	return DS18B20_Check();
}  
//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
short DS18B20_Get_Temp(void)
{
	uint8_t temp;
	uint8_t TL,TH;
	short tem;
	DS18B20_Start ();                    // ds1820 start convert
	DS18B20_Rst();
	DS18B20_Check();	 
	DS18B20_Write_Byte(0xcc);// skip rom
	DS18B20_Write_Byte(0xbe);// convert	    
	TL=DS18B20_Read_Byte(); // LSB   
	TH=DS18B20_Read_Byte(); // MSB   
	if(TH>7)
	{
		TH=~TH;
		TL=~TL; 
		temp=0;//温度为负  
	}
	else 
		temp=1;//温度为正	 
	
	tem=TH; //获得高八位
	tem<<=8;    
	tem+=TL;//获得底八位
	tem=(double)tem*0.625;//转换     
	if(temp)
		return tem; //返回温度值
	else 
		return -tem;    
}



void Temp_collect_flag(void)
{
	temp_collect_flag=!temp_collect_flag;
}

void temp_meature(void)
{
	
  char temp_buff[24]={0};  
 	if(DS18B20_Init())	//DS18B20初始化	
	{
		OLED_ShowString(0,45,"Error",16);
		return;
	}  
	if(temp_collect_flag==1)
	  temperature=DS18B20_Get_Temp();	
//	OLED_Show_Font(0,48,12);  //温
//	OLED_Show_Font(16,48,13); //度
	if(temperature<0)
	{
		temperature=-temperature;					//转为正数
		sprintf(temp_buff,"-%02d.%dC  ",temperature/10,temperature%10);
		OLED_ShowString(64,32,(uint8_t*)temp_buff,16); 
	}
	else 	 
	{
		sprintf(temp_buff," %02d.%dC  ",temperature/10,temperature%10);
		OLED_ShowString(64,32,(uint8_t*)temp_buff,16);
	}				

}











