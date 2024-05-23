/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "lcd.h"
#include "interrupt.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern keys key[4];


extern unsigned int counters;
extern unsigned int time;
//时间判定


//id and pressed flag
struct KEYMSG {
  unsigned char id;
  bool p;
}key_msg;

char text[20];


extern int frq1,frq2;
extern int temp1,temp2;

int a_tm;
int b_tm;

short int pd_number = 1000;
short int ph_number = 5000;
short int px_number = 0;

short int nda_number = 0;
short int ndb_number = 0;
short int nha_number = 0;
short int nhb_number = 0;

unsigned char nda_flag = 0;
unsigned char ndb_flag = 0;
unsigned char nha_flag = 0;
unsigned char nhb_flag = 0;
unsigned char pda_flag = 0;
unsigned char pdb_flag = 0;

short int a_hz = 0;
short int b_hz = 0;

short int a_temp = 0;
short int b_temp = 0;
short int a_min = 0;
short int b_min = 0;
short int a_max = 0;
short int b_max = 0;
short int a_max_tick = 0;
short int b_max_tick = 0;
short int a_min_tick = 0;
short int b_min_tick = 0;
unsigned char a_up_flag = 0;
unsigned char b_up_flag = 0;

unsigned char menu_sta = 0;
unsigned char pinglv_flag = 0;
unsigned char choose_flag = 1;

enum MENUSTA{
  DATA,PARA,RECD
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void Londoge_Function(void);
void Londoge_KeyFunction(void);
void Londoge_KeyMsg(void);
void Londoge_Init(void);
void Londoge_Show(void);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
  Londoge_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    Londoge_KeyMsg();
    Londoge_KeyFunction();
    Londoge_Function();
    Londoge_Show();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Londoge_Function(void)
{
  if(px_number > 1000)
    px_number = 1000;
  if(px_number < -1000)
    px_number = -1000;

  if(pd_number < 100)
    pd_number = 100;
  if(pd_number > 1000)
    pd_number = 1000;

  if(ph_number < 1000)
    ph_number = 1000;
  if(ph_number > 10000)
    ph_number = 10000;

  a_hz = frq1 + px_number;
  b_hz = frq2 + px_number;

  if(a_hz > 0)
  {
    a_tm = 1000000/a_hz;
  }
  if(b_hz > 0)
  {
    b_tm = 1000000/b_hz;
  }

//超限判定
  if(ph_number <= a_hz)
  {
    if(nha_flag == 0)
    {
      nha_number += 1;
      nha_flag = 1;
    }
  }
  else
  {
    nha_flag = 0;
  }
  if(ph_number <= b_hz)
  {
    if(nhb_flag == 0)
    {
      nhb_number += 1;
      nhb_flag = 1;
    }
  }
  else
  {
    nhb_flag = 0;
  }

  //频率突变判定
  //依据时间tick counters 判定
  if(time >= 30)
  {
    time = 0;
    a_max = a_hz;
    a_min = a_hz;
    b_max = b_hz;
    b_min = b_hz;
    pda_flag = 1;
    pdb_flag = 1;
  }
  else
  {
    if(a_hz > a_max)
      a_max = a_hz;
    if(a_hz < a_min)
      a_min = a_hz;
    if(a_max - a_min > pd_number && pda_flag == 1)
    {
      nda_number += 1;
      pda_flag = 0;
    }

    if(b_hz > b_max)
      b_max = b_hz;
    if(b_hz < b_min)
      b_min = b_hz;
    if(b_max - b_min > pd_number && pdb_flag == 1)
    {
      ndb_number += 1;
      pdb_flag = 0;
    }
  }



  //依据上升趋势判断是否是突�?
  //如果下降那么�?定不是突�?
  // if(a_hz > a_temp)
  // {
  //   a_up_flag = 1;//正在上升
  //   a_min = a_temp;
  //   a_min_tick = time;
  // }
  // if(a_hz < a_temp)
  // {
  //   a_up_flag = 0;//�?始下�?
  //   a_max = a_temp;
  //   if(time - a_min_tick <= 30 && a_max - a_min >= pd_number)
  //     nda_number += 1;
  // }

  // a_temp = a_hz;

}

void Londoge_KeyFunction(void)
{
  if(key_msg.p == 1)
  {
    key_msg.p = 0;
    switch (key_msg.id)
    {
      case 0:
          if(menu_sta == PARA)
          {
            switch (choose_flag)
            {
              case 1:
                  pd_number += 100;
                break;
              case 2:
                  ph_number += 100;
                break;
              case 3:
                  px_number += 100;
                break;
              default:
                break;
            }
          }
        break;
      case 1:
          if(menu_sta == PARA)
          {
            switch (choose_flag)
            {
              case 1:
                  pd_number -= 100;
                break;
              case 2:
                  ph_number -= 100;
                break;
              case 3:
                  px_number -= 100;
                break;
              default:
                break;
            }
          }
        break;
      case 2:
          switch (menu_sta)
          {
            case DATA:
                if(pinglv_flag == 0)
                  pinglv_flag = 1;
                else
                  pinglv_flag = 0;
                
              break;
            case PARA:
                choose_flag ++;
                if(choose_flag >= 4)
                {
                  choose_flag = 1;
                }
              break;
            
            default:
              break;
          }
        break;
      case 3:
          menu_sta += 1;
          if(menu_sta >= 3)
          {
            menu_sta = 0;
            pinglv_flag = 0;
            choose_flag = 1;
          }
        break;
      case 6:
          if(menu_sta == RECD)
          {
            nda_number = 0;
            ndb_number = 0;
            nha_number = 0;
            nhb_number = 0;
          }
        break;
      
      default:
        break;
    }
  }
}

//提取按键信息
void Londoge_KeyMsg(void)
{
  for(unsigned char i = 0;i < 4;i ++ )
  {
    if(key[i].single_flag == 1)
    {
      key_msg.p = 1;
      key_msg.id = i;
      key[i].single_flag = 0;
    }
    if(key[i].long_flag == 1)
    {
      key_msg.p = 1;
      key_msg.id = i+4;
      key[i].long_flag = 0;
    }
  }

}


void Londoge_Init(void)
{
  HAL_TIM_Base_Start_IT(&htim4);  //按键扫描
  HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1); //校准用pwm发生 PA7 占空�?50% 频率1000

