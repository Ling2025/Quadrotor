#include "filtering.h"


struct Kaerman_struct_t Kaerman[3]={
        {0,0.02f,0,0,0,0,0,0.001f,0.543f},
        {0,0.02f,0,0,0,0,0,0.001f,0.543f},
        {0,0.02f,0,0,0,0,0,0.001f,0.543f}
};

struct Low_Pass_Filtering_t Low_Pass[3]={
        {0.0f,0.0f},
        {0.0f,0.0f},
        {0.0f,0.0f}
};

void KaermanFilter(struct Kaerman_struct_t *Kaerman_Filter,signed int input)
{
    Kaerman_Filter->Xkk_1=Kaerman_Filter->Xk_1;
    Kaerman_Filter->Pkk_1=Kaerman_Filter->Pk_1+Kaerman_Filter->Q;

    Kaerman_Filter->Kg=Kaerman_Filter->Pkk_1/(Kaerman_Filter->Pkk_1+Kaerman_Filter->R);
    Kaerman_Filter->Xk=Kaerman_Filter->Xkk_1+Kaerman_Filter->Kg*(input-Kaerman_Filter->Xkk_1);
    Kaerman_Filter->Pk=(1.0f-Kaerman_Filter->Kg)*Kaerman_Filter->Pkk_1;

    Kaerman_Filter->Xk_1=Kaerman_Filter->Xk;
    Kaerman_Filter->Pk_1=Kaerman_Filter->Pk;


}


void LowPassFilter(struct Low_Pass_Filtering_t *LowPass_Filter,signed int input)
{
    static u8 t=0;
    float input_float=(float)input;
    if(t==0)
    {
        LowPass_Filter->last=input_float;
        t=1;
    }
    
    LowPass_Filter->current=(1.0f-PASS_LOW_FILTER_ALPHA)*LowPass_Filter->last+PASS_LOW_FILTER_ALPHA*input_float;
    LowPass_Filter->last=LowPass_Filter->current;

}

