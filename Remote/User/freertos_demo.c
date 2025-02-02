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
#include "BSP/ADC/adc.h"
#include "Remote/remote.h"

#define START_TASK_PRIO	1
#define START_TASK_STACK_SIZE 128
TaskHandle_t	Start_Task_Handler;
void Start_Task(void * pvParameters);

#define TASK1_PRIO	2
#define TASK1_STACK_SIZE 512
TaskHandle_t	Task1_Handler;
void Task1(void * pvParameters);

#define MPU6050_TASK_PRIO	3
#define MPU6050_TASK_STACK_SIZE 1024
TaskHandle_t	MPU6050_TASK_Handler;
void MPU6050_Task(void * pvParameters);



struct Attitude_Angle_t MY_Attitude_Angle;

void HardWare_Init(void)
{
	Usart_Init(115200);
	LED_Init();
	
	TIM3_PWM_Init();
	Si24r_Init();
	Si24r_TX_Mode();
	EXIT_GPIOC8_Init();
	ADC1_Init();
	Remote_Offset_Data();

	Time4_Init(500-1,7200-1);
	printf("HardWare Init Finish...\r\n");

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
						#if 1
	xTaskCreate( (TaskFunction_t      ) MPU6050_Task,
               (char *                ) "MPU6050_Task", 
               (configSTACK_DEPTH_TYPE) MPU6050_TASK_STACK_SIZE,
               (void *                ) NULL,
               (UBaseType_t           ) MPU6050_TASK_PRIO,
               (TaskHandle_t *        ) MPU6050_TASK_Handler );
						#endif
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();					 


}


void Task1( void * pvParameters )
{	
		while(1)
		{
			LED=~LED;
			

			vTaskDelay(200);
		
		}
	
	
}

void MPU6050_Task(void * pvParameters)
{
	float att_buf[3];
	while(1)
	{

		Decode_Data(att_buf);
		
		Remote_Data();
		
		
		printf("Roll:%.2f\r\n",att_buf[0]);
		printf("Pitch:%.2f\r\n",att_buf[1]);
		printf("Yaw:%.2f \r\n",att_buf[2]);
		
		vTaskDelay(5);

	}



}

