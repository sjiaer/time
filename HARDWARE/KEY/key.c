#include "key.h"


enum key_state{key_initial,key_confirm,key_oncepress,key_longpress};
uint8_t key_return=0;

void KEY_Scan_stase(void)
{
	static enum key_state key_s=key_initial;
  static uint8_t key_count=0;
  static uint8_t key_count1=0;
  static uint8_t key_count2=0;	
	
//	key_return=0;
	
	switch(key_s)
	{
		case key_initial:   //按键初始状态
			if(KEY0==0||KEY1==0||KEY2==0||WK_UP==1)
				{
					key_s=key_confirm;	
				}
			break;
			
		case key_confirm:   //按键确认状态
			if(KEY0==0||KEY1==0||KEY2==0||WK_UP==1)
			{
				key_count++;
				if(key_count==4)
					{
						key_count=0;
						key_s=key_oncepress;
						if(KEY0==0)
							key_return=1;
						else if(KEY1==0)
							key_return=2;
						else if(KEY2==0)
							key_return=3;
						else if(WK_UP==1)
							key_return=4;
					}
			}
			else
			{
				key_count=0;
				key_s=key_initial;
			}
			break;
		
		case key_oncepress:  //一次按键
			if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)
				{
					key_s=key_initial;
					key_count1=0;
				}
			else 
				{
					key_count1++;
					if(key_count1>200)
					{
						key_s=key_longpress;
						key_count1=0;
					}
				} 
			break;
			
		case key_longpress:    //长按
			if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)
				{
					key_s=key_initial;
					key_count2=0;
				}
			else
				{
					key_count2++;
					if(key_count2==40)  //200ms发送一次按键
					{
						key_count2=0;
						if(KEY0==0)
								key_return=1;
							else if(KEY1==0)
								key_return=2;
							else if(KEY2==0)
								key_return=3;
							else if(WK_UP==1)
								key_return=4;
					}
				}		
			break;
	}
}
	



uint8_t key_read(void)
{
	static uint8_t keyword; 
	keyword = key_return;
	key_return = 0;
	return keyword;
}





