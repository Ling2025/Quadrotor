#include "BSP/ADC/adc.h"
#include "SYSTEM/systick/systick.h"
#include "Remote/remote.h"

uint16_t ADC_Buf[4];

void ADC1_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef  ADC_InitSturct;
    DMA_InitTypeDef  DMA_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    DMA_InitStruct.DMA_PeripheralBaseAddr= (u32)&ADC1->DR;
	DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)ADC_Buf;
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize=4;
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority=DMA_Priority_High;
	DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;
	
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);
	DMA_Cmd(DMA1_Channel1,ENABLE);

    ADC_InitSturct.ADC_ContinuousConvMode = ENABLE;
    ADC_InitSturct.ADC_DataAlign=ADC_DataAlign_Right;
    ADC_InitSturct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
    ADC_InitSturct.ADC_Mode=ADC_Mode_Independent;
    ADC_InitSturct.ADC_NbrOfChannel=4;
    ADC_InitSturct.ADC_ScanConvMode=ENABLE;
    
    ADC_Init(ADC1,&ADC_InitSturct);

    ADC_Cmd(ADC1,ENABLE);

    delay_us(10);

    ADC_ResetCalibration(ADC1);//重置指定的ADC的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));//获取ADC重置校准寄存器的状态
	
	ADC_StartCalibration(ADC1);//开始指定ADC的校准状态
	while(ADC_GetCalibrationStatus(ADC1));//获取指定ADC的校准程序

    ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,4, ADC_SampleTime_239Cycles5);

    
	ADC_DMACmd(ADC1,ENABLE);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);

}


