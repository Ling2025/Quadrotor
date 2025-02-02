#include "Contrl/PID/pid.h"
#include "REMOTE/remotedata.h"

void PID_Control(PID_TYPE * PID,float target,float measure,float dt)
{
    /*误差*/
    PID->Err=target-measure;
    PID->Differ=(PID->Err-PID->prev_Err)/dt;

    /*比例*/
    PID->Pout=PID->P*PID->Err;
    /*积分*/
    PID->Iout=PID->Ilimit_flag*PID->I*PID->Integral;
    /*微分*/
    PID->Dout=PID->D*PID->Differ;

    /*输出*/
    PID->Out=PID->Pout+PID->Iout+PID->Dout;

    if(lock_flag==UNLOCK)
    {
        if((measure>PID->Ilimit)||(measure<-PID->Ilimit))
            PID->Ilimit_flag=0;
        else
        {
            PID->Ilimit_flag=1;
            PID->Integral+=PID->Err*dt;
            if(PID->Integral>PID->Irang)
                PID->Integral=PID->Irang;
            else if(PID->Integral<-PID->Irang)
                PID->Integral=-PID->Irang;
        }

    }
    else
    {
        PID->Ilimit_flag=0;
    }
    PID->prev_Err=PID->Err;

}

void Reset_PID(PID_TYPE **resetPID,uint8_t len)
{
    uint8_t i;
    for(i=0;i<len;i++)
    {
        resetPID[i]->Differ=0;
        resetPID[i]->Err=0;
        resetPID[i]->Ilimit_flag=0;
        resetPID[i]->Integral=0;
        resetPID[i]->prev_Err=0;

    }
    
}

