/**
 * @file motor_ctrl.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 电机控制
 *    PWM引脚：PA6 PA7 PB0 PB1
 *    使用TIM3
 *    EN引脚：PF0， PF1
 *           PF2， PF3
 *           PF4， PF5
 *           PF11，PF12
 *    TIM3 设置： Prescale 72 - 1, Autoreload 1000 - 1.
 * 
 *    
 * 
 *    注意：使用电机的TimeTick作为控制量，所以，控制量和输出量不是正相关关系。
 *        可以将输出量作反向处理。
 *        所以！ PWM极性为 GND！！！
 * 
 *    使用方法：
 *    手动设定方向和速度(TimeTick)的值，然后周期调用Calculate 和 Update.
 *    建议新开个计时器，如果其他部件不需要计时器的话。
 *    也可以使用freeRTOS的软件计时器，具体的可以在motor.c里实现呐。
 *    需要周期进行的工作：
 *    - PID计算，求得新的占空比
 *    - 设置新的占空比
 * 
 * 
 *    数据类型：
 *    feedback -> time tick
 *    PID -> duty cycle (signed)
 *    output -> modified output as duty cycle(unsigned)
 * @version 0.1
 * @date 2021-07-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "motor_ctrl.h"
#include "motor_feedback.h"


PID_InformationTypeDef Motor_PID_Speed[4];
MotorSpeed_t Motor_TargetSpeed[4];

#if 1
#define MOTOR_LOCATIONAL
#else
#define MOTOR_INCREMENTAL
#endif



/**
 * @brief 根据新的目标值计算下一个PID
 * 
 * @param PIDs PID数据类型。 
 * @param target 目标，数组类型。
 */
void MotorCtrl_CalculateNextOutputByTargets(PID_InformationTypeDef *PIDs,
                                        MotorSpeed_t *target) {
  MotorSpeed_t *Info = MotorCtrl_UpdateFeedback(MotorFeedback_GetInformation());
  for (int i = 0; i < 4; ++i) {
    PID_SetTarget(PIDs + i, target[i]);
#ifdef MOTOR_LOCATIONAL
    PID_Calculate_Locational(PIDs + i, Info[i]);
#elif MOTOR_INCREMENTAL
    PID_Calculate_Incremental(PIDs + i, Info[i]);
#endif
  }
}

/**
 * @brief 设置电机旋转方向
 * 
 * 警告：注意程序耗时！！有Delay的话，需要小心
 * 
 * @param Motor 电机编号，取值MotorOrdinal_t
 * @param direction 电机方向，取值MotorDirection_t
 * @return None 
 */
 void MotorCtrl_SetDirection(MotorOrdinal_t Motor,
                                                 MotorDirection_t direction) {
  switch (Motor_Encode(Motor, direction)) {
    case Motor_Encode(Motor_A, Motor_CW):
      Motor_OutAN_GPIO_Port->IDR &= (~Motor_OutAN_Pin);
      // HAL_Delay(1);
      Motor_OutAP_GPIO_Port->IDR &= Motor_OutAP_Pin;
      break;
    case Motor_Encode(Motor_A, Motor_CCW):
      Motor_OutAP_GPIO_Port->IDR &= (~Motor_OutAP_Pin);
      // HAL_Delay(1);
      Motor_OutAN_GPIO_Port->IDR &= Motor_OutAN_Pin;
      break;
    case Motor_Encode(Motor_B, Motor_CW):
      Motor_OutBN_GPIO_Port->IDR &= (~Motor_OutBN_Pin);
      // HAL_Delay(1);
      Motor_OutBP_GPIO_Port->IDR &= Motor_OutBP_Pin;
      break;
    case Motor_Encode(Motor_B, Motor_CCW):
      Motor_OutBP_GPIO_Port->IDR &= (~Motor_OutBP_Pin);
      // HAL_Delay(1);
      Motor_OutBN_GPIO_Port->IDR &= Motor_OutBN_Pin;
      break;
    case Motor_Encode(Motor_C, Motor_CW):
      Motor_OutCN_GPIO_Port->IDR &= (~Motor_OutCN_Pin);
      // HAL_Delay(1);
      Motor_OutCP_GPIO_Port->IDR &= Motor_OutCP_Pin;
      break;
    case Motor_Encode(Motor_C, Motor_CCW):
      Motor_OutCP_GPIO_Port->IDR &= (~Motor_OutCP_Pin);
      // HAL_Delay(1);
      Motor_OutCN_GPIO_Port->IDR &= Motor_OutCN_Pin;
      break;
    case Motor_Encode(Motor_D, Motor_CW):
      Motor_OutDN_GPIO_Port->IDR &= (~Motor_OutDN_Pin);
      // HAL_Delay(1);
      Motor_OutDP_GPIO_Port->IDR &= Motor_OutDP_Pin;
      break;
    case Motor_Encode(Motor_D, Motor_CCW):
      Motor_OutDP_GPIO_Port->IDR &= (~Motor_OutDP_Pin);
      // HAL_Delay(1);
      Motor_OutDN_GPIO_Port->IDR &= Motor_OutDN_Pin;
      break;
    default:
      /*TODO*/
      break;
  }
}

