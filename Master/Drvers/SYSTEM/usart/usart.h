#ifndef __USART_H
#define __USART_H

#include "SYSTEM/system/system.h"
#include "stdio.h"

#define USART1_REC_LEN		200 
extern u8  USART1_RX_BUF[USART1_REC_LEN];
extern u16 USART1_RX_STA; 
void Usart_Init(u32 bound);
#endif

