#include "delay.h"

void delay_us(uint32_t nus)
{	
  static uint16_t i=0;	
	for(;nus>0;nus--)   	 
	{
		for(i=5;i>0;i--);
	}
}




