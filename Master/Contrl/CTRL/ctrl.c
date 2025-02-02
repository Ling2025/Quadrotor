#include "Contrl/CTRL/ctrl.h"

PID_TYPE PID_ROL_Angal;
PID_TYPE PID_PIT_Angal;
PID_TYPE PID_YAW_Angal;
PID_TYPE PID_ROL_Rate;
PID_TYPE PID_PIT_Rate;
PID_TYPE PID_YAW_Rate;
PID_TYPE *ReSet[6]={&PID_ROL_Angal,&PID_PIT_Angal,&PID_YAW_Angal,&PID_ROL_Rate,&PID_PIT_Rate,&PID_YAW_Rate};

uint16_t motor1=0,motor2=0,motor3=0,motor4=0;

uint16_t motor_range(uint16_t min, uint16_t max, uint16_t motor);

void Fly_Control(struct Attitude_Angle_t *Attitude_Angle,struct MPU6050_Original_Data_t *MPU6050_Data,struct Remote_Data_t *Remote,uint8_t lock)
{
    struct Attitude_Angle_t Measure_Angle,Target_Angle;
    struct MPU6050_Original_Data_t angle_rate;
    int16_t thr=0;

    Measure_Angle.Roll=Attitude_Angle->Roll;
    Measure_Angle.Pitch=Attitude_Angle->Pitch;
    Measure_Angle.Yaw=Attitude_Angle->Yaw;

    Target_Angle.Pitch=(Remote->PIT-1500.0f)/12.0f;
    Target_Angle.Roll=(Remote->ROL-1500.0f)/12.0f;
    Target_Angle.Yaw=(Remote->YAW-1500.0f)/12.0f;

    angle_rate.gyr_x=MPU6050_Data->gyr_x*RadtoDeg;
    angle_rate.gyr_y=MPU6050_Data->gyr_y*RadtoDeg;
    angle_rate.gyr_z=MPU6050_Data->gyr_z*RadtoDeg;

    thr=Remote->THR-1000;
    

    PID_Control(&PID_ROL_Angal,Target_Angle.Roll,Measure_Angle.Roll,0.01);
    PID_Control(&PID_PIT_Angal,Target_Angle.Pitch,Measure_Angle.Pitch,0.01);
    PID_Control(&PID_YAW_Angal,Target_Angle.Yaw,Measure_Angle.Yaw,0.01);

    PID_Control(&PID_ROL_Rate,PID_ROL_Angal.Out,angle_rate.gyr_x,0.01);
    PID_Control(&PID_PIT_Rate,PID_PIT_Angal.Out,angle_rate.gyr_y,0.01);
    PID_Control(&PID_YAW_Rate,PID_YAW_Angal.Out,angle_rate.gyr_z,0.01);
    //PID_Control(&PID_YAW_Rate,Target_Angle.Yaw*PID_YAW_Angal.P,angle_rate.gyr_z,0.01);

    if(lock_flag==UNLOCK&&thr>180)
    {

       motor1=thr+PID_ROL_Rate.Out-PID_PIT_Rate.Out-PID_YAW_Rate.Out;
       motor2=thr+PID_ROL_Rate.Out+PID_PIT_Rate.Out+PID_YAW_Rate.Out;
       motor3=thr-PID_ROL_Rate.Out+PID_PIT_Rate.Out-PID_YAW_Rate.Out;
       motor4=thr-PID_ROL_Rate.Out-PID_PIT_Rate.Out+PID_YAW_Rate.Out;
        motor1=motor_range(0,1000,motor1);
        motor2=motor_range(0,1000,motor2);
        motor3=motor_range(0,1000,motor3);
        motor4=motor_range(0,1000,motor4);

    }
    else
    {
        Reset_PID(ReSet,6);
        motor1=0;
        motor2=0;
        motor3=0;
        motor4=0;
    }

    Motor_Pwm(motor1,motor3,motor2,motor4);

    printf("motor1:%d\r\n",motor1);
    printf("motor2:%d\r\n",motor2);
    printf("motor3:%d\r\n",motor3);
    printf("motor4:%d\r\n",motor4);



}

void PID_Data_Init(void)
{
    PID_ROL_Angal.P=-2.5f;
    PID_ROL_Angal.I=0.0f;
    PID_ROL_Angal.D=0.0f;

    PID_PIT_Angal.P=2.5f;
    PID_PIT_Angal.I=0.0f;
    PID_PIT_Angal.D=0.0f;

    PID_YAW_Angal.P=3.0f;
    PID_YAW_Angal.I=0.0f;
    PID_YAW_Angal.D=0.0f;

    PID_ROL_Rate.P=-2.0f;
    PID_ROL_Rate.I=0.0f;
    PID_ROL_Rate.D=-0.1f;

    PID_PIT_Rate.P=2.0f;
    PID_PIT_Rate.I=0.0f;
    PID_PIT_Rate.D=0.1f;

    PID_YAW_Rate.P=-2.0f;
    PID_YAW_Rate.I=0.0f;
    PID_YAW_Rate.D=0.0f;

    PID_ROL_Angal.Ilimit_flag=0;
    PID_ROL_Angal.Ilimit=100.0f;
    PID_ROL_Angal.Irang=100.0f;
    PID_ROL_Rate.Ilimit_flag=0;
    PID_ROL_Rate.Ilimit=100.0f;
    PID_ROL_Rate.Irang=100.0f;

    PID_PIT_Angal.Ilimit_flag=0;
    PID_PIT_Angal.Ilimit=100.0f;
    PID_PIT_Angal.Irang=100.0f;
    PID_PIT_Rate.Ilimit_flag=0;
    PID_PIT_Rate.Ilimit=100.0f;
    PID_PIT_Rate.Irang=100.0f;

    PID_YAW_Angal.Ilimit_flag=0;
    PID_YAW_Angal.Ilimit=100.0f;
    PID_YAW_Angal.Irang=100.0f;
    PID_YAW_Rate.Ilimit_flag=0;
    PID_YAW_Rate.Ilimit=100.0f;
    PID_YAW_Rate.Irang=100.0f;






    
}

uint16_t motor_range(uint16_t min, uint16_t max, uint16_t motor)
{
    if(motor>max)
        motor=max;
    if(motor<min)
        motor=min;
    return motor;
}
