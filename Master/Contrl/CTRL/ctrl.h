#ifndef __CTRL_H
#define __CTRL_H

#include "SYSTEM/system/system.h"
#include "Contrl/PID/pid.h"
#include "imu.h"
#include "BSP/MPU-6050/mpu6050.h"
#include "REMOTE/remotedata.h"
#include "BSP/MOTOR/motor.h"
#include "SYSTEM/usart/usart.h"

extern PID_TYPE PID_ROL_Angal;
extern PID_TYPE PID_PIT_Angal;
extern PID_TYPE PID_YAW_Angal;
extern PID_TYPE PID_ROL_Rate;
extern PID_TYPE PID_PIT_Rate;
extern PID_TYPE PID_YAW_Rate;
extern PID_TYPE *ReSet[6];

void Fly_Control(struct Attitude_Angle_t *Attitude_Angle,struct MPU6050_Original_Data_t *MPU6050_Data,struct Remote_Data_t *Remote,uint8_t lock);

void PID_Data_Init(void);

#endif

