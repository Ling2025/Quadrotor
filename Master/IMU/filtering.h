#ifndef __FILTERING_H
#define __FILTERING_H

#include "SYSTEM/system/system.h"

#define PASS_LOW_FILTER_ALPHA 0.15f
struct Kaerman_struct_t
{
    float Xk_1; //x(k-1|k-1)
    float Pk_1; //p(k-1|k-1)
    float Xkk_1; //x(k|k-1)
    float Pkk_1; //p(k|k-1)
    float Kg; //Kg(k)
    float Xk; //x(k|k)
    float Pk; //p(k|k)
    float Q;
    float R;


};

struct Low_Pass_Filtering_t
{
    float current;
    float last;
};


extern struct Kaerman_struct_t Kaerman[3];
extern struct Low_Pass_Filtering_t Low_Pass[3];
void KaermanFilter(struct Kaerman_struct_t *Kaerman_Filter,signed int input);
void LowPassFilter(struct Low_Pass_Filtering_t *LowPass_Filter,signed int input);

#endif
