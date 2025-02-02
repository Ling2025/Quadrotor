#ifndef __MPU6050_H
#define __MPU6050_H

#include "SYSTEM/system/system.h"
#include "BSP/IIC/iic.h"

#define MPU6050_ADDR    0xD0

/*自检寄存器*/
#define SELF_TEST_X     0X0D
#define SELF_TEST_Y     0X0E
#define SELF_TEST_Z     0X0F
#define SELF_TEST_A     0X10

/*采样频率除数，用于生成mpu6050采样频率*/
#define SMPLRT_DIV      0x19
/*外部帧同步（FSYNC）引脚采样以及陀螺仪和加速度计的数字低通滤波器（DLPF）设置。*/
#define CONFIG          0x1A
/*全量程范围*/
#define GYRO_CONFIG     0X1B

#define FIFO_EN         0x23

#define MPU6050_RA_ACCEL_CONFIG     0x1C
 

#define MPU6050_RA_INT_PIN_CFG      0x37
#define MPU6050_RA_INT_ENABLE       0x38


#define MPU6050_RA_ACCEL_XOUT_H     0x3B
#define MPU6050_RA_ACCEL_XOUT_L     0x3C
#define MPU6050_RA_ACCEL_YOUT_H     0x3D
#define MPU6050_RA_ACCEL_YOUT_L     0x3E
#define MPU6050_RA_ACCEL_ZOUT_H     0x3F
#define MPU6050_RA_ACCEL_ZOUT_L     0x40
#define MPU6050_RA_TEMP_OUT_H       0x41
#define MPU6050_RA_TEMP_OUT_L       0x42
#define MPU6050_RA_GYRO_XOUT_H      0x43
#define MPU6050_RA_GYRO_XOUT_L      0x44
#define MPU6050_RA_GYRO_YOUT_H      0x45
#define MPU6050_RA_GYRO_YOUT_L      0x46
#define MPU6050_RA_GYRO_ZOUT_H      0x47
#define MPU6050_RA_GYRO_ZOUT_L      0x48


#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_PWR_MGMT_2       0x6C
#define MPU6050_RA_WHO_AM_I         0x75



#define MPU6050_DLPF_BW_256         0x00
#define MPU6050_DLPF_BW_188         0x01
#define MPU6050_DLPF_BW_98          0x02
#define MPU6050_DLPF_BW_42          0x03
#define MPU6050_DLPF_BW_20          0x04
#define MPU6050_DLPF_BW_10          0x05
#define MPU6050_DLPF_BW_5           0x06


struct MPU6050_Original_Data_t
{

    signed int acc_orig_x;
    signed int acc_orig_y;
    signed int acc_orig_z;
    signed int temp_orig;
    signed int gyr_orig_x;
    signed int gyr_orig_y;
    signed int gyr_orig_z;
    float tempdata;

    float acc_x;
    float acc_y;
    float acc_z;
    float gyr_x;
    float gyr_y;
    float gyr_z;



};

struct MPU6050_offset_t
{
    int16_t acc_x_offset;
    int16_t acc_y_offset;
    int16_t acc_z_offset;
    int16_t gyr_x_offset;
    int16_t gyr_y_offset;
    int16_t gyr_z_offset;

};


extern struct MPU6050_Original_Data_t MPU6050_Original_Data;
extern struct MPU6050_offset_t MPU6050_offset_struct;
u8 MPU6050_Check_MPU6050_ID(void);
void Get_MPU6050_ID(u8 *ID);
u8 MPU6050_ReadData(u8 reg, u8 *data);
void Read_MPU6050_Original_Data(void);
void MPU6050_Init(void);
void MPU6050_Offset(void);
void MPU6050_Original_Offset_Data(void);
void Takeoff_Attitude_Check(void);
void MPU6050_Filetering_Data(void);
#endif
