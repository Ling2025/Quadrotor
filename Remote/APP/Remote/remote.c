#include "Remote/remote.h"
#include "BSP/ADC/adc.h"
#include "SYSTEM/usart/usart.h"
#include "BSP/SI24R/si24r.h"
#include "BSP/EXIT/exit.h"

struct Remote_Data_t My_Remote_Data;
struct Filter_Window_t Filter_Window_THR,Filter_Window_YAW,Filter_Window_PIT,Filter_Window_ROL;
struct Remote_Offset_t Remote_Offset;
uint8_t offset_flag=0;


void Remote_Data(void)
{

    /*中点校准*/

    /*滑动窗口滤波*/
    Remote_Window_Filter();
    /*限制最大最小*/
    Max_Min_Limitations();
    /*中点限幅*/
    Remote_Middle_Limit();
    /*发送遥控数据*/
    

}


void Remote_ADC_Data(void)
{

    My_Remote_Data.THR=(uint16_t)(0.2441f*ADC_Buf[0])+1000+Remote_Offset.THR;
    My_Remote_Data.YAW=(uint16_t)(0.2441f*ADC_Buf[1])+1000+Remote_Offset.YAW;
    My_Remote_Data.PIT=(uint16_t)(0.2441f*ADC_Buf[2])+1000+Remote_Offset.PIT;
    My_Remote_Data.ROL=(uint16_t)(0.2441f*ADC_Buf[3])+1000+Remote_Offset.ROL;

}

void Remote_Window_Filter(void)
{

    static uint8_t count=0;
    Remote_ADC_Data();

    Filter_Window_THR.sum-=Filter_Window_THR.old[count];
    Filter_Window_THR.sum+=My_Remote_Data.THR;
    Filter_Window_THR.old[count]=My_Remote_Data.THR;
    My_Remote_Data.THR= Filter_Window_THR.sum/Filter_NUM;

    Filter_Window_YAW.sum-=Filter_Window_YAW.old[count];
    Filter_Window_YAW.sum+=My_Remote_Data.YAW;
    Filter_Window_YAW.old[count]=My_Remote_Data.YAW;
    My_Remote_Data.YAW= Filter_Window_YAW.sum/Filter_NUM;


    Filter_Window_PIT.sum-=Filter_Window_PIT.old[count];
    Filter_Window_PIT.sum+=My_Remote_Data.PIT;
    Filter_Window_PIT.old[count]=My_Remote_Data.PIT;
    My_Remote_Data.PIT= Filter_Window_PIT.sum/Filter_NUM;

    Filter_Window_ROL.sum-=Filter_Window_ROL.old[count];
    Filter_Window_ROL.sum+=My_Remote_Data.ROL;
    Filter_Window_ROL.old[count]=My_Remote_Data.ROL;
    My_Remote_Data.ROL= Filter_Window_ROL.sum/Filter_NUM;
 
    count++;
    if(count>=10)
        count=0;

}

void Remote_Offset_Data(void)
{

    uint32_t sum[4]={0};
    uint8_t i=0;
    for(i=0; i<100; i++)
    {
        Remote_ADC_Data();
        if(i>49)
        {
            sum[0]+=My_Remote_Data.THR;
            sum[1]+=My_Remote_Data.YAW;
            sum[2]+=My_Remote_Data.PIT;
            sum[3]+=My_Remote_Data.ROL;
        }

    }
    Remote_Offset.THR=(int16_t)(1500.0f-sum[0]/50.0f);
    Remote_Offset.YAW=(int16_t)(1500.0f-sum[1]/50.0f);
    Remote_Offset.PIT=(int16_t)(1500.0f-sum[2]/50.0f);
    Remote_Offset.ROL=(int16_t)(1500.0f-sum[3]/50.0f);


}

