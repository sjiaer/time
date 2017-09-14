#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   



#define DS18B20_IO_IN()  {GPIOB->CRH&=0x0fffffff;GPIOB->CRH|=(uint32_t)0x8<<28;}	//PB15����ģʽ
#define DS18B20_IO_OUT() {GPIOB->CRH&=0x0fffffff;GPIOB->CRH|=(uint32_t)0x3<<28;} 	//PB15���ģʽ

//IO��������											   
#define	DS18B20_DQ_OUT PBout(15) //���ݶ˿�	PB15
#define	DS18B20_DQ_IN  PBin(15)  //���ݶ˿�	PB15 
   	
uint8_t DS18B20_Init(void);			//��ʼ��DS18B20
short DS18B20_Get_Temp(void);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(uint8_t dat);//д��һ���ֽ�
uint8_t DS18B20_Read_Byte(void);		//����һ���ֽ�
uint8_t DS18B20_Read_Bit(void);		//����һ��λ
uint8_t DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20    
void temp_meature(void);
void Temp_collect_flag(void);

#endif















