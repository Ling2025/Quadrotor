#ifndef __LED_H
#define __LED_H

#include "SYSTEM/system/system.h"


#define LED1_PORT 			GPIOB   
#define LED1_PIN 			GPIO_Pin_5
#define LED1_PORT_RCC		RCC_APB2Periph_GPIOB


void LED_Init(void);

#define LED	PBout(5)


#endif

