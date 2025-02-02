#include "BSP/SI24R/si24r.h"
#include "BSP/SPI/spi.h"
#include "SYSTEM/systick/systick.h"
#include "SYSTEM/usart/usart.h"


unsigned char TX_ADDR[5]={0x34,0x43,0x10,0x10,0x01};
unsigned char RX_ADDR[5]={0x34,0x43,0x10,0x10,0x01};


uint8_t state;

void Si24r_Init(void)
{
    /*初始化IO口*/
    GPIO_InitTypeDef        GPIO_Initstruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	

    GPIO_Initstruct.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Initstruct.GPIO_Pin=GPIO_Pin_12;
    GPIO_Initstruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_Initstruct);

    GPIO_Initstruct.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Initstruct.GPIO_Pin=GPIO_Pin_8;
    GPIO_Initstruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_Initstruct);

    SPI2_Init();
    Si24r_Check();

    SI24R_CS=1;
    SI24R_CE=0;
    
    
    
}

void Si24r_Write_reg(unsigned char reg, unsigned char value)
{
    SI24R_CS=0;
    SPI2_ReadWrite(reg);

    SPI2_ReadWrite(value);
    SI24R_CS=1;

}

unsigned char Si24r_Read_reg(unsigned char reg)
{
    unsigned char reg_value;
    SI24R_CS=0;
    SPI2_ReadWrite(reg);
    reg_value=SPI2_ReadWrite(0xFF);
    SI24R_CS=1;
    return reg_value;
}

unsigned char Si24r_Write_Data(unsigned char reg,unsigned char *buf,unsigned char size)
{

    u8 i=0,status;

    SI24R_CS=0;
    status=SPI2_ReadWrite(reg);
    for(i=0;i<size;i++)
    {
        SPI2_ReadWrite(*buf);
        buf++;

    }
    SI24R_CS=1;
return status;
}

unsigned char Si24r_Read_Data(unsigned char reg,unsigned char *buf,unsigned int size)
{
    u8 i=0,status;

    SI24R_CS=0;
    status=SPI2_ReadWrite(reg);
    for(i=0;i<size;i++)
    {
        buf[i]=SPI2_ReadWrite(0xFF);
    }
    SI24R_CS=1;

return status;

}

unsigned char Si24r_TestConnection(void)
{
    uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i; 	 

    Si24r_Write_Data(Si24r_Write+SI24R_TX_ADDR,buf,5);
    Si24r_Read_Data(Si24r_Read+SI24R_TX_ADDR,buf,5);
    for(i=0;i<5;i++)
    {
        if(buf[i]!=0XA5)
        break;	 							   	
    }
	if(i!=5)
        return 0;
	return 1;

}

void Si24r_RX_Mode(void)
{
    SI24R_CE=0;

    Si24r_Write_reg(Si24r_Write+SI24R_SETUP_AW,0x03);

    Si24r_Write_Data(Si24r_Write+SI24R_RX_ADDR_P0,TX_ADDR,SI24R_RX_ADDR_WIDTH);
    Si24r_Write_reg(Si24r_Write+SI24R_EN_AA,0x01);
    Si24r_Write_reg(Si24r_Write+SI24R_EN_RXADDR,0x01);
    Si24r_Write_reg(Si24r_Write+SI24R_RF_CH,30);
    Si24r_Write_reg(Si24r_Write+SI24R_RX_PW_P0,RX_PLOAD_WIDTH);
    Si24r_Write_reg(Si24r_Write+SI24R_RF_SETUP,0x27);
    Si24r_Write_reg(Si24r_Write+SI24R_CONFIG,0x0f);
    Si24r_Write_reg(Si24r_Write+SI24R_STATUS,0xff);
	
//	Si24r_Write_reg(Si24r_Write+SI24R_STATUS,0X7e);

    Si24r_Write_reg(FLUSH_TX,0xff);
   Si24r_Write_reg(FLUSH_RX,0xff);

    Si24rset_Mode(IT_RX);
	
    SI24R_CE=1;

}

