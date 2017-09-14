#ifndef __NIXIE_TUBE_H 
#define __NIXIE_TUBE_H
#include "sys.h"

void Nixie_tube_display(uint8_t data0,uint8_t data1);
void Nixie_tube_delay(void);
void GPIO_write7(GPIO_TypeDef* GPIOx, uint8_t PortVal);
void GPIO_write4(GPIO_TypeDef* GPIOx, uint16_t PortVal);
	
#endif