void Max_Min_Limitations(void)
{
    My_Remote_Data.THR=My_Remote_Data.THR>2000?2000:My_Remote_Data.THR;
    My_Remote_Data.YAW=My_Remote_Data.YAW>2000?2000:My_Remote_Data.YAW;
    My_Remote_Data.PIT=My_Remote_Data.PIT>2000?2000:My_Remote_Data.PIT;
    My_Remote_Data.ROL=My_Remote_Data.ROL>2000?2000:My_Remote_Data.ROL;

    My_Remote_Data.THR=My_Remote_Data.THR<1000?1000:My_Remote_Data.THR;
    My_Remote_Data.YAW=My_Remote_Data.YAW<1000?1000:My_Remote_Data.YAW;
    My_Remote_Data.PIT=My_Remote_Data.PIT<1000?1000:My_Remote_Data.PIT;
    My_Remote_Data.ROL=My_Remote_Data.ROL<1000?1000:My_Remote_Data.ROL;

}

void Remote_Middle_Limit(void)
{
    if(My_Remote_Data.THR>1490&&My_Remote_Data.THR<1510)
        My_Remote_Data.THR=1500;

    if(My_Remote_Data.YAW>1490&&My_Remote_Data.YAW<1510)
        My_Remote_Data.YAW=1500;
    if(My_Remote_Data.PIT>1490&&My_Remote_Data.PIT<1510)
        My_Remote_Data.PIT=1500;
    if(My_Remote_Data.ROL>1490&&My_Remote_Data.ROL<1510)
        My_Remote_Data.ROL=1500;
    
}

void Remote_Send_Data(void)
{
    uint8_t send_buffer[32]={0};
    uint32_t data_sum=0;
    uint8_t i=0;


    send_buffer[0]=0xAA;
    send_buffer[1]=0xAF;

    send_buffer[2]=0x03;
    send_buffer[3]=20;//数据长度

    send_buffer[4]=(uint8_t)(My_Remote_Data.THR>>8);
    send_buffer[5]=(uint8_t)My_Remote_Data.THR;
    send_buffer[6]=(uint8_t)(My_Remote_Data.YAW>>8);
    send_buffer[7]=(uint8_t)My_Remote_Data.YAW;
    send_buffer[8]=(uint8_t)(My_Remote_Data.ROL>>8);
    send_buffer[9]=(uint8_t)My_Remote_Data.ROL;
    send_buffer[10]=(uint8_t)(My_Remote_Data.PIT>>8);
    send_buffer[11]=(uint8_t)My_Remote_Data.PIT;

    send_buffer[12]=(uint8_t)(My_Remote_Data.AUX1>>8);
    send_buffer[13]=(uint8_t)My_Remote_Data.AUX1;
    send_buffer[14]=(uint8_t)(My_Remote_Data.AUX2>>8);
    send_buffer[15]=(uint8_t)My_Remote_Data.AUX2;
    send_buffer[16]=(uint8_t)(My_Remote_Data.AUX3>>8);
    send_buffer[17]=(uint8_t)My_Remote_Data.AUX3;
    send_buffer[18]=(uint8_t)(My_Remote_Data.AUX4>>8);
    send_buffer[19]=(uint8_t)My_Remote_Data.AUX4;
    send_buffer[20]=(uint8_t)(My_Remote_Data.AUX5>>8);
    send_buffer[21]=(uint8_t)My_Remote_Data.AUX5;
    send_buffer[22]=(uint8_t)(My_Remote_Data.AUX6>>8);
    send_buffer[23]=(uint8_t)My_Remote_Data.AUX6;

    for(i=4;i<24;i++)
    {
        data_sum+=send_buffer[i];
    }
    send_buffer[24]=(uint8_t)(data_sum>>24);
    send_buffer[25]=(uint8_t)(data_sum>>16);
    send_buffer[26]=(uint8_t)(data_sum>>8);
    send_buffer[27]=(uint8_t)data_sum;



    SI24R1_TxPacket(send_buffer);

}

void Decode_Data(float * Att_Buf)
{

    Att_Buf[0]=((float)((int16_t)(rxbuf[0]<<8|rxbuf[1])))/100.0;
    Att_Buf[1]=((float)((int16_t)(rxbuf[2]<<8|rxbuf[3])))/100.0;
    Att_Buf[2]=((float)((int16_t)(rxbuf[4]<<8|rxbuf[5])))/100.0;

}
