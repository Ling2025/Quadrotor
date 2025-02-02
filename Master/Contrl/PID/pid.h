#ifndef __PID_H
#define __PID_H

#include "SYSTEM/system/system.h"

typedef struct PID
{
    float Err;
    float prev_Err;
    float P;
    float I;
    float D;
    float Integral;
    float Differ;

    float Pout;
    float Iout;
    float Dout;
    float Out;
    float Ilimit;
    uint8_t Ilimit_flag;
    float Irang;

}PID_TYPE;



void Reset_PID(PID_TYPE **resetPID,uint8_t len);
void PID_Control(PID_TYPE * PID,float target,float measure,float dt);

#endif
