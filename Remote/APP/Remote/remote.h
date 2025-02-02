#ifndef __REMOTE_H
#define __REMOTE_H


#include "SYSTEM/system/system.h"

#define Filter_NUM  10

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

struct Filter_Window_t
{

    uint32_t sum;
    uint16_t old[Filter_NUM];


};

struct Remote_Offset_t
{
    int16_t THR;
    int16_t YAW;
    int16_t ROL;
    int16_t PIT;

};

extern struct Remote_Data_t My_Remote_Data;


void Remote_ADC_Data(void);
void Remote_Window_Filter(void);
void Remote_Offset_Data(void);
void Max_Min_Limitations(void);
void Remote_Middle_Limit(void);
void Remote_Send_Data(void);
void Remote_Data(void);
void Decode_Data(float * Att_Buf);

#endif
