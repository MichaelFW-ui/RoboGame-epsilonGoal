/**
 * @file steer_ctrl.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 舵机控制的接口
 *    Warning　& 警告：
 *    因为PWM模块的具体使用方法未知，且舵机对PWM的占空比有严格要求，
 *    上电前使用示波器／逻辑分析仪检查占空比！！！
 *    警告
 *    警告
 *    警告
 * @version 0.1
 * @date 2021-08-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __STEER_CTRL_H
#define __STEER_CTRL_H

#include "pwm_generate.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

/*              Begin of Steer COM numbers                                    */

#define ARM_A_STEER_A (uint8_t)0
#define ARM_A_STEER_B (uint8_t)1
#define ARM_A_STEER_C (uint8_t)2
#define ARM_B_STEER_A (uint8_t)3
#define ARM_B_STEER_B (uint8_t)4
#define ARM_B_STEER_C (uint8_t)5

/*                End of Steer COM numbers                                    */

typedef uint8_t Steer_COMNumber_t;

/*
*           不同舵机对PWM占空比有不同要求。
*/



/**
 * @brief 预设舵机可以转动180度的前提下，根据端口号、角度设定占空比。
 * 
 * @param SteerNumber 端口号
 * @param angle 角度值，[0, 180]
 * @retval None
 */
void __STATIC_FORCEINLINE Steer_SetAngleByDegree(Steer_COMNumber_t SteerNumber,
                                                 int16_t angle) {
  PWM_SetPWM_ByDutyCycle(SteerNumber, (uint16_t)((angle * 2.0 / 180 + 0.5) / 20 * 4096));
}

/**
 * @brief 舵机操作的初始化
 * 
 */
void __STATIC_FORCEINLINE Steer_Init(void) {
  PWM_Reset();
  PWM_SetFrequencyAndStartUp(50);
  // 50Hz下，为1ms占空
  Steer_SetAngleByDegree(ARM_A_STEER_A, 60);
  Steer_SetAngleByDegree(ARM_A_STEER_B, 0);
  Steer_SetAngleByDegree(ARM_A_STEER_C, 50);
  Steer_SetAngleByDegree(ARM_B_STEER_A, 50);
  Steer_SetAngleByDegree(ARM_B_STEER_B, 0);
  Steer_SetAngleByDegree(ARM_B_STEER_C, 50);
  printf("Successfully initialized the Steers\r\n");
}

#endif // !__STEER_CTRL_H
