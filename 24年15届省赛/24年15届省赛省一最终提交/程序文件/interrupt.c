#include "interrupt.h"
#include "main.h"

keys key[4] = {0,0,0,0,0,0};
unsigned int counters = 0;
long time = 0;

//按键扫描
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
    if(htim->Instance == TIM4)
	{
        counters ++;
        if(counters % 100 == 0)
        {
            counters = 0;
            time ++;
        }
        if(counters >= 20000)
            counters = 0;
        key[0].key_sta = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
        key[1].key_sta = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
        key[2].key_sta = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
        key[3].key_sta = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);

        for(unsigned char i = 0;i < 4;i++)
        {
            switch (key[i].key_judge)
            {
                case kCheck:
                        if(key[i].key_sta == 0)
                        {
                            key[i].key_tim ++;
                            if(key[i].key_tim >= 20)
                            {
                                key[i].key_judge = Pressed;
                                key[i].key_tim = 0;
                            }
                        }
                        else
                        {
                            key[i].key_tim = 0;
                        }
                    break;

                case Pressed:
                        if(key[i].key_sta == 0)
                        {
                            key[i].key_tim ++;
                            if(key[i].key_tim >= 1000)
                            {
                                key[i].key_tim = 0;
                                key[i].key_judge = isLong;
                            }
                        }
                        else
                        {
                            key[i].key_tim = 0;
                            key[i].key_judge = isSingle;
                        }
                    break;
                case isSingle:
                        key[i].single_flag = 1;
                        key[i].key_judge = waitUP;
                    break;

                case isLong:
                        key[i].long_flag = 1;
                        key[i].key_judge = waitUP;
                    break;
                
                case waitUP:
                        if(key[i].key_sta == 0)
                        {

                        }
                        else
                        {
                            key[i].key_tim = 0;
                            key[i].key_judge = kCheck;
                        }
                    break;

                default:
                    break;
            }
        }
	}




}


int frq1,frq2;
int temp1,temp2;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        temp1 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
        //清零计数
        __HAL_TIM_SetCounter(htim,0);
        frq1 = (80000000/80/temp1);
        HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_1);
    }

    if(htim->Instance == TIM3)
    {
        temp2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
        //清零计数
        __HAL_TIM_SetCounter(htim,0);
        htim->Instance->CCR1 = 0;
        frq2 = (80000000/80/temp2);
        HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_1);
    }
    
}
