#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"


#define AirQuality_AddrH 		 		  0x00
#define AirQuality_AddrL 		 		  0x11

#define IndoorHumidity_AddrH   		0x01
#define IndoorHumidity_AddrL   		0x07

#define OutdoorTemperature_AddrH   0x01
#define OutdoorTemperature_AddrL   0x0B

#define IndoorTemperature_AddrH    0x01
#define IndoorTemperature_AddrL    0x0F


u8 good_str_send[10]={0xA5, 0x5A, 0x07, 0x82, AirQuality_AddrH, AirQuality_AddrL, 'G', 'o', 'o', 'd'};
u8 bad_str_send[9]= {0xA5, 0x5A, 0x06, 0x82, AirQuality_AddrH, AirQuality_AddrL, 'B', 'a', 'd'};

u8 IndoorHumidity_send[7]=  {0xA5, 0x5A, 0x04, 0x82, IndoorHumidity_AddrH, IndoorHumidity_AddrL, 0x00};
u8 OutdoorTemperature_send[7]= {0xA5, 0x5A, 0x04, 0x82, OutdoorTemperature_AddrH, OutdoorTemperature_AddrL, 0x00};
u8 IndoorTemperature_send[7]= {0xA5, 0x5A, 0x04, 0x82, IndoorTemperature_AddrH, IndoorTemperature_AddrH, 0x00};


u8 Air_quality()
{
	u8 Airquality;
	//Add your code here
	return Airquality;
}
u8 Indoor_humidity()
{
	u8 Inhumidity;
	//Add your code here
	return Inhumidity;
}
u8 Outdoor_temperature()
{
	u8 Outtemp;
	//Add your code here
	return Outtemp;
}
u8 Indoor_temperature()
{
	u8 Intemp;
	//Add your code here
	return Intemp;
}

void UART1_Send_Array(u8 send_array[],unsigned char num)
{
        u8 i=0;  
        while(i<num)
        {
                USART_SendData(USART1,send_array[i]);  
                while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
                i++;  
        }
}

 int main(void)
 {		
	u16 times=0;
	u16 pwm_duty=0;
	delay_init();	    	 //delay init 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //NVIC INIT
	uart_init(115200);	 //UART INIT
 	LED_Init();			     //Light Init
//	KEY_Init();          
	TIM3_PWM_Init(899,0);	 //80Khz
	 TIM_SetCompare2(TIM3,900);	
 	while(1)
	{
		if(USART_RX_END)
		{					   
			switch (USART_RX_BUF[5])
			{
				case LivingRoom:
					Light_Contral(LivingRoom,USART_RX_BUF[8]);
					break;
				case Restuarant:
					Light_Contral(Restuarant,USART_RX_BUF[8]);
					break;
				case BathRomm:
					Light_Contral(BathRomm,USART_RX_BUF[8]);
					break;
				case ChildrenRoom:		
						pwm_duty=900-USART_RX_BUF[8]*9;
				if(pwm_duty>900)pwm_duty=900;
						ChildrenRoom_Light_Pwm(pwm_duty);		 
					break;
				default:
					USART_RX_END=0;
					USART_RX_STA=0;
					break;
			}
			USART_RX_STA=0;
			USART_RX_END=0;
		}
		else
		{
			times++;
			if(times>=500)
			{
				IndoorHumidity_send[6]=Indoor_humidity();
				UART1_Send_Array(IndoorHumidity_send,7);
			
				OutdoorTemperature_send[6]=Outdoor_temperature();
				UART1_Send_Array(OutdoorTemperature_send,7);
				
				IndoorTemperature_send[6]=Indoor_temperature();
				UART1_Send_Array(IndoorTemperature_send,7);
				
				if(Air_quality()>=20)
				{
					UART1_Send_Array(good_str_send,10);
				}
				else
				{
					UART1_Send_Array(bad_str_send,9);
				}
				times=0;
			}
			delay_ms(10);   
		}
	}	 
 }

