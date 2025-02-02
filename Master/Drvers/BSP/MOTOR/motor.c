#include "BSP/MOTOR/motor.h"
#define MOTOR_PWM_MAX 1000

/*
    电机初始化
    重装载值为1000
    预分频系数为3
*/
void TIM3_PWM_Init(void)
{

    GPIO_InitTypeDef        GPIO_Initstruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitstruct;
    TIM_OCInitTypeDef       TIM_OCInitstruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    GPIO_Initstruct.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Initstruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
    GPIO_Initstruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_Initstruct);

    GPIO_Initstruct.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Initstruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
    GPIO_Initstruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_Initstruct);

    TIM_TimeBaseInitstruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitstruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitstruct.TIM_Period=1000-1;
    TIM_TimeBaseInitstruct.TIM_Prescaler=3-1;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitstruct);

    TIM_OCInitstruct.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitstruct.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCInitstruct.TIM_Pulse=0;
    TIM_OCInitstruct.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OC1Init(TIM3,&TIM_OCInitstruct);
    TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);

    TIM_OCInitstruct.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitstruct.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCInitstruct.TIM_Pulse=0;
    TIM_OCInitstruct.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OC2Init(TIM3,&TIM_OCInitstruct);
    TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);

    TIM_OCInitstruct.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitstruct.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCInitstruct.TIM_Pulse=0;
    TIM_OCInitstruct.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OC3Init(TIM3,&TIM_OCInitstruct);
    TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);

    TIM_OCInitstruct.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitstruct.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCInitstruct.TIM_Pulse=0;
    TIM_OCInitstruct.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OC4Init(TIM3,&TIM_OCInitstruct);
    TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);

   TIM_Cmd(TIM3,ENABLE);


}

void Motor_Pwm(int16_t MOTOR1_PWM,int16_t MOTOR2_PWM,int16_t MOTOR3_PWM,int16_t MOTOR4_PWM)
{

    if(MOTOR1_PWM>MOTOR_PWM_MAX)MOTOR1_PWM=MOTOR_PWM_MAX;
    if(MOTOR2_PWM>MOTOR_PWM_MAX)MOTOR2_PWM=MOTOR_PWM_MAX;
    if(MOTOR3_PWM>MOTOR_PWM_MAX)MOTOR3_PWM=MOTOR_PWM_MAX;
    if(MOTOR4_PWM>MOTOR_PWM_MAX)MOTOR4_PWM=MOTOR_PWM_MAX;

    if(MOTOR1_PWM<0)MOTOR1_PWM=0;
    if(MOTOR2_PWM<0)MOTOR2_PWM=0;
    if(MOTOR3_PWM<0)MOTOR3_PWM=0;
    if(MOTOR4_PWM<0)MOTOR4_PWM=0;

    TIM3->CCR1=MOTOR1_PWM;
    TIM3->CCR2=MOTOR2_PWM;
    TIM3->CCR3=MOTOR3_PWM;
    TIM3->CCR4=MOTOR4_PWM;
}

