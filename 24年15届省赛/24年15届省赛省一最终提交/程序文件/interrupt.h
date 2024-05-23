#ifndef __INTERRUPT_H_
#define __INTERRUPT_H_

#include "stdbool.h"
#include "tim.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

typedef struct KEYS{
    bool key_sta;
    bool single_flag;
    bool long_flag;
    short int key_tim;
    short int key_wait;
    unsigned char key_judge;
}keys;


//0-4
enum KEYSTA {
    kCheck,Pressed,isSingle,isLong,waitUP
};

#endif


