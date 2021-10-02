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
#include "cannon.h"
#include "com.h"
#include "debug.h"
#include "motion.h"
#include "motor.h"
#include "motor_ctrl.h"
#include "motor_feedback.h"
#include "position.h"
#include "procedure.h"
#include "pushrod.h"
#include "sensor.h"
#include "stdio.h"
#include "steer_ctrl.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "usart.h"

MotorSpeed_t Motor_X, Motor_Y, Motor_W;

void TEST_MAIN_FUNCTION(void) {

  // Procedure_Default();
  // Procedure_HeadForPickingArea();
  // CurrentNode = Node_7;
  // Procedure_EnterPickingArea();
  // Procedure_ExitPickingArea();
  // Procedure_HeadForThrowingArea();
  // Procedure_StayInThrowingArea();
  Motor_Decode(0, 0, 0);
  while (1) {
    TraceInfo_t *ptr = Sensor_GetCurrentInfo();
    HAL_Delay(100);
  }

  while (1) {
    TraceInfo_t *ptr = Sensor_GetCurrentInfo();

#ifndef ISHIGH
#define ISHIGH(x, n) (!!(x & (1 << n)))
#endif

    printf("Trace information in hex: %x,%x,%x,%x\r\n", ptr[0], ptr[1], ptr[2], ptr[3]);
    printf(" %d %d %d %d %d %d %d %d %d\r\n", ISHIGH(ptr[0], 8), ISHIGH(ptr[0], 7), ISHIGH(ptr[0], 6), ISHIGH(ptr[0], 5), ISHIGH(ptr[0], 4), ISHIGH(ptr[0], 3), ISHIGH(ptr[0], 2), ISHIGH(ptr[0], 1), ISHIGH(ptr[0], 0));
    printf("%d                        %d\r\n", ISHIGH(ptr[1], 10), ISHIGH(ptr[2], 10));
    printf("%d                        %d\r\n", ISHIGH(ptr[1], 9), ISHIGH(ptr[2], 9));
    printf("%d                        %d\r\n", ISHIGH(ptr[1], 8), ISHIGH(ptr[2], 8));
    printf("%d                        %d\r\n", ISHIGH(ptr[1], 7), ISHIGH(ptr[2], 7));
    printf("%d                        %d\r\n", ISHIGH(ptr[1], 6), ISHIGH(ptr[2], 6));
    printf("%d                        %d\r\n", ISHIGH(ptr[1], 5), ISHIGH(ptr[2], 5));
    printf("%d                        %d\r\n", ISHIGH(ptr[1], 4), ISHIGH(ptr[2], 4));
    printf("%d                        %d\r\n", ISHIGH(ptr[1], 3), ISHIGH(ptr[2], 3));
    printf("%d                        %d\r\n", ISHIGH(ptr[1], 2), ISHIGH(ptr[2], 2));
    printf("%d                        %d\r\n", ISHIGH(ptr[1], 1), ISHIGH(ptr[2], 1));
    printf("%d                        %d\r\n", ISHIGH(ptr[1], 0), ISHIGH(ptr[2], 0));
    printf(" %d %d %d %d %d %d %d %d %d\r\n", ISHIGH(ptr[3], 8), ISHIGH(ptr[3], 7), ISHIGH(ptr[3], 6), ISHIGH(ptr[3], 5), ISHIGH(ptr[3], 4), ISHIGH(ptr[3], 3), ISHIGH(ptr[3], 2), ISHIGH(ptr[3], 1), ISHIGH(ptr[3], 0));
    HAL_Delay(500);
  }
}


void Main_(void) {
  Motor_Init();
  Debug_Init();
  Steer_Init();
  HAL_TIM_Base_Start_IT(&htim5);
  HAL_Delay(1000);
  ARM_Forward_TalonClose();
  ARM_Backward_TalonClose();
  // ARM_Forward_PutDown();
  // ARM_Backward_PutDown();

  // CurrentNode = Node_7;
  // Procedure_EnterPickingArea();

  // TEST_MAIN_FUNCTION();

  Procedure_Default();
  Procedure_HeadForPickingArea();
  Procedure_EnterPickingArea();
  Procedure_ExitPickingArea();
  Procedure_HeadForThrowingArea();
  Procedure_StayInThrowingArea();
  TEST_MAIN_FUNCTION();


  Procedure_HeadForPickingAreaSecondly();
  Procedure_EnterPickingAreaSecondly();
  Procedure_ExitPickingArea();
  Procedure_HeadForThrowingArea();
  Procedure_StayInThrowingArea();

  Procedure_HeadForPickingAreaSecondly();
  Procedure_EnterPickingAreaSecondly();
  Procedure_ExitPickingArea();
  Procedure_HeadForThrowingArea();
  Procedure_StayInThrowingArea();

  Procedure_HeadForPickingAreaSecondly();
  Procedure_EnterPickingAreaSecondly();
  Procedure_ExitPickingArea();
  Procedure_HeadForThrowingArea();
  Procedure_StayInThrowingArea();

  printf("Seems to have accomplished the tasks\r\n");

  ARM_BOTH_DANCE();
  HAL_Delay(1000);

  while (1) {
    ;
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
    }
    cnt = (cnt + 1) % 4;
}

