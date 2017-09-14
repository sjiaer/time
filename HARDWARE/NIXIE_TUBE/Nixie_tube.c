#include "Nixie_tube.h"

uint8_t Nixie_tube_count=0;

void Nixie_tube_delay(void)
{
	static uint16_t two_point_count;
	two_point_count++;
	Nixie_tube_count++;  //��ͬ��ֵ����ͬ�������
	if(Nixie_tube_count==4)
		Nixie_tube_count=0;
	if(two_point_count==500)
	{
		two_point_count=0;
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);  //1sð����˸һ�Σ�������
	}
}

//7���θ�ֵ
void GPIO_write7(GPIO_TypeDef* GPIOx, uint8_t PortVal)
{
	GPIOx->ODR&=~0xFE;
  GPIOx->ODR |= PortVal;
}

//4������ܸ�ֵ
void GPIO_write4(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
	GPIOx->ODR&=~0xC06;
  GPIOx->ODR |= PortVal;
}

void Nixie_tube_display(uint8_t data0,uint8_t data1)
{
	unsigned char LED_display[10]={0x08,0x3b,0x24,0x21,0x13,0x41,0x40,0x2b,0x00,0x01};
	if(Nixie_tube_count==0)  //��һ������� ��ʾʱ��ʮλ
	{
	GPIO_write4(GPIOB,0x04);
	GPIO_write7(GPIOA,LED_display[data0/10]<<1);
	}
	if(Nixie_tube_count==1)  //�ڶ�������� ��ʾʱ�ĸ�λ
	{
	GPIO_write4(GPIOB,0x400);
	GPIO_write7(GPIOA,LED_display[data0%10]<<1);
	}
	if(Nixie_tube_count==2)  //����������� ��ʾ�ֵ�ʮλ
	{
	GPIO_write4(GPIOB,0x800);
	GPIO_write7(GPIOA,LED_display[data1/10]<<1);
	}
	if(Nixie_tube_count==3)  //���ĸ������ ��ʾ�ֵĸ�λ
	{
	GPIO_write4(GPIOB,0x02);
	GPIO_write7(GPIOA,LED_display[data1%10]<<1);
	}
}
