#include "Nixie_tube.h"

uint8_t Nixie_tube_count=0;

void Nixie_tube_delay(void)
{
	static uint16_t two_point_count;
	two_point_count++;
	Nixie_tube_count++;  //不同的值代表不同的数码管
	if(Nixie_tube_count==4)
		Nixie_tube_count=0;
	if(two_point_count==500)
	{
		two_point_count=0;
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);  //1s冒号闪烁一次，代替秒
	}
}

//7个段赋值
void GPIO_write7(GPIO_TypeDef* GPIOx, uint8_t PortVal)
{
	GPIOx->ODR&=~0xFE;
  GPIOx->ODR |= PortVal;
}

//4个数码管赋值
void GPIO_write4(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
	GPIOx->ODR&=~0xC06;
  GPIOx->ODR |= PortVal;
}

void Nixie_tube_display(uint8_t data0,uint8_t data1)
{
	unsigned char LED_display[10]={0x08,0x3b,0x24,0x21,0x13,0x41,0x40,0x2b,0x00,0x01};
	if(Nixie_tube_count==0)  //第一个数码管 显示时的十位
	{
	GPIO_write4(GPIOB,0x04);
	GPIO_write7(GPIOA,LED_display[data0/10]<<1);
	}
	if(Nixie_tube_count==1)  //第二个数码管 显示时的个位
	{
	GPIO_write4(GPIOB,0x400);
	GPIO_write7(GPIOA,LED_display[data0%10]<<1);
	}
	if(Nixie_tube_count==2)  //第三个数码管 显示分的十位
	{
	GPIO_write4(GPIOB,0x800);
	GPIO_write7(GPIOA,LED_display[data1/10]<<1);
	}
	if(Nixie_tube_count==3)  //第四个数码管 显示分的个位
	{
	GPIO_write4(GPIOB,0x02);
	GPIO_write7(GPIOA,LED_display[data1%10]<<1);
	}
}
