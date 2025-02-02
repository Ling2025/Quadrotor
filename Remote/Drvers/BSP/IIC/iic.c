#include "BSP/IIC/iic.h"
#include "SYSTEM/systick/systick.h"

void IIC_MPU_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(IIC_MPU_SCL_RCC|IIC_MPU_SDA_RCC,ENABLE);

    GPIO_InitStruct.GPIO_Pin = IIC_MPU_SCL_PIN|IIC_MPU_SDA_PIN;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(IIC_MPU_SCL_PORT,&GPIO_InitStruct);

    IIC_MPU_SCL=1;
    IIC_MPU_SDA=1;

}


void MPU_IIC_Start(void)
{
    IIC_MPU_SCL=1;
    IIC_MPU_SDA=1;
    delay_us(5);
    IIC_MPU_SDA=0;
    delay_us(5);
    IIC_MPU_SCL=0;

}

void MPU_IIC_STOP(void)
{
    IIC_MPU_SCL=0;
    IIC_MPU_SDA=0;
    delay_us(5);
    IIC_MPU_SCL=1;
    IIC_MPU_SDA=1;
    delay_us(5);

}

u8 MPU_IIC_Wait_ACK(void)
{
    u8 LongTime=0;

    IIC_MPU_SDA=1;
    delay_us(1);
    IIC_MPU_SCL=1;
    delay_us(1);
    while(IIC_READ_SDA)
    {
        LongTime++;
        if(LongTime>250)
        {
            MPU_IIC_STOP();
            return 1;
        }

    }
    IIC_MPU_SCL=0;
 return 0;
}

void MPU_IIC_ACK(void)
{
    IIC_MPU_SCL=0;
   // delay_us(1);
    IIC_MPU_SDA=0;
    delay_us(1);
    IIC_MPU_SCL=1;
    delay_us(2);
    IIC_MPU_SCL=0;

}

void MPU_IIC_NACK(void)
{

    IIC_MPU_SCL=0;
    delay_us(1);
    IIC_MPU_SDA=1;
    delay_us(1);
    IIC_MPU_SCL=1;
    delay_us(1);
    IIC_MPU_SCL=0;

}

void MPU_IIC_SendByte(u8 data)
{
    u8 i=0;
    IIC_MPU_SCL=0;
    for(i=0;i<8;i++)
    {
        if(data &(1<<7))
            IIC_MPU_SDA=1;
        else
            IIC_MPU_SDA=0;
        data<<=1;
        delay_us(1);
        IIC_MPU_SCL=1;
        delay_us(1);
        IIC_MPU_SCL=0;
        delay_us(1);
        
    }

}

u8 MPU_IIC_ReadByte(u8 ack)
{

    u8 resavedata=0;
    u8 i=0;
    for(i=0;i<8;i++)
    {
        IIC_MPU_SCL=0;
        delay_us(2);
        IIC_MPU_SCL=1;
        resavedata<<=1;
        if(IIC_READ_SDA)
            resavedata++;
        delay_us(2);
    }
    if(!ack)
        MPU_IIC_NACK();
    else
        MPU_IIC_ACK();

return resavedata;


}

u8 MPU_IIC_ReadByteFrommSlave(u8 Addr,u8 reg,u8 *buf)
{
    MPU_IIC_Start();
    MPU_IIC_SendByte(Addr);
    if(MPU_IIC_Wait_ACK())
    {
        MPU_IIC_STOP();
        return 1;
    }
    MPU_IIC_SendByte(reg);
    if(MPU_IIC_Wait_ACK())
    {
        MPU_IIC_STOP();
        return 1;
    }
    MPU_IIC_Start();
    MPU_IIC_SendByte(Addr|0x01);
    MPU_IIC_Wait_ACK();

    *buf=MPU_IIC_ReadByte(0);
    MPU_IIC_STOP();

return 0;

}

u8 MPU_IIC_WriteByteToSlave(u8 Addr,u8 reg,u8 data)
{
    MPU_IIC_Start();
    MPU_IIC_SendByte(Addr);
    if(MPU_IIC_Wait_ACK())
    {
        MPU_IIC_STOP();
        return 1;
    }
    MPU_IIC_SendByte(reg);
    if(MPU_IIC_Wait_ACK())
    {
        MPU_IIC_STOP();
        return 1;
    }
    MPU_IIC_SendByte(data);
    if(MPU_IIC_Wait_ACK())
    {
        MPU_IIC_STOP();
        return 1;
    }
     MPU_IIC_STOP();

return 0;

}

u8 MPU_IIC_ReadLengthData(u8 Addr,u8 reg,u8 *buf,u8 length)
{
    u8 i=0;
		uint8_t temp;
    MPU_IIC_Start();
    MPU_IIC_SendByte(Addr);
    if(MPU_IIC_Wait_ACK())
    {
        MPU_IIC_STOP();
        return 1;
    }
    MPU_IIC_SendByte(reg);
    MPU_IIC_Wait_ACK();

    MPU_IIC_Start();
    MPU_IIC_SendByte(Addr|0x01);
    MPU_IIC_Wait_ACK();
    for(i=0;i<length;i++)
    {
        if(i!=length-1)
            temp=MPU_IIC_ReadByte(1);
        else
            temp=MPU_IIC_ReadByte(0);
				buf[i]=temp;
			delay_us(2);
    }
    MPU_IIC_STOP();

return 0;

}




