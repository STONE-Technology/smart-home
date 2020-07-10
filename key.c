#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
    
//KEY CONFIGRATION
void KEY_Init(void) 
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOE, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
//按键处理函数 Button process function
//返回按键值 return button value
//mode:0,不支持连续按 No continuously pressing;1,支持连续按 support continuously pressing;
//0，没有任何按键按下 No button pressed
//1，KEY0按下 KEY0 pressed
//2，KEY1按下 KEY1 pressed
//3，KEY2按下 KEY2 pressed
//4，KEY3按下 WK_UP KEY3 pressed
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!! Note this function has response priority
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志 Key released 
	if(mode)key_up=1;  //支持连按	support continuously pressing	  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(10);//去抖动 Debounce
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1; 	    
 	return 0;// 无按键按下 No key pressed 
}
