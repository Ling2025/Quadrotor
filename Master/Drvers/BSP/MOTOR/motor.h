#ifndef __MOTOR_H
#define __MOTOR_H

#include "SYSTEM/system/system.h"
void TIM3_PWM_Init(void);
void Motor_Pwm(int16_t MOTOR1_PWM,int16_t MOTOR2_PWM,int16_t MOTOR3_PWM,int16_t MOTOR4_PWM);

#endif

