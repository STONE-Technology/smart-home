#include "timer.h"
#include "led.h"
#include "usart.h"

   	  
//通用定时器3中断初始化 Timer3 interrupt initialization
//这里时钟选择为APB1的2倍，而APB1为36M Clock uses 2 times APB1, APB1 is 36M
//arr：自动重装值。 arr: auto reload value
//psc：时钟预分频数 clock prescale
//这里使用的是定时器3! Timer3 used here
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能 Enable Timer3

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms Set auto reload register period value for next event, count 5000 as 500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  Set prescaler as TimerX clock frequency divisor
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim Set Clock devision
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式 Timer counter mode is Up
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 Initialize time base unit of TimerX, according to TIM_TimeBaseInitStruct
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断 Enable TIM3 interrupt, allow update interrupt

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断 TIM3 interrupt
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级 Priority = 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级 SubPriority = 3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能 Enable IRQ channel
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 Initialize NVIC register according to NVIC_InitStruct

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设 Enable TIM3 
							 
}
//定时器3中断服务程序 Timer3 interrupt service
void TIM3_IRQHandler(void)   //TIM3中断 TIM3 interrupt
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 check TIM interrupt happens or not: TIM interrupt source
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源  Clear TIM pending interrupt: TIM interrupt source
		//Add your code here
		}
}


void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//clock
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3 PartialRemap TIM3_CH2->PB5    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 
	TIM_Cmd(TIM3, ENABLE);
}