void Si24r_TX_Mode(void)
{	
    SI24R_CE=0;
    
    Si24r_Write_reg(Si24r_Write+SI24R_SETUP_AW,0x03);
    Si24r_Write_Data(Si24r_Write+SI24R_TX_ADDR,TX_ADDR,SI24R_TX_ADDR_WIDTH);
    Si24r_Write_Data(Si24r_Write+SI24R_RX_ADDR_P0,RX_ADDR,SI24R_RX_ADDR_WIDTH);
    Si24r_Write_reg(Si24r_Write+SI24R_SETUP_RETR,0xff);
    Si24r_Write_reg(Si24r_Write+SI24R_EN_RXADDR,0x01);
    Si24r_Write_reg(Si24r_Write+SI24R_EN_AA,0x01);
    Si24r_Write_reg(Si24r_Write+SI24R_RX_PW_P0,RX_PLOAD_WIDTH);
    Si24r_Write_Data(Si24r_Write+SI24R_RX_ADDR_P0,RX_ADDR,SI24R_RX_ADDR_WIDTH);
    Si24r_Write_reg(Si24r_Write+SI24R_RF_CH,30);
    Si24r_Write_reg(Si24r_Write+SI24R_RF_SETUP,0x27);

    Si24rset_Mode(IT_TX);

    Si24r_Write_reg(FLUSH_TX,0xff);
    Si24r_Write_reg(FLUSH_RX,0xff);


    SI24R_CE=1;

}


void Si24r_Check(void)
{
    while(! Si24r_TestConnection())
    {
        printf("Si24r no connection\r\n");
    }
    printf("Si24r connection\r\n");

}


void SI24R1_TxPacket(uint8_t *txbuf)
{

	SI24R_CE=0;	 
     Si24r_Write_reg(Si24r_Write+SI24R_STATUS,0x70);
    Si24r_Write_reg(Si24r_Write+SI24R_CONFIG,0x0e);
    Si24r_Write_Data(Si24r_Write+SI24R_TX_ADDR,TX_ADDR,SI24R_TX_ADDR_WIDTH);
    Si24r_Write_Data(Si24r_Write+SI24R_RX_ADDR_P0,RX_ADDR,SI24R_RX_ADDR_WIDTH);
    Si24r_Write_Data(SI24R_W_TX_PAYLOAD,txbuf,TX_PLOAD_WIDTH); 
    SI24R_CE=1;
	delay_us(10);
 
 //   state=Si24r_Read_reg(Si24r_Read+SI24R_STATUS);
 //   Si24r_Write_reg(Si24r_Write+SI24R_STATUS,state);


}

void SI24R1_RxPacket(uint8_t *rxbuf)
{

    SI24R_CE=0;
    Si24r_Read_Data(Si24r_Read+SI24R_R_RX_PAYLOAD,rxbuf,RX_PLOAD_WIDTH);
    Si24r_Write_reg(FLUSH_RX,0xff); 
    SI24R_CE=1;	
	
  //  state=Si24r_Read_reg(Si24r_Read+SI24R_STATUS);
  //  Si24r_Write_reg(Si24r_Write+SI24R_STATUS,state);

}

void Si24rset_Mode(uint8_t mode)
{
    if(mode ==IT_TX)
    {
        SI24R_CE=0;
        Si24r_Write_reg(Si24r_Write+SI24R_CONFIG,0x0e);
        Si24r_Write_reg(Si24r_Write+SI24R_STATUS,0X7e);
        SI24R_CE=1;
    }
    else
    {
        SI24R_CE=0;
        Si24r_Write_reg(Si24r_Write+SI24R_CONFIG,0x0f);
        Si24r_Write_reg(Si24r_Write+SI24R_STATUS,0X7e);
        SI24R_CE=1;
        delay_us(200);

    }


}
