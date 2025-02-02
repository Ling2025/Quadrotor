#include "BSP/EXIT/exit.h"
#include "SYSTEM/usart/usart.h"
#include "BSP/SI24R/si24r.h"
#include "SYSTEM/systick/systick.h"
#include "REMOTE/remotedata.h"

uint8_t rxbuf[32]={0};
uint8_t rx_flag=0;

void EXIT_GPIOC8_Init(void)
{

    GPIO_InitTypeDef        GPIO_Initstruct;
    EXTI_InitTypeDef        EXTI_Initstruct;
    NVIC_InitTypeDef        NVIC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    GPIO_Initstruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Initstruct.GPIO_Pin=GPIO_Pin_8;
    GPIO_Init(GPIOC,&GPIO_Initstruct);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);

    EXTI_Initstruct.EXTI_Line=EXTI_Line8;
    EXTI_Initstruct.EXTI_LineCmd=ENABLE;
    EXTI_Initstruct.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_Initstruct.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_Initstruct);

    NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=6;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&NVIC_InitStruct);

}


void EXTI9_5_IRQHandler(void)
{
    u8 i=0;
    if(EXTI_GetITStatus(EXTI_Line8))
    {
      //  printf("entry\r\n");
            state=Si24r_Read_reg(Si24r_Read+SI24R_STATUS);

        if(state&TX_DS)
        {
            Si24rset_Mode(IT_RX);
            Si24r_Write_reg(Si24r_Write+SI24R_STATUS,TX_DS);
            Si24r_Write_reg(FLUSH_TX,0xff);
           // printf("send ok\r\n");
        }
        else if(state & RX_DR)
        {

            SI24R1_RxPacket(rxbuf);
            Si24r_Write_reg(Si24r_Write+SI24R_STATUS,RX_DR);
            rx_flag=1;
            Connection_State.Connect_count=0;

        }
        else if(state & MAX_RT)
        {
            Si24rset_Mode(IT_RX);
			//	Si24r_RX_Mode();
            Si24r_Write_reg(Si24r_Write+SI24R_STATUS,MAX_RT);
            Si24r_Write_reg(FLUSH_TX,0xff);
            //printf("MAX TX\r\n");

        }


    }
    EXTI_ClearITPendingBit(EXTI_Line8);
     
}
