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
#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "motor_feedback.h"
#include "motor_ctrl.h"
#include "tim.h"


void Main_(void) {
  // MotorCtrl_SetTarget(-100, 0);
  // MotorCtrl_SetTarget(100, 1);
  // MotorCtrl_SetTarget(-100, 2);
  // MotorCtrl_SetTarget(100, 3);
  MotorCtrl_SetDirection(Motor_A, Motor_CW);
  MotorCtrl_SetDirection(Motor_B, Motor_CW);
  MotorCtrl_SetDirection(Motor_C, Motor_CW);
  MotorCtrl_SetDirection(Motor_D, Motor_CW);

  for(;;) {
    /*CODE*/
    HAL_Delay(1000);
    MotorCtrl_PrintArguments();
    printf("A%u\r\n", Motor_InformationInstance.TimeTicks[0]);
    printf("B%u\r\n", Motor_InformationInstance.TimeTicks[1]);
    printf("C%u\r\n", Motor_InformationInstance.TimeTicks[2]);
    printf("D%u\r\n", Motor_InformationInstance.TimeTicks[3]);
    printf("Running\r\n");
  }
}

