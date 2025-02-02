#include "imu.h"
#include "BSP/MPU-6050/mpu6050.h"
#include "math.h"

struct Attitude_Angle_t MY_Attitude_Angle;

#define Acc_Gain    G*0.0001220f//加速度变成G
#define Gyro_Gain 	0.0609756f//角速度变成度
#define Gyro_Gr	    0.0010641f//角速度变成弧度

#define halfT       0.005f
#define Kp          1.5f
#define Ki          0.005f


float exInt=0,eyInt=0,ezInt=0;
static float q0=1,q1=0,q2=0,q3=0;
float   DCMgb[3][3];
static float invSqrt(float x) 
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f375a86 - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}


void Prepare_Data(void)
{

    MPU6050_Filetering_Data();
    //MPU6050_Original_Offset_Data();
    MPU6050_Original_Data.acc_x=(float)MPU6050_Original_Data.acc_orig_x*Acc_Gain;
    MPU6050_Original_Data.acc_y=(float)MPU6050_Original_Data.acc_orig_y*Acc_Gain;
    MPU6050_Original_Data.acc_z=(float)MPU6050_Original_Data.acc_orig_z*Acc_Gain;

    MPU6050_Original_Data.gyr_x=(float)MPU6050_Original_Data.gyr_orig_x*Gyro_Gr;
    MPU6050_Original_Data.gyr_y=(float)MPU6050_Original_Data.gyr_orig_y*Gyro_Gr;
    MPU6050_Original_Data.gyr_z=(float)MPU6050_Original_Data.gyr_orig_z*Gyro_Gr;

}

void Get_Attitude_Angle(struct Attitude_Angle_t *Attitude_Angle)
{

    float Rotation_Matrix[9]={0.0f};
    float ax=MPU6050_Original_Data.acc_x, ay=MPU6050_Original_Data.acc_y,az=MPU6050_Original_Data.acc_z;
    float gx=MPU6050_Original_Data.gyr_x, gy=MPU6050_Original_Data.gyr_y,gz=MPU6050_Original_Data.gyr_z;
    float vx,vy,vz;
    float ex,ey,ez;
    float Normalization;

    float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q0q3 = q0*q3;
	float q1q1 = q1*q1;
	float q1q2 = q1*q2;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;
    u8 i=0;

    Prepare_Data();

    Normalization=invSqrt(ax*ax+ay*ay+az*az);
    ax=ax*Normalization;
    ay=ay*Normalization;
    az=az*Normalization;
    if(ax*ay*az==0)
        return;
    
    vx=2*(q1q3-q0q2);
    vy=2*(q2q3+q0q1);
    vz=q0q0-q1q1-q2q2+q3q3;

    ex=ay*vz-az*vy;
    ey=az*vx-ax*vz;
    ez=ax*vy-ay*vx;

    exInt=exInt+ex*Ki;
    eyInt=eyInt+ey*Ki;
    ezInt=ezInt+ez*Ki;

    gx=gx+Kp*ex+exInt;
    gy=gy+Kp*ey+eyInt;
    gz=gz+Kp*ez+ezInt;

    q0=q0+halfT*(-gx*q1-gy*q2-gz*q3);
    q1=q1+halfT*(gx*q0-gy*q3+gz*q2);
    q2=q2+halfT*(gx*q3+gy*q0-gz*q1);
    q3=q3+halfT*(gy*q1+gz*q0-gx*q2);

    Normalization=invSqrt(q0*q0+q1*q1+q2*q2+q3*q3);
    q0 = q0 * Normalization;
	q1 = q1 * Normalization;
	q2 = q2 * Normalization;  
	q3 = q3 * Normalization;

    Rotation_Matrix[0]=q0q0+q1q1+q2q2+q3q3;//11:一行一列
    Rotation_Matrix[1]=2.0f*(q1q2-q0q3);//12
    Rotation_Matrix[2]=2.0f*(q1q3-q0q2);//13
    Rotation_Matrix[3]=2.0f*(q1q2+q0q3);//21
    Rotation_Matrix[4]=q0q0+q2q2-q1q1-q3q3;//22
    Rotation_Matrix[5]=2.0f*(q2q3-q0q1);//23
    Rotation_Matrix[6]=2.0f*(q1q3-q0q2);//31
    Rotation_Matrix[7]=2.0f*(q2q3+q0q1);//32
    Rotation_Matrix[8]=q0q0-q1q1-q2q2+q3q3;//33

    //Attitude_Angle->Yaw=atan2(2.0f*(q1*q2+q0*q3),q0*q0+q1*q1-q2*q2-q3*q3)*57.3f;
    Attitude_Angle->Yaw+=MPU6050_Original_Data.gyr_z*RadtoDeg*0.01f;
    Attitude_Angle->Pitch=-asin(2.0f*(q1*q3-q0*q2))*RadtoDeg;
    Attitude_Angle->Roll=-atan2(2.0f*(q2*q3+q0*q1),q0*q0-q1*q1-q2*q2+q3*q3)*RadtoDeg;
    //=-asin
    //-atan2
    
    for(i=0;i<9;i++)
    {
        *(&(DCMgb[0][0])+i)=Rotation_Matrix[i];
    }


}
