/**
 * @file motor_feedback.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 底盘电机的编码器信号解析
 *    参考TIM2 的分频设置和reload设计计算具体时间
 *    为保证精度，读取的值均为 时间计数
 * 
 *    注释里的警告（Attention或Warning）都是值得注意的。
 *    为了减少代码耦合度，不得已而为之，能力太差。
 * @version 0.1
 * @date 2021-07-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "motor_feedback.h"
#include "motor.h"

MotorFeedback_InformationTypeDef Motor_InformationInstance;


/**
 * @brief InputCapture中断的回调函数。
 *      在TIM_IC_CaptureCallback中调用。
 *      作用：检查当前活跃通道，将其计数器的结果计算出来。
 * 
 * @param htim 
 * @return None 
 */
void MotorFeedback_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  static uint16_t MotorFeedback_LastTick[4];



  if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_1) {
    Temp_GetTicks(0);
    Motor_InformationInstance.Directions[0] =
      ((Motor_InA_GPIO_Port->IDR & Motor_InA_Pin) ? MotorFeedback_CW
                                                  : MotorFeedback_CCW);
    // Motor_UpdateLocation(0, &Motor_InformationInstance);
  }
  if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_2) {
    Temp_GetTicks(1);
    Motor_InformationInstance.Directions[1] =
      ((Motor_InB_GPIO_Port->IDR & Motor_InB_Pin) ? MotorFeedback_CW
                                                  : MotorFeedback_CCW);
    // Motor_UpdateLocation(1, &Motor_InformationInstance);
  }
  if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_3) {
    Temp_GetTicks(2);
    Motor_InformationInstance.Directions[2] =
      ((Motor_InC_GPIO_Port->IDR & Motor_InC_Pin) ? MotorFeedback_CW
                                                  : MotorFeedback_CCW);
    // Motor_UpdateLocation(2, &Motor_InformationInstance);
  }
  if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_4) {
    Temp_GetTicks(3);
    Motor_InformationInstance.Directions[3] =
      ((Motor_InD_GPIO_Port->IDR & Motor_InD_Pin) ? MotorFeedback_CW
                                                  : MotorFeedback_CCW);
    // Motor_UpdateLocation(3, &Motor_InformationInstance);
  }
}
