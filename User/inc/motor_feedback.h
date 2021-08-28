/**
 * @file motor_feedback.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 电机编码器反馈头文件。
 * @version 0.1
 * @date 2021-07-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __MOTOR_FEEDBACK_H
#define __MOTOR_FEEDBACK_H


#include "stm32f1xx_hal.h"
#include "tim.h"

#define MOTOR_FEEDBACK_HANDLE htim2

/*
    ATTENTION - 警告：
    改变TIM参数后必须改变这个参数。
*/
#define MOTOR_TIM_COUNTER_PERIOD 10000



/*           Motor TypeDef Begins                                             */

typedef int32_t MotorFeedback_TimeTick_t;
typedef uint8_t MotorFeedback_ReloadTime_t;
typedef enum {
  MotorFeedback_CW  = 0,
  MotorFeedback_CCW = 1
} MotorFeedback_Direction_t;
typedef struct {
  MotorFeedback_TimeTick_t TimeTicks[4];
  MotorFeedback_ReloadTime_t ReloadTimes[4];
  MotorFeedback_Direction_t Directions[4];
} MotorFeedback_InformationTypeDef;

/*           Motor TypeDef Ends                                               */

extern MotorFeedback_InformationTypeDef Motor_InformationInstance;

/**
 * @brief 计时器达到设定值后引发的回调函数。
 *      在计时器的TIM_PeriodElapsedCallback()函数中调用。
 *      作用：各个电机的计数器加一
 * @param None
 * @return None 
 */
__STATIC_INLINE void MotorFeedback_TIM_PeriodElapsedCallback(void) {
  Motor_InformationInstance.ReloadTimes[0] += 1;
  Motor_InformationInstance.ReloadTimes[1] += 1;
  Motor_InformationInstance.ReloadTimes[2] += 1;
  Motor_InformationInstance.ReloadTimes[3] += 1;
}

/**
 * @brief 初始化函数
 *    事实上的开启四个通道的InputCapture 中断。
 * @param None
 * @return None 
 */
__STATIC_INLINE void MotorFeedback_Init(void) {
  HAL_TIM_IC_Start_IT(&MOTOR_FEEDBACK_HANDLE, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&MOTOR_FEEDBACK_HANDLE, TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&MOTOR_FEEDBACK_HANDLE, TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&MOTOR_FEEDBACK_HANDLE, TIM_CHANNEL_4);
}

/**
 * @brief 临时过程，用于更新通道计数
 *      作用：
 *      根据ReloadTimes和计数器的值计算出实际周期的Tick数。
 * @param arr 第几个通道，取值[0, 3]
 */
#define Temp_GetTicks(arr)                                \
    do {                                                  \
        int32_t cnt = htim->Instance->CNT;                \
        Motor_InformationInstance.TimeTicks[arr] =        \
            Motor_InformationInstance.ReloadTimes[arr] *  \
                MOTOR_TIM_COUNTER_PERIOD +                \
            ((cnt - MotorFeedback_LastTick[arr] > 0)      \
                 ? (cnt - MotorFeedback_LastTick[arr])    \
                 : ((cnt - MotorFeedback_LastTick[arr]) + \
                    MOTOR_TIM_COUNTER_PERIOD));           \
        Motor_InformationInstance.ReloadTimes[arr] = 0;   \
        MotorFeedback_LastTick[arr]                = cnt; \
    } while (0u)

/**
 * @brief 获取电机的反馈信息
 * 
 * @param None
 * @return MotorFeedback_InformationTypeDef *
 */
__STATIC_INLINE MotorFeedback_InformationTypeDef* MotorFeedback_GetInformation(
  void) {
  return &Motor_InformationInstance;
}

void MotorFeedback_IC_CaptureCallback(TIM_HandleTypeDef *htim);

#endif // !__MOTOR_FEEDBACK_H
