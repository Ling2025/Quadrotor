#include "BSP/MPU-6050/mpu6050.h"
#include "SYSTEM/usart/usart.h"
#include "SYSTEM/systick/systick.h"
#include "filtering.h"

struct MPU6050_Original_Data_t MPU6050_Original_Data;
struct MPU6050_offset_t MPU6050_offset_struct;
u8 MPU6050_ReadData(u8 reg, u8 *data)
{
    u8 ret;
    ret=MPU_IIC_ReadByteFrommSlave(MPU6050_ADDR,reg,data);
    return ret;
}

u8 MPU6050_ReadData_Length(u8 reg, u8 *buf,u8 length)
{
    u8 ret;
    ret=MPU_IIC_ReadLengthData(MPU6050_ADDR,reg,buf,length);
    return ret;
}

u8 MPU6050_WriteData(u8 reg,u8 data)
{
    u8 ret;
    ret=MPU_IIC_WriteByteToSlave(MPU6050_ADDR,reg,data);
    return ret;

}

void Get_MPU6050_ID(u8 *ID)
{
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,ID);
}

u8 MPU6050_Check_MPU6050_ID(void)
{
    u8 ID;
    Get_MPU6050_ID(&ID);
    if(ID!=0x68)
        return 1;
return 0;

}

void MPU6050_Init(void)
{

    IIC_MPU_Init();
    while(MPU6050_Check_MPU6050_ID())
    {
		printf("mpu6050 error\r\n");
	}
	printf("mpu6050 normal\r\n");

    MPU6050_WriteData(MPU6050_RA_PWR_MGMT_1,0x80);//复位
    delay_ms(100);
	MPU6050_WriteData(MPU6050_RA_PWR_MGMT_1,0x01);//唤醒
	MPU6050_WriteData(MPU6050_RA_INT_ENABLE,0x00);//关闭中断
	MPU6050_WriteData(GYRO_CONFIG,0x18);//陀螺仪输出的满量程范围是正负2000
	MPU6050_WriteData(MPU6050_RA_ACCEL_CONFIG,0x08);//加速度计满量程范围是正负4g
	MPU6050_WriteData(CONFIG,0x04);//陀螺仪频率为1kHz,DLPF为20Hz,则采样率为1kHz
	MPU6050_WriteData(SMPLRT_DIV,0x00);//不分频
	MPU6050_WriteData(MPU6050_RA_INT_PIN_CFG,0x02);
	MPU6050_WriteData(FIFO_EN,0x00);//不使用FiFO

	Takeoff_Attitude_Check();
	delay_ms(50);
	MPU6050_Offset();


	printf("mpu6050 Init Finish\r\n");
}

void Read_MPU6050_Original_Data(void)
{
    u8 data[14];

	MPU6050_ReadData(MPU6050_RA_ACCEL_XOUT_H,&data[0]);
	MPU6050_ReadData(MPU6050_RA_ACCEL_XOUT_L,&data[1]);
	MPU6050_ReadData(MPU6050_RA_ACCEL_YOUT_H,&data[2]);
	MPU6050_ReadData(MPU6050_RA_ACCEL_YOUT_L,&data[3]);
	MPU6050_ReadData(MPU6050_RA_ACCEL_ZOUT_H,&data[4]);
	MPU6050_ReadData(MPU6050_RA_ACCEL_ZOUT_L,&data[5]);
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,&data[6]);
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_L,&data[7]);
	
	MPU6050_ReadData(MPU6050_RA_GYRO_XOUT_H,&data[8]);
	MPU6050_ReadData(MPU6050_RA_GYRO_XOUT_L,&data[9]);
	MPU6050_ReadData(MPU6050_RA_GYRO_YOUT_H,&data[10]);
	MPU6050_ReadData(MPU6050_RA_GYRO_YOUT_L,&data[11]);
	MPU6050_ReadData(MPU6050_RA_GYRO_ZOUT_H,&data[12]);
	MPU6050_ReadData(MPU6050_RA_GYRO_ZOUT_L,&data[13]);
	
	MPU6050_Original_Data.acc_orig_x=( signed short)((data[0]<<8)|data[1]);
	MPU6050_Original_Data.acc_orig_y=( signed short)((data[2]<<8)|data[3]);
	MPU6050_Original_Data.acc_orig_z=( signed short)((data[4]<<8)|data[5]);
	MPU6050_Original_Data.temp_orig=( signed short)((data[6]<<8)|data[7]);
	MPU6050_Original_Data.gyr_orig_x=( signed short)((data[8]<<8)|data[9]);
	MPU6050_Original_Data.gyr_orig_y=( signed short)((data[10]<<8)|data[11]);
	MPU6050_Original_Data.gyr_orig_z=( signed short)((data[12]<<8)|data[13]);
    MPU6050_Original_Data.tempdata=(float)36.53f+((float)MPU6050_Original_Data.temp_orig/340.0f);	
	
}

