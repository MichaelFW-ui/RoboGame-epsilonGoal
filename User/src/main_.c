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

#include "motion.h"
#include "motor_ctrl.h"
#include "motor_feedback.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"
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
    }
    cnt = (cnt + 1) % 1000;
}
