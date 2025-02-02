#include "SYSTEM/systick/systick.h"
#include "FreeRTOS.h"
#include "task.h"

extern void xPortSysTickHandler(void);

static u8  fac_us=0;		   


void SysTick_Init(u16 SYSCLK)
{
	uint32_t reload;
	SysTick->CTRL=0;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;
	reload=SYSCLK/8;
	reload *= 1000000 / configTICK_RATE_HZ;
	SysTick->CTRL |= 1 << 1;                            
    SysTick->LOAD = reload;                            
    SysTick->CTRL |= 1 << 0;                            

}


void delay_us(u32 us)
{
	 uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD的值 */
    ticks = us * fac_us;                 /* 需要的节拍数 */
    told=SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* 这里注意一下SYSTICK是一个递减的计数器就可以了 */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) 
            {
                break;                      /* 时间超过/等于要延迟的时间,则退出 */
            }
        }
    }

}

void delay_ms(u32 ms)
{
	
	u32 i=0;
	for(i=0;i<ms;i++)
	{
		delay_us(1000);
	
	}

}

void SysTick_Handler(void)
{
	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
	{
		
		
		xPortSysTickHandler();
	
	}


}