void MPU6050_Offset(void)
{

	signed int sum[6]={0};
	u16 i=0;

	for(i=0;i<200;i++)
	{
		if(i>99)
		{
			Read_MPU6050_Original_Data();
			sum[0]+=MPU6050_Original_Data.acc_orig_x;
			sum[1]+=MPU6050_Original_Data.acc_orig_y;
			sum[2]+=MPU6050_Original_Data.acc_orig_z-8192;
			sum[3]+=MPU6050_Original_Data.gyr_orig_x;
			sum[4]+=MPU6050_Original_Data.gyr_orig_y;
			sum[5]+=MPU6050_Original_Data.gyr_orig_z;
			delay_ms(10);
		}
	}

	MPU6050_offset_struct.acc_x_offset=(signed int)(sum[0]/100.0);
	MPU6050_offset_struct.acc_y_offset=(signed int)(sum[1]/100.0);
	MPU6050_offset_struct.acc_z_offset=(signed int)(sum[2]/100.0);

	MPU6050_offset_struct.gyr_x_offset=(signed int)(sum[3]/100.0);
	MPU6050_offset_struct.gyr_y_offset=(signed int)(sum[4]/100.0);
	MPU6050_offset_struct.gyr_z_offset=(signed int)(sum[5]/100.0);

}

void MPU6050_Original_Offset_Data(void)
{
	Read_MPU6050_Original_Data();

	MPU6050_Original_Data.acc_orig_x=MPU6050_Original_Data.acc_orig_x-MPU6050_offset_struct.acc_x_offset;
	MPU6050_Original_Data.acc_orig_y=MPU6050_Original_Data.acc_orig_y-MPU6050_offset_struct.acc_y_offset;
	MPU6050_Original_Data.acc_orig_z=MPU6050_Original_Data.acc_orig_z-MPU6050_offset_struct.acc_z_offset;
	MPU6050_Original_Data.gyr_orig_x=MPU6050_Original_Data.gyr_orig_x-MPU6050_offset_struct.gyr_x_offset;
	MPU6050_Original_Data.gyr_orig_y=MPU6050_Original_Data.gyr_orig_y-MPU6050_offset_struct.gyr_y_offset;
	MPU6050_Original_Data.gyr_orig_z=MPU6050_Original_Data.gyr_orig_z-MPU6050_offset_struct.gyr_z_offset;

}

void Takeoff_Attitude_Check(void)
{

	u8 gyr_i=30;/*连续30次飞行器是平稳的*/
	const int8_t Max_Gyro_Quiet=10;
	const int8_t Min_Gyro_Quiet=-10;
	int16_t Last_Gyro[3]={0};
	int16_t Err_Gyro[3]={0};

	/*初始化上一次的加速度数据*/
	Read_MPU6050_Original_Data();
	Last_Gyro[0]=MPU6050_Original_Data.gyr_orig_x;
	Last_Gyro[1]=MPU6050_Original_Data.gyr_orig_y;
	Last_Gyro[2]=MPU6050_Original_Data.gyr_orig_z;

	
	while(gyr_i--)
	{

		do
		{

			Read_MPU6050_Original_Data();
			delay_ms(20);
			Err_Gyro[0]=MPU6050_Original_Data.gyr_orig_x-Last_Gyro[0];
			Err_Gyro[1]=MPU6050_Original_Data.gyr_orig_y-Last_Gyro[1];
			Err_Gyro[2]=MPU6050_Original_Data.gyr_orig_z-Last_Gyro[2];

			Last_Gyro[0]=MPU6050_Original_Data.gyr_orig_x;
			Last_Gyro[1]=MPU6050_Original_Data.gyr_orig_y;
			Last_Gyro[2]=MPU6050_Original_Data.gyr_orig_z;

		} while ((Err_Gyro[0]<Min_Gyro_Quiet||Err_Gyro[0]>Max_Gyro_Quiet)
		||(Err_Gyro[1]<Min_Gyro_Quiet||Err_Gyro[1]>Max_Gyro_Quiet)
		||(Err_Gyro[2]<Min_Gyro_Quiet||Err_Gyro[2]>Max_Gyro_Quiet));
		


	}
	
}
/*原始数据滤波*/
void MPU6050_Filetering_Data(void)
{
	MPU6050_Original_Offset_Data();
	KaermanFilter(&Kaerman[0],MPU6050_Original_Data.acc_orig_x);
	MPU6050_Original_Data.acc_orig_x=(signed int)Kaerman[0].Xk;
	KaermanFilter(&Kaerman[1],MPU6050_Original_Data.acc_orig_y);
	MPU6050_Original_Data.acc_orig_y=(signed int)Kaerman[1].Xk;
	KaermanFilter(&Kaerman[2],MPU6050_Original_Data.acc_orig_z);
	MPU6050_Original_Data.acc_orig_z=(signed int)Kaerman[2].Xk;


	LowPassFilter(&Low_Pass[0],MPU6050_Original_Data.gyr_orig_x);
	MPU6050_Original_Data.gyr_orig_x=(signed int)Low_Pass[0].current;
	LowPassFilter(&Low_Pass[1],MPU6050_Original_Data.gyr_orig_y);
	MPU6050_Original_Data.gyr_orig_y=(signed int)Low_Pass[1].current;
	LowPassFilter(&Low_Pass[2],MPU6050_Original_Data.gyr_orig_z);
	MPU6050_Original_Data.gyr_orig_z=(signed int)Low_Pass[2].current;

}


