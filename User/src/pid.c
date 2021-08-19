/**
 * @file pid.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief PID 的C语言面向对象实践
 *    未测试！！！
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "pid.h"
#include "stdio.h"

/**
 * @brief PID参量结构默认初始化，全为零。
 * 
 * @param handle 类型 PID_InformationTypeDef 
 *         内容 PID handle
 * @return None 
 */
void PID_InformationInit(PID_InformationTypeDef *handle) {
  handle->Current = 0;
  handle->Target = 0;

  handle->Ki = 0;
  handle->Kp = 0;
  handle->Kd = 0;

  handle->Previous = 0;
  handle->prePrevious = 0;

  handle->Integral = 0;
  handle->Derivative = 0;

  handle->Output = 0;
}

/**
 * @brief 位置式PID计算
 * 
 * @param handle  类型 PID_InformationTypeDef 
 *                内容 PID handle
 * @param current 类型 double
 *                内容 当前参量
 * @return None 
 */
void PID_Calculate_Locational(PID_InformationTypeDef *handle, double current) {
  // 计算
  double error = handle->Target - current;
  double lastError = handle->Current - handle->Previous;

  // 求值
  handle->Integral += current;
  handle->Derivative = error - lastError;

  // 输出
  handle->Output = handle->Kp * error + handle->Ki * handle->Integral +
                   handle->Kd * handle->Derivative;

  // 迭代
  handle->Previous = handle->Current;
  handle->Current = current;
  // printf("%f\r\n", handle->Output);
}

/**
 * @brief 位置式PID计算，带基本的积分溢出处理。
 * 
 * @param handle  类型 PID_InformationTypeDef 
 *                内容 PID handle
 * @param current 类型 double
 *                内容 当前参量
 * @param IntegralMinimum 类型 double
 *                        内容 积分下限
 * @param IntegralMaximum 类型 double
 *                        内容 积分上限
 * @return None 
 */
void PID_Calculate_Locational_CounterOverflow(
  PID_InformationTypeDef *handle, double current, double IntegralMinimum,
  double IntegralMaximum) {
  // 计算
  double error = handle->Target - current;
  double lastError = handle->Current - handle->Previous;

  // 求值
  handle->Integral += current;
  handle->Derivative = error - lastError;

  // 积分溢出处理
  if (handle->Integral > IntegralMaximum) handle->Integral = IntegralMaximum;
  if (handle->Integral < IntegralMinimum) handle->Integral = IntegralMinimum;

  // 输出
  handle->Output = handle->Kp * error + handle->Ki * handle->Integral +
                   handle->Kd * handle->Derivative;

  // 迭代
  handle->Previous = handle->Current;
  handle->Current = current;
}
/**
 * @brief 增量式PID计算
 * 
 * @param handle  类型 PID_InformationTypeDef 
 *                内容 PID handle
 * @param current 类型 double
 *                内容 当前参量
 * @return None 
 */
void PID_Calculate_Incremental(PID_InformationTypeDef *handle,
                                               double current) {
  // 计算
  double error = handle->Target - current;

  // 求值
  handle->Integral = error;
  handle->Derivative = error - 2 * handle->Previous + handle->prePrevious;

  // 输出
  handle->Output +=
    handle->Kp * (error - handle->Previous) +
    handle->Derivative * handle->Kd + handle->Integral * handle->Ki;

  // 迭代
  handle->prePrevious = handle->Previous;
  handle->Previous = error;
  handle->Current = current;
}
