#include "stm32f10x.h"
#include "SYSTEM/system/system.h"
#include "BSP/LED/led.h"
#include "SYSTEM/systick/systick.h"
#include "FreeRTOS.h"
#include "task.h"
#include "freertos_demo.h"
#include "BSP/SI24R/si24r.h"
#include "SYSTEM/usart/usart.h"

extern unsigned char txbuf[32];

int main(void)
{

	System_Systick_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	SysTick_Init(72);

	HardWare_Init();

	freertos_demo();
	
	
//	while(1)
//	{
//		SI24R1_TxPacket(txbuf);
//		delay_ms(200);
//	
//	}


}



