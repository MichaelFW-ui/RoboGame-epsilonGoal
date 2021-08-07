/**
 * @file main_.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 调试运动流程
 * @version 0.1
 * @date 2021-08-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "main_.h"
#include "debug.h"
#include "stm32f1xx_hal.h"
#include "motor_feedback.h"


void Main__Main(void) {

  for(;;) {
    /*CODE*/
    MotorFeedback_Init();
    HAL_Delay(1000);
    Debug_PutString(
      "%d %d %d %d\r\n",
      Motor_InformationInstance.TimeTicks[0] * (1000 - TIM3->CCR1),
      Motor_InformationInstance.TimeTicks[1] * (1000 - TIM3->CCR2),
      (1000 - TIM3->CCR1),
      (1000 - TIM3->CCR2));
    Debug_PutString("Running\r\n");
  }
}

