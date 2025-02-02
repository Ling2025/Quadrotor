#include "REMOTE/remotedata.h"
#include "SYSTEM/usart/usart.h"
#include "BSP/SI24R/si24r.h"
#include "SYSTEM/systick/systick.h"
#include "BSP/EXIT/exit.h"
#include "Contrl/PID/pid.h"

#define STATE1  1//拉低
#define STATE2  2//拉高
#define STATE3  3//拉低
#define STATE4  4//解锁

#define UNLOCK_MAX_TIME 5000

static uint8_t lock_state=STATE1;
uint8_t lock_flag=LOCK;

struct Remote_Data_t Remote_Data;
struct Connection_State_t Connection_State;
void Remote_Data_Decode(void)
{
    uint32_t re_sum=0,my_sum=0;
    uint16_t head=0;
    uint8_t func_word=0;
    uint8_t i=0;

    head=(uint16_t)rxbuf[0]<<8|rxbuf[1];//帧头
    func_word=rxbuf[2];

    if(head!=0xAAAF)
        return;
    if(func_word!=0x03)
        return;

    re_sum=(uint32_t)((rxbuf[24]<<24)|(rxbuf[25]<<16)|(rxbuf[26]<<8)|(rxbuf[27]));
    for(i=4;i<24;i++)
    {
        my_sum+=rxbuf[i];
    }
    if(re_sum==my_sum)
    {
        Remote_Data.THR=(uint16_t)rxbuf[4]<<8|rxbuf[5];
        Remote_Data.YAW=(uint16_t)rxbuf[6]<<8|rxbuf[7];
        Remote_Data.ROL=(uint16_t)rxbuf[8]<<8|rxbuf[9];
        Remote_Data.PIT=(uint16_t)rxbuf[10]<<8|rxbuf[11];
    }
           

}

void Fly_Unlock(void)
{
    static uint16_t statecount=0;
    if(lock_flag==LOCK)
    {
        switch(lock_state)
        {
            case(STATE1):
                if(Remote_Data.THR<1030)
                    lock_state=STATE2;

            break;
            case(STATE2):
                if(Remote_Data.THR>1970)
                    lock_state=STATE3;
            break;
            case(STATE3):
                if(Remote_Data.THR<1030)
                {
                    lock_state=STATE4;
                    printf("UNLOCK\r\n");
                    statecount=0;
                }
                statecount++;
                if(statecount>UNLOCK_MAX_TIME)
                {
                    statecount=0;
                    
                    lock_state=STATE1;
                }
            break;
            case(STATE4):
                lock_flag=UNLOCK;
            break;
            default:

            break;

        }

    }

}

void Connect_Communication(void)
{

    Connection_State.Connect_count++;
    if(Connection_State.Connect_count>1000)
    {
        Connection_State.Connect_flag=UNCONNECT;
        lock_flag=LOCK;
        printf("no connection\r\n");

    }
    else
        Connection_State.Connect_flag=CONNECT;
        
    if((Connection_State.Connect_flag==UNCONNECT)&&(Connection_State.Connect_count>65534))
        Connection_State.Connect_count=1000;


}
