#include "BSP/TIME/time4.h"
#include "BSP/SI24R/si24r.h"
#include "com_ano.h"
#include "imu.h"


void Time4_Init(uint16_t per, uint16_t psc)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitstruct;
    NVIC_InitTypeDef        NVIC_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

    TIM_TimeBaseInitstruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitstruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitstruct.TIM_Period=per;
    TIM_TimeBaseInitstruct.TIM_Prescaler=psc;

    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitstruct);



    NVIC_InitStruct.NVIC_IRQChannel=TIM4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=7;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStruct);
		
		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM4,ENABLE);


}


void TIM4_IRQHandler(void)
{

    if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
    {
        
        Send_Attitude();
    }
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

