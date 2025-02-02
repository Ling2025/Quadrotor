#ifndef __REMOTEDATA_H
#define __REMOTEDATA_H

#include "SYSTEM/system/system.h"

#define LOCK   0
#define UNLOCK 1

struct Remote_Data_t
{
    uint16_t THR;
    uint16_t YAW;
    uint16_t ROL;
    uint16_t PIT;
    uint16_t AUX1;
    uint16_t AUX2;
    uint16_t AUX3;
    uint16_t AUX4;
    uint16_t AUX5;
    uint16_t AUX6;



};

typedef enum
{
    CONNECT=1,
    UNCONNECT=0 

}Connect_Flag; 

struct Connection_State_t
{
    Connect_Flag Connect_flag;
    uint16_t Connect_count;

};

extern struct Connection_State_t Connection_State;
extern struct Remote_Data_t Remote_Data;
extern uint8_t lock_flag;
extern uint16_t connect_flag;

void Remote_Data_Decode(void);
void Fly_Unlock(void);
void Connect_Communication(void);

#endif
