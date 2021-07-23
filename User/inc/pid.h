/**
 * @file pid.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief PID实现的头文件
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "stm32f1xx_hal.h"

/*             PID TypeDef Begin                                              */

typedef struct {
  double Ki;              // Ki
  double Kp;              // Kp
  double Kd;              // Kd
  double Integral;        // 积分累计
  double Derivative;      // 微分项
  double Target;          // 目标值
  double Current;         // 当前值
  double Previous;        // 上一值
  double Output;          // 输出值
  double prePrevious;     // 上上一值
} PID_InformationTypeDef;

/*
*     不同函数中上结构体的具体含义不同，再次强调：
*     位置式PID中，有
*         Current 当前参量
*         Previous 先前参量
*         prePrevious 不使用
*     增量式PID中，有
*         Current 当前参量
*         Previous 先前误差
*         prePrevious 先先前误差*
*
*
*/

/*             PID TypeDef End                                                */

__STATIC_INLINE void PID_InformationInit(PID_InformationTypeDef *handle);

__STATIC_INLINE void PID_Calculate_Locational(PID_InformationTypeDef *handle,
                                              double current);

__STATIC_INLINE void PID_Calculate_Incremental(PID_InformationTypeDef *handle,
                                               double current);

__STATIC_INLINE void PID_SetTarget(PID_InformationTypeDef *handle,
                                   double target) {
  handle->Target = target;
}
