#ifndef __RED_H
#define __RED_H 
#include  "sys.h"    

#define RDATA PAin(8)	 //�������������

//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0 

#define ERROR     0		   
#define CH_CUT    162 	    
#define CH        98	    
#define RIGHT     2		 
#define CH_ADD    226  		  
#define PLAY      194   	   
#define LEFT  		34    
#define VOL_CUT  	224    
#define VOL_ADD  	168     
#define EQ  	    144     
//#define ONE  		  104
//#define TWO  	    152
//#define THREE  	  176  
//#define FOUR      48 		    
//#define FIVE      24  		    
//#define SIX       122   		  
//#define SEVEN     16   			   					
//#define EIGHT     56   	 
//#define NINE      90   
//#define ZERO      66    
//#define DELETE  	82  	

uint8_t Remote_Scan(void);	 
void remote_tim1_update(void);
void remote_tim1_ic(void);
#endif

