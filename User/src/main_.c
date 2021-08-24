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

void Main_(void) {
  // MotorCtrl_SetTarget(-100, 0);
  // MotorCtrl_SetTarget(100, 1);
  // MotorCtrl_SetTarget(-100, 2);
  // MotorCtrl_SetTarget(100, 3);
  MotorCtrl_SetDirection(Motor_A, Motor_CW);
  MotorCtrl_SetDirection(Motor_B, Motor_CCW);
  MotorCtrl_SetDirection(Motor_C, Motor_CCW);
  MotorCtrl_SetDirection(Motor_D, Motor_CW);


  // Steer_Init();
  HAL_TIM_Base_Start_IT(&htim5);
  while (1) {
    Pushrod_MoveBackward(40000);
    HAL_Delay(1000);
    Pushrod_MoveForward(40000);
    HAL_Delay(1000);
    // HAL_GPIO_WritePin(Pushrod_Pulse_GPIO_Port, Pushrod_Pulse_Pin, GPIO_PIN_SET);
    // HAL_Delay(1000);
    // osDelay(50);
    // HAL_GPIO_WritePin(Pushrod_Pulse_GPIO_Port, Pushrod_Pulse_Pin, GPIO_PIN_RESET);
  }

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



  for(;;) {
    /*CODE*/
    HAL_Delay(1000);
  Pushrod_MoveForward(200);
    MotorCtrl_PrintArguments();
    printf("A%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[0] * ((Motor_InformationInstance.Directions[0] == Motor_CW) ? 1 : -1));
    printf("B%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[1] * ((Motor_InformationInstance.Directions[1] == Motor_CW) ? 1 : -1));
    printf("C%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[2] * ((Motor_InformationInstance.Directions[2] == Motor_CW) ? 1 : -1));
    printf("D%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[3] * ((Motor_InformationInstance.Directions[3] == Motor_CW) ? 1 : -1));
    printf("Running\r\n");
  }
}

/**
 * @brief 函数调用频率为1000Hz
 * 自行调整对应时间
 * 
 */
void FrequentlyCalledUpdate() {
    static uint16_t cnt = 0;
    // 500 Hz
    if (cnt & 0x0001) {
        cnt = 0;
        // MotorCtrl_CalculateNextOutput();
        // MotorCtrl_UpdateControlFlow();
        // Motion_UpdateTargetsInVelocity();
        // Motion_PIDUpdateHighFrequency();
    }
    // 100Hz
    if (!(cnt % 10)) {
      // Motion_PIDUpdateLowFrequency();
    }
    // 10Hz
    if (!(cnt % 100)) {
      // PositionUPdate, 还没写
    }
    cnt = (cnt + 1) % 1000;
}
