#include "timer.h"
#include "stop_watch.h"
#include "cutdown.h"
#include "key.h"
#include "ds18b20.h"
#include "ds1302.h" 
#include "Nixie_tube.h"
#include "remote.h"
#include "clock.h"
 
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim1;
uint32_t count_ms;
//uint8_t key;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim3)  //定时器3 1ms进中断
	{
		count_ms++;
		if(count_ms%50==0)
		{
		  stop_watch_running();
		}
		if(count_ms%50==0)
		{
			cutdown_running();
		}
		if(count_ms%5==0)
		{
			KEY_Scan_stase();
		}
//		key=key_read();
		if(count_ms%100==0)
		{
			Temp_collect_flag();
		}
		if(count_ms%100==0)
		{
			time_get();
			ds1302_alarm_ring();
		}
		Nixie_tube_delay();
		ds1302_Nixie_tube_display();
	}
	if(htim == &htim1)
	{
		remote_tim1_update();
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim1)
	{
		remote_tim1_ic();
	}
}
