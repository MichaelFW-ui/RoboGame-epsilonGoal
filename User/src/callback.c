/**
 * @file callback.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 回调函数集中处理处
 * HAL_TIM_PeriodElapsedCallback在 main.c中，不能写在此处。
 * @version 0.1
 * @date 2021-07-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "callback.h"
#include "motor_feedback.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) {
    MotorFeedback_IC_CaptureCallback(htim);
  }
}




