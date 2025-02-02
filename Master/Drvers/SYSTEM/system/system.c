#include "SYSTEM/system/system.h"


void System_Systick_Init(void)
{

	RCC->CR=0;
	
	RCC->CR |=1<<16;
	while(!(RCC->CR & 1<<17));

	RCC->CFGR=0;
	RCC->CFGR|=(7<<18)|(1<<16)|(2<<14)|(4<<8)|(2<<0);
	RCC->CR |=1<<24;

	while(!((RCC->CFGR &0x0C)==0x08));


}

