#ifndef __SI24R_H
#define __SI24R_H

#include "SYSTEM/system/system.h"


#define Si24r_Write  0x20
#define Si24r_Read   0x00


#define SI24R_CS   PBout(12)
#define SI24R_CE   PAout(8)
#define SI24R_IRQ  PCin(8)
#define SI24R_TX_ADDR_WIDTH 5
#define SI24R_RX_ADDR_WIDTH 5

#define TX_PLOAD_WIDTH      32
#define RX_PLOAD_WIDTH      32

#define SI24R_W_TX_PAYLOAD  0xA0//写FIFO地址
#define SI24R_R_RX_PAYLOAD  0x61//读FIFO地址
#define FLUSH_TX            0XE1
#define FLUSH_RX            0XE2
#define REUSE_TX_PL         0XE3
#define	W_ACK_PAYLOAD		0xA8

#define SI24R_CONFIG        0x00//配置寄存器
#define SI24R_EN_AA         0x01//使能自动确认
#define SI24R_EN_RXADDR     0x02//使能接收数据管道地址
#define SI24R_SETUP_AW      0x03//地址宽度配置
#define SI24R_SETUP_RETR    0x04//自动重发配置
#define SI24R_RF_CH         0x05//射频信道
#define SI24R_RF_SETUP      0x06//射频配置
#define SI24R_STATUS        0x07//状态寄存器(SPI操作的第一个字节，状态寄存器值通过MISO串行输出)。
#define SI24R_OBSERVE_TX    0x08//发射结果统计
#define SI24R_RSSI          0x09//接收信号强度检测
#define SI24R_RX_ADDR_P0    0x0A//数据管道0的接收地址，最大宽度为5bytes (LSByte最先写入，通过SETUP_AW配置地址宽度) 。
#define SI24R_RX_ADDR_P1    0x0B
#define SI24R_RX_ADDR_P2    0x0C
#define SI24R_RX_ADDR_P3    0x0D
#define SI24R_RX_ADDR_P4    0x0E
#define SI24R_RX_ADDR_P5    0x0F//数据管道5的接收地址的最低字节
#define SI24R_TX_ADDR       0x10//发射方的发射地址(LSByte最先写入)
#define SI24R_RX_PW_P0      0x11//接收数据管道0数据字节数
#define SI24R_RX_PW_P1      0x12//接收数据管道1数据字节数
#define SI24R_RX_PW_P2      0x13//接收数据管道2数据字节数
#define SI24R_RX_PW_P3      0x14//接收数据管道3数据字节数
#define SI24R_RX_PW_P4      0x15//接收数据管道4数据字节数
#define SI24R_RX_PW_P5      0x16//接收数据管道5数据字节数
#define SI24R_FIFO_STATUS   0x17//FIFO状态
#define SI24R_DYNPD         0x1C//使能动态负载长度
#define SI24R_FEATURE       0x1D//特征寄存器

#define		IDLE				0x00	// Idle, no interrupt pending
#define		MAX_RT				0x10	// Max #of TX retrans interrupt
#define		TX_DS				0x20	// TX data sent interrupt
#define		RX_DR				0x40	// RX data received

#define  IT_TX 0x0E 
#define  IT_RX 0x0F 

void Si24r_Init(void);
void Si24r_Write_reg(unsigned char reg, unsigned char value);
unsigned char Si24r_Read_reg(unsigned char reg);
unsigned char Si24r_Read_Data(unsigned char reg,unsigned char *buf,unsigned int size);
unsigned char Si24r_Write_Data(unsigned char reg,unsigned char *buf,unsigned char size);

void Si24r_Check(void);


void SI24R1_TxPacket(uint8_t *txbuf);
void SI24R1_RxPacket(uint8_t *rxbuf);
void Si24r_RX_Mode(void);
void Si24r_TX_Mode(void);
void Si24rset_Mode(uint8_t mode);

extern uint8_t state;

#endif