/**
 * @brief 更新PWM占空比
 *      注意：正负代表方向
 * TODO : 何时调用？是不是需要一个定时器？
 * @param None
 * @retval None
 */
void MotorCtrl_UpdateControlFlow(void) {
  if (Motor_PID_Speed[0].Output >= 0) {
    MotorCtrl_SetDirection(Motor_A, Motor_CW);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
                        Motor_OutputFix(Motor_PID_Speed[0].Output));
  } else {
    MotorCtrl_SetDirection(Motor_A, Motor_CCW);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
                        Motor_OutputFix(-Motor_PID_Speed[0].Output));
  }
  if (Motor_PID_Speed[1].Output >= 0) {
    MotorCtrl_SetDirection(Motor_B, Motor_CW);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
                        Motor_OutputFix(Motor_PID_Speed[1].Output));
  } else {
    MotorCtrl_SetDirection(Motor_B, Motor_CCW);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
                        Motor_OutputFix(-Motor_PID_Speed[1].Output));
  }
  if (Motor_PID_Speed[2].Output >= 0) {
    MotorCtrl_SetDirection(Motor_C, Motor_CW);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
                        Motor_OutputFix(Motor_PID_Speed[2].Output));
  } else {
    MotorCtrl_SetDirection(Motor_C, Motor_CCW);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
                        Motor_OutputFix(-Motor_PID_Speed[2].Output));
  }
  if (Motor_PID_Speed[3].Output >= 0) {
    MotorCtrl_SetDirection(Motor_D, Motor_CW);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
                        Motor_OutputFix(Motor_PID_Speed[3].Output));
  } else {
    MotorCtrl_SetDirection(Motor_D, Motor_CCW);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
                        Motor_OutputFix(-Motor_PID_Speed[3].Output));
  }
}


/**
 * @brief 反馈更新，将feedback更新成符合PID的数据类型
 * 
 * @param info 即MotorFeedback_InformationTypeDef.
 * @return MotorSpeed_t* 当前速度指针，类型是signed的
 */
MotorSpeed_t *MotorCtrl_UpdateFeedback(MotorFeedback_InformationTypeDef *info) {
  // 终于忍不住使用了静态变量（
  static MotorSpeed_t motorSpeed_data[4];
  for (int i = 0; i < 4; ++i) {
    if (info->Directions[i] == MotorFeedback_CW) {
      motorSpeed_data[i] = Motor_FeedbackFix(info->TimeTicks[i]);
    } else {
      motorSpeed_data[i] = -Motor_FeedbackFix(info->TimeTicks[i]);
    }
  }
  return motorSpeed_data;
}
