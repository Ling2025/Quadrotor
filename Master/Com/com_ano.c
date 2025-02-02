#include "com_ano.h"
#include "imu.h"
#include "SYSTEM/usart/usart.h"
#include "BSP/SI24R/si24r.h"

void Com_ANO_Data(struct Attitude_Angle_t Attitude_Angle)
{

    int8_t AnoDAta[16]={0};
    int16_t ROLL,PIT,YAW;
    int32_t ALT_USE;
    uint8_t FLY_MODL;
    uint8_t ARMED;
    u8 i=0;
    ROLL=(int16_t)Attitude_Angle.Roll*100.0f;
    PIT=(int16_t)Attitude_Angle.Pitch*100.0f;
    YAW=(int16_t)Attitude_Angle.Yaw*100.0f;
    ALT_USE=0;
    FLY_MODL=0;
    ARMED=1;

    AnoDAta[0]=0xAA;
    AnoDAta[1]=0xAA;
    AnoDAta[2]=0x01;
    AnoDAta[3]=0x12;
    AnoDAta[4]=ROLL>>8;
    AnoDAta[5]=ROLL<<8;
    AnoDAta[6]=PIT>>8;
    AnoDAta[7]=PIT<<8;
    AnoDAta[8]=YAW>>8;
    AnoDAta[9]=YAW<<8;
    AnoDAta[10]=ALT_USE>>24;
    AnoDAta[11]=(ALT_USE&0xFF0000)>>16;
    AnoDAta[12]=(ALT_USE&0xFF00)>>8;
    AnoDAta[13]=(ALT_USE&0xFF);
    AnoDAta[14]=FLY_MODL;
    AnoDAta[15]=ARMED;

    for(i=0;i<16;i++)
    {
         printf("%c",AnoDAta[i]);

    }
   

    

}


void Send_Attitude(void)
{
    int16_t ROLL,PIT,YAW;
    int8_t Data[32]={0};

    ROLL=(int16_t)MY_Attitude_Angle.Roll*100.0f;
    PIT=(int16_t)MY_Attitude_Angle.Pitch*100.0f;
    YAW=(int16_t)MY_Attitude_Angle.Yaw*100.0f;

    Data[0]=(int8_t)(ROLL>>8);
    Data[1]=(int8_t)ROLL;
    Data[2]=(int8_t)(PIT>>8);
    Data[3]=(int8_t)PIT;
    Data[4]=(int8_t)(YAW>>8);
    Data[5]=(int8_t)YAW;

    SI24R1_TxPacket((uint8_t *)Data);

}

