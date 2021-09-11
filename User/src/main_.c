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
#include "debug.h"
#include "motor.h"
#include "motion.h"
#include "motor_ctrl.h"
#include "motor_feedback.h"
#include "pushrod.h"
#include "stdio.h"
#include "steer_ctrl.h"
#include "stm32f1xx_hal.h"
#include "cannon.c"
#include "tim.h"
#include "usart.h"
#include "sensor.h"
#include "position.h"

MotorSpeed_t Motor_X, Motor_Y, Motor_W;

void Main_(void) {
  Motor_Init();
  Debug_Init();
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

  // for (;;) {
    // Pushrod_MoveBackward(300000);
    // HAL_Delay(8000);
    // Pushrod_MoveForward(300000);
  //   HAL_Delay(8000);
  // }
  // Cannon_SetTargetSpeed(2000);
//   for (;;) {
//     Motion_MoveForward(25);
//     while (1) {
//       Motion_CorrectWhenMovingAtY();
//       // Motor_Decode(20, 4, 0);
//       static int cnt = 0;
      
//       HAL_Delay(100);
//       cnt = (cnt + 1) % 10;
// #define ISHIGH(x, n) (!!(x & (1 << n)))

//       if (!cnt) {
//           TraceInfo_t *ptr = Sensor_GetCurrentInfo();
//           printf("X:%dY%dW%d\r\n", Motor_InputInstance.x, Motor_InputInstance.y, Motor_InputInstance.w);
//           printf("%x,%x,%x,%x\r\n", ptr[0], ptr[1], ptr[2], ptr[3]);
//           printf(" %d%d%d%d%d%d%d%d%d\r\n", ISHIGH(ptr[0], 8), ISHIGH(ptr[0], 7), ISHIGH(ptr[0], 6), ISHIGH(ptr[0], 5), ISHIGH(ptr[0], 4), ISHIGH(ptr[0], 3), ISHIGH(ptr[0], 2), ISHIGH(ptr[0], 1), ISHIGH(ptr[0], 0));
//           printf("%d           %d\r\n", ISHIGH(ptr[1], 10), ISHIGH(ptr[2], 10));
//           printf("%d           %d\r\n", ISHIGH(ptr[1], 9), ISHIGH(ptr[2], 9));
//           printf("%d           %d\r\n", ISHIGH(ptr[1], 8), ISHIGH(ptr[2], 8));
//           printf("%d           %d\r\n", ISHIGH(ptr[1], 7), ISHIGH(ptr[2], 7));
//           printf("%d           %d\r\n", ISHIGH(ptr[1], 6), ISHIGH(ptr[2], 6));
//           printf("%d           %d\r\n", ISHIGH(ptr[1], 5), ISHIGH(ptr[2], 5));
//           printf("%d           %d\r\n", ISHIGH(ptr[1], 4), ISHIGH(ptr[2], 4));
//           printf("%d           %d\r\n", ISHIGH(ptr[1], 3), ISHIGH(ptr[2], 3));
//           printf("%d           %d\r\n", ISHIGH(ptr[1], 2), ISHIGH(ptr[2], 2));
//           printf("%d           %d\r\n", ISHIGH(ptr[1], 1), ISHIGH(ptr[2], 1));
//           printf("%d           %d\r\n", ISHIGH(ptr[1], 0), ISHIGH(ptr[2], 0));
//           printf(" %d%d%d%d%d%d%d%d%d\r\n", ISHIGH(ptr[3], 8), ISHIGH(ptr[3], 7), ISHIGH(ptr[3], 6), ISHIGH(ptr[3], 5), ISHIGH(ptr[3], 4), ISHIGH(ptr[3], 3), ISHIGH(ptr[3], 2), ISHIGH(ptr[3], 1), ISHIGH(ptr[3], 0));
//       }
//     }
//   }
//   for(;;) {
//     HAL_Delay(1000);
//     Motion_MoveForward(20);
//     while (1) {
//       // TraceInfo_t *ptr = Sensor_GetCurrentInfo();
//       // uint8_t beg, end;
//       // Position_GetOneActive(ptr[0], 9, &beg, &end);
//       // if (end - beg > 4) {
//       //     HAL_Delay(200);
//       //     Motor_Decode(0, 0, 0);
//       //     while (1) {
//       //         ;
//       //     }
//       // }
//       Motion_CorrectWhenMovingAtY();
//       HAL_Delay(50);
//     TraceInfo_t *ptr = Sensor_GetCurrentInfo();
// #define ISHIGH(x, n) (!!(x & (1 << n)))

//     printf("%x,%x,%x,%x\r\n", ptr[0], ptr[1], ptr[2], ptr[3]);
//     printf(" %d%d%d%d%d%d%d%d%d\r\n", ISHIGH(ptr[0], 8), ISHIGH(ptr[0], 7), ISHIGH(ptr[0], 6), ISHIGH(ptr[0], 5), ISHIGH(ptr[0], 4), ISHIGH(ptr[0], 3), ISHIGH(ptr[0], 2), ISHIGH(ptr[0], 1), ISHIGH(ptr[0], 0));
//     printf("%d                    %d\r\n", ISHIGH(ptr[1], 10), ISHIGH(ptr[2], 10));
//     printf("%d                    %d\r\n", ISHIGH(ptr[1], 9), ISHIGH(ptr[2], 9));
//     printf("%d                    %d\r\n", ISHIGH(ptr[1], 8), ISHIGH(ptr[2], 8));
//     printf("%d                    %d\r\n", ISHIGH(ptr[1], 7), ISHIGH(ptr[2], 7));
//     printf("%d                    %d\r\n", ISHIGH(ptr[1], 6), ISHIGH(ptr[2], 6));
//     printf("%d                    %d\r\n", ISHIGH(ptr[1], 5), ISHIGH(ptr[2], 5));
//     printf("%d                    %d\r\n", ISHIGH(ptr[1], 4), ISHIGH(ptr[2], 4));
//     printf("%d                    %d\r\n", ISHIGH(ptr[1], 3), ISHIGH(ptr[2], 3));
//     printf("%d                    %d\r\n", ISHIGH(ptr[1], 2), ISHIGH(ptr[2], 2));
//     printf("%d                    %d\r\n", ISHIGH(ptr[1], 1), ISHIGH(ptr[2], 1));
//     printf("%d                    %d\r\n", ISHIGH(ptr[1], 0), ISHIGH(ptr[2], 0));
//     printf(" %d%d%d%d%d%d%d%d%d\r\n", ISHIGH(ptr[3], 8), ISHIGH(ptr[3], 7), ISHIGH(ptr[3], 6), ISHIGH(ptr[3], 5), ISHIGH(ptr[3], 4), ISHIGH(ptr[3], 3), ISHIGH(ptr[3], 2), ISHIGH(ptr[3], 1), ISHIGH(ptr[3], 0));
//     }
//   }

  for (;;) {
    Motor_SetY(30);
    for (;;) {
        TraceInfo_t *ptr = Sensor_GetCurrentInfo();
        uint8_t beg, end;
        Position_GetOneActive(ptr[0], 9, &beg, &end);
        if (count_bits(ptr[0]) >= 5) {
            HAL_Delay(500);
            Motor_SetY(0);
        }
        Motion_CorrectWhenMovingAtY();
        HAL_Delay(20);
    }
  }
  Steer_Init();
  ARM_Forward_TakeBall();
  ARM_Backward_TakeBall();
  ARM_Forward_TakeBall();
  ARM_Backward_TakeBall();
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
    Motor_Decode(Motor_X, Motor_Y, Motor_W);
    HAL_Delay(20);
  }
  for (;;) {
    Motion_MoveToLeft(15);
    while (1) {
      Motion_CorrectWhenMovingAtX();
      // Motor_Decode(20, 4, 0);
      static int cnt = 0;
      
      HAL_Delay(100);
      cnt = (cnt + 1) % 10;
#define ISHIGH(x, n) (!!(x & (1 << n)))

      if (!cnt) {
          TraceInfo_t *ptr = Sensor_GetCurrentInfo();
          printf("X:%dY%dW%d\r\n", Motor_InputInstance.x, Motor_InputInstance.y, Motor_InputInstance.w);
          printf("%x,%x,%x,%x\r\n", ptr[0], ptr[1], ptr[2], ptr[3]);
          printf(" %d%d%d%d%d%d%d%d%d\r\n", ISHIGH(ptr[0], 8), ISHIGH(ptr[0], 7), ISHIGH(ptr[0], 6), ISHIGH(ptr[0], 5), ISHIGH(ptr[0], 4), ISHIGH(ptr[0], 3), ISHIGH(ptr[0], 2), ISHIGH(ptr[0], 1), ISHIGH(ptr[0], 0));
          printf("%d           %d\r\n", ISHIGH(ptr[1], 10), ISHIGH(ptr[2], 10));
          printf("%d           %d\r\n", ISHIGH(ptr[1], 9), ISHIGH(ptr[2], 9));
          printf("%d           %d\r\n", ISHIGH(ptr[1], 8), ISHIGH(ptr[2], 8));
          printf("%d           %d\r\n", ISHIGH(ptr[1], 7), ISHIGH(ptr[2], 7));
          printf("%d           %d\r\n", ISHIGH(ptr[1], 6), ISHIGH(ptr[2], 6));
          printf("%d           %d\r\n", ISHIGH(ptr[1], 5), ISHIGH(ptr[2], 5));
          printf("%d           %d\r\n", ISHIGH(ptr[1], 4), ISHIGH(ptr[2], 4));
          printf("%d           %d\r\n", ISHIGH(ptr[1], 3), ISHIGH(ptr[2], 3));
          printf("%d           %d\r\n", ISHIGH(ptr[1], 2), ISHIGH(ptr[2], 2));
          printf("%d           %d\r\n", ISHIGH(ptr[1], 1), ISHIGH(ptr[2], 1));
          printf("%d           %d\r\n", ISHIGH(ptr[1], 0), ISHIGH(ptr[2], 0));
          printf(" %d%d%d%d%d%d%d%d%d\r\n", ISHIGH(ptr[3], 8), ISHIGH(ptr[3], 7), ISHIGH(ptr[3], 6), ISHIGH(ptr[3], 5), ISHIGH(ptr[3], 4), ISHIGH(ptr[3], 3), ISHIGH(ptr[3], 2), ISHIGH(ptr[3], 1), ISHIGH(ptr[3], 0));
      }
    }
  }
  for(;;) {
    /*CODE*/
    // Motor_SetW(MOTION_HIGH_SPEED * 10);
    // Motor_Decode(MOTION_LOW_SPEED, 0, 100);
    Motor_Decode(Motor_X, Motor_Y, Motor_W);
    HAL_Delay(20);
    // Motor_Decode(0, 0, 0);
    // HAL_Delay(5000);
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
    printf("\r\nA%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[0] * ((Motor_InformationInstance.Directions[0] == Motor_CW) ? 1 : -1));
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


  for(;;) {
    HAL_Delay(1000);
    printf("A%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[0] * ((Motor_InformationInstance.Directions[0] == Motor_CW) ? 1 : -1));
    printf("B%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[1] * ((Motor_InformationInstance.Directions[1] == Motor_CW) ? 1 : -1));
    printf("C%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[2] * ((Motor_InformationInstance.Directions[2] == Motor_CW) ? 1 : -1));
    printf("D%d\r\n", 32000 / Motor_InformationInstance.TimeTicks[3] * ((Motor_InformationInstance.Directions[3] == Motor_CW) ? 1 : -1));
    TraceInfo_t *ptr = Sensor_GetCurrentInfo();
#define ISHIGH(x, n) (!!(x & (1 << n)))

    printf("%x,%x,%x,%x\r\n", ptr[0], ptr[1], ptr[2], ptr[3]);
    printf(" %d%d%d%d%d%d%d%d%d\r\n", ISHIGH(ptr[0], 8), ISHIGH(ptr[0], 7), ISHIGH(ptr[0], 6), ISHIGH(ptr[0], 5), ISHIGH(ptr[0], 4), ISHIGH(ptr[0], 3), ISHIGH(ptr[0], 2), ISHIGH(ptr[0], 1), ISHIGH(ptr[0], 0));
    printf("%d                    %d\r\n", ISHIGH(ptr[1], 10), ISHIGH(ptr[2], 10));
    printf("%d                    %d\r\n", ISHIGH(ptr[1], 9), ISHIGH(ptr[2], 9));
    printf("%d                    %d\r\n", ISHIGH(ptr[1], 8), ISHIGH(ptr[2], 8));
    printf("%d                    %d\r\n", ISHIGH(ptr[1], 7), ISHIGH(ptr[2], 7));
    printf("%d                    %d\r\n", ISHIGH(ptr[1], 6), ISHIGH(ptr[2], 6));
    printf("%d                    %d\r\n", ISHIGH(ptr[1], 5), ISHIGH(ptr[2], 5));
    printf("%d                    %d\r\n", ISHIGH(ptr[1], 4), ISHIGH(ptr[2], 4));
    printf("%d                    %d\r\n", ISHIGH(ptr[1], 3), ISHIGH(ptr[2], 3));
    printf("%d                    %d\r\n", ISHIGH(ptr[1], 2), ISHIGH(ptr[2], 2));
    printf("%d                    %d\r\n", ISHIGH(ptr[1], 1), ISHIGH(ptr[2], 1));
    printf("%d                    %d\r\n", ISHIGH(ptr[1], 0), ISHIGH(ptr[2], 0));
    printf(" %d%d%d%d%d%d%d%d%d\r\n", ISHIGH(ptr[3], 8), ISHIGH(ptr[3], 7), ISHIGH(ptr[3], 6), ISHIGH(ptr[3], 5), ISHIGH(ptr[3], 4), ISHIGH(ptr[3], 3), ISHIGH(ptr[3], 2), ISHIGH(ptr[3], 1), ISHIGH(ptr[3], 0));

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

    if (!cnt) {
        MotorCtrl_CalculateNextOutput();
        MotorCtrl_UpdateControlFlow();
        // Motion_UpdateTargetsInVelocity();
        // Motion_PIDUpdateHighFrequency();
    }
    if (!(cnt % 20)) {
      // MotorFeedback_TimeCallback();
    }
    cnt = (cnt + 1) % 4;
}
