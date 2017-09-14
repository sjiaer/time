#ifndef _DS1302_H  
#define _DS1302_H  
  
#include "sys.h"  

#define DS1302_SDA_IN()             {GPIOB->CRH&=0xff0fffff;GPIOB->CRH|=(uint32_t)0x8<<20;}    
#define DS1302_SDA_OUT()            {GPIOB->CRH&=0xff0fffff;GPIOB->CRH|=(uint32_t)0x3<<20;}   
   
#define DS1302_RST                   PBout(12)          
#define DS1302_SCLK                  PBout(14)         
#define DS1302_DATA_OUT              PBout(13)         
#define DS1302_DATA_IN               PBin(13)    
  
#define DS1302_SECOND                0x80  
#define DS1302_MINUTE                0x82  
#define DS1302_HOUR                  0x84  
#define DS1302_DATE                  0x86  
#define DS1302_MONTH                 0x88  
#define DS1302_WEEK                  0x8A  
#define DS1302_YEAR                  0x8C  
#define DS1302_ENABLE                0x8E 
#define DS1302_ALARM_HOUR            0xC0
#define DS1302_ALARM_MIN             0xC2
#define DS1302_ALARM_SEC             0xC4
#define DS1302_ALARM_ENABLE          0xC6

  
typedef struct  
{  
    uint8_t second;  
    uint8_t minute;  
    uint8_t hour;  
    uint8_t date;  
    uint8_t month;  
    uint8_t week;  
    uint8_t year;  
}TIME;


void DS1302_Init(void); 
uint16_t ds1302_Read(uint16_t read_cmd);
void ds1302_Write(uint16_t write_cmd,uint16_t write_dat);
void Get_Time(TIME* time);  
void time_display(void);
void time_get(void); 
void time_ds1302(uint8_t key,uint8_t remote_key);
void Time_set(uint8_t key,uint8_t remote_key);
void time_set_display(void);
void time_write(void);
void ds1302_alarm(uint8_t key,uint8_t remote_key);
void ds1302_alarm_ring(void);
void DS1302_alarm_set(uint8_t key,uint8_t remote_key);
void DS1302_alarm_set_display(void);
void alarm_ring(uint8_t key,uint8_t remote_key);

#endif  