  LCD_Init();
  LCD_SetBackColor(Black);
  LCD_SetTextColor(White);

  LCD_Clear(Black);
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
  GPIOC->BSRR = 0xff00;
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);


//突变数�?�初始化
  a_max = a_hz;
  a_min = a_hz;
} 



void Londoge_Show(void)
{
  // number show for Debug
  // sprintf(text," %d %d %d ",frq1,a_max,a_min);
  // LCD_DisplayStringLine(Line8,(u8*)text);
  // sprintf(text,"     %d     ",frq2);
  // LCD_DisplayStringLine(Line9,(u8*)text);

  switch (menu_sta)
  {
    case DATA:
      sprintf(text,"        DATA        ");
      LCD_DisplayStringLine(Line1,(u8*)text);
      if(pinglv_flag == 0)
      {
        if(a_hz < 1000)
          sprintf(text,"     A=%dHz      ",a_hz);
        else
          sprintf(text,"     A=%.2fkHz     ",(float)a_hz/1000);
        if(a_hz < 0)
          sprintf(text,"     A=NULL      ");
        LCD_DisplayStringLine(Line3,(u8*)text);
        if(b_hz < 1000)
          sprintf(text,"     B=%dHz      ",b_hz);
        else 
          sprintf(text,"     B=%.2fkHz     ",(float)b_hz/1000);
        if(b_hz < 0)
          sprintf(text,"     B=NULL      ");
        LCD_DisplayStringLine(Line4,(u8*)text);
      }
      else
      {
        if(a_tm < 1000)
          sprintf(text,"     A=%duS      ",a_tm);
        else
          sprintf(text,"     A=%.2fmS     ",(float)a_tm/1000);
        if(a_hz < 0)
          sprintf(text,"     A=NULL      ");
        LCD_DisplayStringLine(Line3,(u8*)text);
        memset(text,0,sizeof(text));
        if(b_tm < 1000)
          sprintf(text,"     B=%duS      ",b_tm);
        else
          sprintf(text,"     B=%.2fmS     ",(float)b_tm/1000);
        if(b_hz < 0)
          sprintf(text,"     B=NULL      ");
        LCD_DisplayStringLine(Line4,(u8*)text);
      }
      
      sprintf(text,"                    ");
      LCD_DisplayStringLine(Line5,(u8*)text);
      sprintf(text,"                    ");
      LCD_DisplayStringLine(Line6,(u8*)text);
      break;

    case PARA:
      sprintf(text,"        PARA        ");
      LCD_DisplayStringLine(Line1,(u8*)text);
      sprintf(text,"     PD=%dHz      ",pd_number);
      LCD_DisplayStringLine(Line3,(u8*)text);
      sprintf(text,"     PH=%dHz     ",ph_number);
      LCD_DisplayStringLine(Line4,(u8*)text);
      sprintf(text,"     PX=%dHz     ",px_number);
      LCD_DisplayStringLine(Line5,(u8*)text);

      switch (choose_flag)
      {
        case 0:
            
          break;
        case 1:
          sprintf(text,"    >");
          LCD_DisplayStringLine(Line3,(u8*)text);
          break;
        case 2:
          sprintf(text,"    >");
          LCD_DisplayStringLine(Line4,(u8*)text);
          break;
        case 3:
          sprintf(text,"    >");
          LCD_DisplayStringLine(Line5,(u8*)text);
          break;
        
        default:
          break;
      }
      break;
    case RECD:
      sprintf(text,"        RECD        ");
      LCD_DisplayStringLine(Line1,(u8*)text);
      sprintf(text,"     NDA=%d      ",nda_number);
      LCD_DisplayStringLine(Line3,(u8*)text);
      sprintf(text,"     NDB=%d      ",ndb_number);
      LCD_DisplayStringLine(Line4,(u8*)text);
      sprintf(text,"     NHA=%d     ",nha_number);
      LCD_DisplayStringLine(Line5,(u8*)text);
      sprintf(text,"     NHB=%d     ",nhb_number);
      LCD_DisplayStringLine(Line6,(u8*)text);
      break;
    
    default:
      break;
  }

  //LED显示�?�?
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
  GPIOC->BSRR = 0xff00;
  if(menu_sta == DATA)
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
  if(a_hz > ph_number)
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
  if(b_hz > ph_number)
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);
  if(nda_number >= 3 || ndb_number >= 3)
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
