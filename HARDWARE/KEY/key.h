#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 

/*下面方式是通过位带操作方式读取IO*/
#define KEY0 		PAin(9)   	//PA9
#define KEY1 		PAin(10)		//PA10 
#define KEY2 		PAin(15)		//PA15
#define WK_UP 	PAin(0)		  //PA0



#define KEY0_PRES 	1
#define KEY1_PRES	  2
#define KEY2_PRES	  3
#define WKUP_PRES   4



void KEY_Scan_stase(void);
uint8_t key_read(void);

#endif
