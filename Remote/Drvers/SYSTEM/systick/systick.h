#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "SYSTEM/system/system.h"
void SysTick_Init(u16 SYSCLK);
void delay_us(u32 us);
void delay_ms(u32 ms);



#endif
