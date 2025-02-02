#ifndef __IMU_H
#define __IMU_H

#include "SYSTEM/system/system.h"

#define G 9.80665f
#define RadtoDeg    57.324841f  //弧度到角度 (弧度 * 180/3.1415)
#define DegtoRad    0.0174533f	//角度到弧度 (角度 * 3.1415/180)

struct Attitude_Angle_t
{
    float Pitch;
    float Roll;
    float Yaw;

};

extern struct Attitude_Angle_t MY_Attitude_Angle;

void Prepare_Data(void);
void Get_Attitude_Angle(struct Attitude_Angle_t *Attitude_Angle);

#endif
