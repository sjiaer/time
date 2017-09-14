#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   



#define DS18B20_IO_IN()  {GPIOB->CRH&=0x0fffffff;GPIOB->CRH|=(uint32_t)0x8<<28;}	//PB15输入模式
#define DS18B20_IO_OUT() {GPIOB->CRH&=0x0fffffff;GPIOB->CRH|=(uint32_t)0x3<<28;} 	//PB15输出模式

//IO操作函数											   
#define	DS18B20_DQ_OUT PBout(15) //数据端口	PB15
#define	DS18B20_DQ_IN  PBin(15)  //数据端口	PB15 
   	
uint8_t DS18B20_Init(void);			//初始化DS18B20
short DS18B20_Get_Temp(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(uint8_t dat);//写入一个字节
uint8_t DS18B20_Read_Byte(void);		//读出一个字节
uint8_t DS18B20_Read_Bit(void);		//读出一个位
uint8_t DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20    
void temp_meature(void);
void Temp_collect_flag(void);

#endif















