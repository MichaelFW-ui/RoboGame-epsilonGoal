/**
 * @file motor_feedback.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 底盘电机的编码器信号解析
 *    参考TIM2 的分频设置和reload设计计算具体时间
 *    为保证精度，读取的值均为 时间计数
 * @version 0.1
 * @date 2021-07-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "motor_feedback.h"

MotorFeedback_InformationTypeDef Motor_InformationInstance;


/**
 * @brief InputCapture中断的回调函数。
 *      在TIM_IC_CaptureCallback中调用。
 *      作用：检查当前活跃通道，将其计数器的结果计算出来。
 * 
 * @param htim 
 * @return __STATIC_INLINE 
 */
void MotorFeedback_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  static uint16_t MotorFeedback_LastTick[4];

  if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_1) {
    Temp_GetTicks(0);
  }
  if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_2) {
    Temp_GetTicks(1);
  }
  if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_3) {
    Temp_GetTicks(2);
  }
  if (htim->Channel & HAL_TIM_ACTIVE_CHANNEL_4) {
    Temp_GetTicks(3);
  }
}



