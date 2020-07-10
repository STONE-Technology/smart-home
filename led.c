#include "led.h"
#include "timer.h"

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //ENABLE CLOCK
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;				 //-->PB config
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 // PP OUT 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //GPIO SPEED 50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 
 GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);						 //config out hight

 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 //GPIO_Init(GPIOE, &GPIO_InitStructure);			
 //GPIO_SetBits(GPIOE,GPIO_Pin_5); 	 
}
 
void Light_Contral(u8 LightSelect,u8 Onflag)
{
	u16 gpio_select;
		if(LightSelect==LivingRoom)
			gpio_select=GPIO_Pin_0;
		else if(LightSelect==Restuarant)
			gpio_select=GPIO_Pin_1;
		else if(LightSelect==BathRomm)
			gpio_select=GPIO_Pin_2;
		
		if(Onflag==LightOn)
			GPIO_SetBits(GPIOB,gpio_select);
		else
			GPIO_ResetBits(GPIOB,gpio_select);
}

void ChildrenRoom_Light_Pwm(u16 pwm_duty)
{
		TIM_SetCompare2(TIM3,pwm_duty);		
}
