#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BSP/LED/led.h"
#include "SYSTEM/system/system.h"
#include "SYSTEM/systick/systick.h"
#include "SYSTEM/usart/usart.h"
#include "BSP/IIC/iic.h"
#include "BSP/MPU-6050/mpu6050.h"
#include "imu.h"
#include "com_ano.h"
#include "BSP/MOTOR/motor.h"
#include "BSP/SPI/spi.h"
#include "BSP/SI24R/si24r.h"
#include "BSP/EXIT/exit.h"
#include "BSP/TIME/time4.h"
#include "REMOTE/remotedata.h"
#include "Contrl/CTRL/ctrl.h"

#define START_TASK_PRIO	1
#define START_TASK_STACK_SIZE 128
TaskHandle_t	Start_Task_Handler;
void Start_Task(void * pvParameters);

#define TASK1_PRIO	2
#define TASK1_STACK_SIZE 128
TaskHandle_t	Task1_Handler;
void Task1(void * pvParameters);

#define MPU6050_TASK_PRIO	3
#define MPU6050_TASK_STACK_SIZE 1024
TaskHandle_t	MPU6050_TASK_Handler;
void MPU6050_Task(void * pvParameters);

#define REMOTE_TASK_PRIO	4
#define REMOTE_TASK_STACK_SIZE 512
TaskHandle_t	REMOTE_TASK_Handler;
void REMOTE_Task(void * pvParameters);


unsigned char txbuf[32]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};


void HardWare_Init(void)
{

	Usart_Init(115200);
	LED_Init();
	PID_Data_Init();
	
	MPU6050_Init();
	TIM3_PWM_Init();

	Si24r_Init();

	Si24r_TX_Mode();
	EXIT_GPIOC8_Init();
	Time4_Init(500-1,7200-1);
	printf("\r\n");

	printf("HardWare Init Finish...\r\n");
	delay_ms(100);
}


void freertos_demo(void)
{

	xTaskCreate( (TaskFunction_t        ) Start_Task,
               (char *                ) "Start_Task", 
               (configSTACK_DEPTH_TYPE) START_TASK_STACK_SIZE,
               (void *                ) NULL,
               (UBaseType_t           ) START_TASK_PRIO,
               (TaskHandle_t *        ) Start_Task_Handler );

	vTaskStartScheduler();

}


void Start_Task(void * pvParameters)
{

	taskENTER_CRITICAL();
	xTaskCreate( (TaskFunction_t        ) Task1,
               (char *                ) "Task1", 
               (configSTACK_DEPTH_TYPE) TASK1_STACK_SIZE,
               (void *                ) NULL,
               (UBaseType_t           ) TASK1_PRIO,
               (TaskHandle_t *        ) Task1_Handler );
					
	xTaskCreate( (TaskFunction_t      ) MPU6050_Task,
               (char *                ) "MPU6050_Task", 
               (configSTACK_DEPTH_TYPE) MPU6050_TASK_STACK_SIZE,
               (void *                ) NULL,
               (UBaseType_t           ) MPU6050_TASK_PRIO,
               (TaskHandle_t *        ) MPU6050_TASK_Handler );

	xTaskCreate( (TaskFunction_t      ) REMOTE_Task,
               (char *                ) "REMOTE_Task", 
               (configSTACK_DEPTH_TYPE) REMOTE_TASK_STACK_SIZE,
               (void *                ) NULL,
               (UBaseType_t           ) REMOTE_TASK_PRIO,
               (TaskHandle_t *        ) REMOTE_TASK_Handler );

	vTaskDelete(NULL);
	taskEXIT_CRITICAL();					 


}


void Task1( void * pvParameters )
{
		while(1)
		{
			LED=~LED;
			vTaskDelay(300);
		
		}
	
}

void MPU6050_Task(void * pvParameters)
{

	while(1)
	{

		Get_Attitude_Angle(&MY_Attitude_Angle);
		Fly_Control(&MY_Attitude_Angle,&MPU6050_Original_Data,&Remote_Data,lock_flag);
		
		vTaskDelay(10);

	}



}

void REMOTE_Task(void * pvParameters)
{

	while(1)
	{
		Connect_Communication();

		if(rx_flag==1)
		{
			rx_flag=0;
		 	Remote_Data_Decode();
			Fly_Unlock();
		}


		vTaskDelay(5);
	}


}
