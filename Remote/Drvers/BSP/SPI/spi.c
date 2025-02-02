#include "BSP/SPI/spi.h"
#include "SYSTEM/systick/systick.h"
void SPI2_Init(void)
{
    GPIO_InitTypeDef        GPIO_Initstruct;
    SPI_InitTypeDef         SPI_Initstuct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

    /*SPI的GPIO初始化*/
    GPIO_Initstruct.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Initstruct.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_15;
    GPIO_Initstruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_Initstruct);


    GPIO_Initstruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_Initstruct.GPIO_Pin=GPIO_Pin_14;
    GPIO_Init(GPIOB,&GPIO_Initstruct);

    SPI_Initstuct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;
    SPI_Initstuct.SPI_CPHA=SPI_CPHA_1Edge;
    SPI_Initstuct.SPI_CPOL=SPI_CPOL_Low;
    SPI_Initstuct.SPI_DataSize=SPI_DataSize_8b;
    SPI_Initstuct.SPI_FirstBit=SPI_FirstBit_MSB;
    SPI_Initstuct.SPI_NSS=SPI_NSS_Soft;
    SPI_Initstuct.SPI_Mode=SPI_Mode_Master;
    SPI_Initstuct.SPI_CRCPolynomial=7;
    SPI_Initstuct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;

    SPI_Init(SPI2, &SPI_Initstuct);
    SPI_Cmd(SPI2, ENABLE);

}

unsigned char SPI2_ReadWrite(unsigned char Data)
{
	 while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
	 SPI_I2S_SendData(SPI2, Data);
	
	 while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE));
	 return SPI_I2S_ReceiveData(SPI2);
	 

}

