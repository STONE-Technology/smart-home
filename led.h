#ifndef __LED_H
#define __LED_H	 
#include "sys.h"


#define LivingRoom   0x09
#define Restuarant   0x0a
#define BathRomm 		 0x0b
#define ChildrenRoom 0x0f
#define LightOn      0x00
#define LightOff     0x01

void LED_Init(void);
void Light_Contral(u8 LightSelect,u8 Onflag);
void ChildrenRoom_Light_Pwm(u16 pwm_duty);		 				    
#endif
