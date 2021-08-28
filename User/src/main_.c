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

#include "arm_ctrl.h"
#include "motion.h"
#include "motor_ctrl.h"
#include "motor_feedback.h"
#include "pushrod.h"
#include "stdio.h"
#include "steer_ctrl.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "usart.h"

void Main_(void) {
  Motor_Init();

  // MotorCtrl_SetTarget(40, 0);
  // MotorCtrl_SetTarget(40, 1);
  // MotorCtrl_SetTarget(40, 2);
  // MotorCtrl_SetTarget(40, 3);


  // __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 200);
  // __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 200);
  // __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 200);
  // __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, 200);

  // MotorCtrl_SetDirection(0, Motor_CW);
  // MotorCtrl_SetDirection(1, Motor_CW);
  // MotorCtrl_SetDirection(2, Motor_CW);
  // MotorCtrl_SetDirection(3, Motor_CW);

  // Steer_Init();
  HAL_TIM_Base_Start_IT(&htim5);

  // Pushrod_MoveForward(20000);
 
  HAL_Delay(2000);
  // Pushrod_MoveBackward(20000);

  // while (1) {
  //   Pushrod_MoveBackward(480000);
  //   HAL_Delay(5000);
  //   Pushrod_MoveForward(480000);
  //   HAL_Delay(5000);
  // }


  for(;;) {
    /*CODE*/
    Motor_SetX(MOTION_LOW_SPEED);
    HAL_Delay(10000);
    Motor_SetX(0);
    HAL_Delay(5000);
    // MotorCtrl_PrintArguments();

    // MotorCtrl_SetTarget(50, 0);
    // MotorCtrl_SetTarget(50, 1);
    // MotorCtrl_SetTarget(50, 2);
    // MotorCtrl_SetTarget(50, 3);
    // HAL_Delay(500);
    // MotorCtrl_SetTarget(0, 0);
    // MotorCtrl_SetTarget(0, 1);
    // MotorCtrl_SetTarget(0, 2);
    // MotorCtrl_SetTarget(0, 3);
    // HAL_Delay(500);
    // MotorCtrl_SetTarget(-50, 0);
    // MotorCtrl_SetTarget(-50, 1);
    // MotorCtrl_SetTarget(-50, 2);
    // MotorCtrl_SetTarget(-50, 3);
    // HAL_Delay(500);
    // MotorCtrl_SetTarget(0, 0);
    // MotorCtrl_SetTarget(0, 1);
    // MotorCtrl_SetTarget(0, 2);
    // MotorCtrl_SetTarget(0, 3);
    // HAL_Delay(5000);
    printf("A%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[0] * ((Motor_InformationInstance.Directions[0] == Motor_CW) ? 1 : -1));
    printf("B%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[1] * ((Motor_InformationInstance.Directions[1] == Motor_CW) ? 1 : -1));
    printf("C%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[2] * ((Motor_InformationInstance.Directions[2] == Motor_CW) ? 1 : -1));
    printf("D%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[3] * ((Motor_InformationInstance.Directions[3] == Motor_CW) ? 1 : -1));
    printf("Running\r\n");
    // char str[] = {'#'};
    // HAL_UART_Transmit(&huart5, str, 1, 0x0fff);
    // uint8_t ret[5] = {0};
    // HAL_UART_Receive(&huart5, ret, 2, 0x0FFF);
    // if (ret[0] == 0xff) {
    //   char rec[] = "Received";
    //   HAL_UART_Transmit(&huart5, rec, sizeof(rec) + 1, 0xfff);
    // } else {
    //   char rec[] = "NOT received";
    //   HAL_UART_Transmit(&huart5, rec, sizeof(rec) + 1, 0xfff);
    // }
    // uint8_t rec[2] = {0};
    // HAL_UART_Receive(&huart5, rec, 2, 0x0fff);
    // HAL_UART_Transmit(&huart5, rec, 2, 0x0fff);
    // HAL_UART_Transmit(&huart5, ret, 2, 0xFFFF);
  }

  Steer_Init();
  while (1) {
    ARM_Forward_Raise();
    HAL_Delay(1000);
    ARM_Forward_TalonOpen();
    HAL_Delay(1000);
    ARM_Forward_TakeBall();
    HAL_Delay(1000);
    ARM_Forward_TalonClose();
    HAL_Delay(1000);
    ARM_Forward_Raise();
    HAL_Delay(1000);
    ARM_Forward_PutDown();
    HAL_Delay(1000);
    ARM_Forward_TalonOpen();
    HAL_Delay(1000);

    ARM_Backward_Raise();
    HAL_Delay(1000);
    ARM_Backward_TalonOpen();
    HAL_Delay(1000);
    ARM_Backward_TakeBall();
    HAL_Delay(1000);
    ARM_Backward_TalonClose();
    HAL_Delay(1000);
    ARM_Backward_Raise();
    HAL_Delay(1000);
    ARM_Backward_PutDown();
    HAL_Delay(1000);
    ARM_Backward_TalonOpen();
    HAL_Delay(1000);
  }
}

/**
 * @brief 函数调用频率为1000Hz
 * 自行调整对应时间
 * 
 */
void FrequentlyCalledUpdate() {
    static uint16_t cnt = 0;

    if (!cnt) {
        MotorCtrl_CalculateNextOutput();
        MotorCtrl_UpdateControlFlow();
        // Motion_UpdateTargetsInVelocity();
        // Motion_PIDUpdateHighFrequency();
    }
    cnt = (cnt + 1) % 3;
}
