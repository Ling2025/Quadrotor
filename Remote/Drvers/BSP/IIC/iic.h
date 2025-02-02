#ifndef __IIC_H
#define __IIC_H

#include "SYSTEM/system/system.h"

#define IIC_MPU_SCL_PORT GPIOB
#define IIC_MPU_SCL_PIN  GPIO_Pin_6
#define IIC_MPU_SCL_RCC  RCC_APB2Periph_GPIOB

#define IIC_MPU_SDA_PORT GPIOB
#define IIC_MPU_SDA_PIN  GPIO_Pin_7
#define IIC_MPU_SDA_RCC  RCC_APB2Periph_GPIOB

#define IIC_MPU_SCL      PBout(6)
#define IIC_MPU_SDA      PBout(7)
#define IIC_READ_SDA     PBin(7)

void IIC_MPU_Init(void);
u8 MPU_IIC_ReadByteFrommSlave(u8 Addr,u8 reg,u8 *buf);
u8 MPU_IIC_WriteByteToSlave(u8 Addr,u8 reg,u8 data);
u8 MPU_IIC_ReadByte(u8 ack);
u8 MPU_IIC_ReadLengthData(u8 Addr,u8 reg,u8 *buf,u8 length);

#endif
