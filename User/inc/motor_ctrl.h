
/**
 * @file motor_ctrl.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 电机控制头文件
 * @version 0.1
 * @date 2021-07-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __MOTOR_CTRL_H
#define __MOTOR_CTRL_H

#include "stm32f1xx_hal.h"
#include "tim.h"
#include "pid.h"
#include "motor_feedback.h"
#include "stdio.h"

/*      Begin of Motor control typedef                                        */
typedef enum {
  Motor_A = 0,
  Motor_B = 1,
  Motor_C = 2,
  Motor_D = 3
} MotorOrdinal_t;

typedef enum {
  Motor_CW = 0, Motor_CCW = 1
} MotorDirection_t;

typedef enum {
  Motor_PID_P = 0,
  Motor_PID_I = 1,
  Motor_PID_D = 2
} MotorPIDTypeDef;

typedef int16_t MotorSpeed_t;


/*        End of Motor control typedef                                        */


extern PID_InformationTypeDef Motor_PID_Speed[4];
extern MotorSpeed_t Motor_TargetSpeed[4];

/*              Preoperation                                                  */
/**
 * @brief 对电机方向进行编码，便于程序书写和理解
 * @param Motor 电机编码，取值MotorOrdinal_t
 * @param Direction 方向，取值MotorDirection_t
 * @retval 编码值
 */
#define Motor_Encode(Motor, Direction) (uint8_t)((Motor << 1) | Direction)

#define M(output) (output * 4)

/**
 * @brief 修正output用于PWM占空比设置
 *      警告：随时修改
 * @param output 原始PID计算的output
 * @retval 修正后的PWM占空比
 */
#define Motor_OutputFix(output)                                      \
    (((uint16_t)(M(output)) < 800)                                   \
         ? (((uint16_t)(M(output)) > 0) ? (uint16_t)(M(output)) : 0) \
         : 800)

/**
 * @brief 修正反馈值用于PID计算
 *      警告：随时修改
 * @param feedback 原始反馈值
 * @retval 修正后的可用于PID的占空比
 */
#define Motor_FeedbackFix(feedback) ( 32000 / (int32_t)((feedback)))
/*              Preoperation ends                                             */


/**
 * @brief 设置电机的占空比
 * 
 * @param Motor 电机编号，类型为MotorOrdinal_t
 * @param DutyCycle 占空比，取值范围为[0, AutoReload]
 * @return None 
 */
__STATIC_FORCEINLINE void MotorCtrl_SetDutyCycle(MotorOrdinal_t Motor,
                                                 uint16_t DutyCycle) {
  switch (Motor) {
    case Motor_A:
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, DutyCycle);
      break;
    case Motor_B:
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, DutyCycle);
      break;
    case Motor_C:
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, DutyCycle);
      break;
    case Motor_D:
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, DutyCycle);
      break;
    default:
      /*TODO*/
      break;
  }
}

/**
 * @brief 电机控制初始化
 * 
 * @return None 
 */
__STATIC_INLINE void MotorCtrl_Init(void) {
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  HAL_TIM_Base_Start_IT(&htim4);
  for (int i = 0; i < 4; ++i) {
    PID_InformationInit(&Motor_PID_Speed[i]);
  }
  /*TODO*/
  /*
  *   这里放置PID的调试后的参数
  */

  Motor_PID_Speed[0].Kp = 1;
  Motor_PID_Speed[0].Ki = 0.3;
  Motor_PID_Speed[0].Kd = 0;
  Motor_PID_Speed[1].Kp = 1;
  Motor_PID_Speed[1].Ki = 0.3;
  Motor_PID_Speed[1].Kd = 0;
  Motor_PID_Speed[2].Kp = 1;
  Motor_PID_Speed[2].Ki = 0.3;
  Motor_PID_Speed[2].Kd = 0;
  Motor_PID_Speed[3].Kp = 1;
  Motor_PID_Speed[3].Ki = 0.3;
  Motor_PID_Speed[3].Kd = 0;
}

void MotorCtrl_CalculateNextOutputByTargets(PID_InformationTypeDef *PIDs,
                                        MotorSpeed_t *target);

void MotorCtrl_SetDirection(MotorOrdinal_t Motor, MotorDirection_t direction);

/**
 * @brief 单纯地计算下一个PID
 *
 * @param PIDs PID数据类型
 */
void __STATIC_INLINE MotorCtrl_CalculateNextOutput(void) {
  MotorCtrl_CalculateNextOutputByTargets(Motor_PID_Speed, Motor_TargetSpeed);
}

void __STATIC_INLINE MotorCtrl_SetPIDArguments(MotorOrdinal_t motor,
                                               MotorPIDTypeDef pid,
                                               double val) {
  switch (pid) {
    case Motor_PID_P:
      Motor_PID_Speed[motor].Kp = val;
      break;
    case Motor_PID_I:
      Motor_PID_Speed[motor].Ki = val;
      break;
    case Motor_PID_D:
      Motor_PID_Speed[motor].Kd = val;
      break;
    default:
      /*TODO*/
      break;
  }
}

double __STATIC_INLINE MotorCtrl_GetPIDArguments(MotorOrdinal_t motor,
                                                 MotorPIDTypeDef pid) {
  switch (pid) {
    case Motor_PID_P:
      return Motor_PID_Speed[motor].Kp;
    case Motor_PID_I:
      return Motor_PID_Speed[motor].Ki;
    case Motor_PID_D:
      return Motor_PID_Speed[motor].Kd;
    default:
      /*TODO*/
      return 0;
  }
}

void __STATIC_INLINE MotorCtrl_PrintArguments(void) {
  printf("Outputs:\r\n");
  printf("%f -> %d\r\n", Motor_PID_Speed[0].Output, ((Motor_PID_Speed[0].Output > 0) ? (Motor_OutputFix(Motor_PID_Speed[0].Output)) : (Motor_OutputFix(-Motor_PID_Speed[0].Output))));
  printf("%f -> %d\r\n", Motor_PID_Speed[1].Output, ((Motor_PID_Speed[1].Output > 0) ? (Motor_OutputFix(Motor_PID_Speed[1].Output)) : (Motor_OutputFix(-Motor_PID_Speed[1].Output))));
  printf("%f -> %d\r\n", Motor_PID_Speed[2].Output, ((Motor_PID_Speed[2].Output > 0) ? (Motor_OutputFix(Motor_PID_Speed[2].Output)) : (Motor_OutputFix(-Motor_PID_Speed[2].Output))));
  printf("%f -> %d\r\n", Motor_PID_Speed[3].Output, ((Motor_PID_Speed[3].Output > 0) ? (Motor_OutputFix(Motor_PID_Speed[3].Output)) : (Motor_OutputFix(-Motor_PID_Speed[3].Output))));
  printf("speed %f\r\n", Motor_PID_Speed[0].Current);
  printf("speed %f\r\n", Motor_PID_Speed[1].Current);
  printf("speed %f\r\n", Motor_PID_Speed[2].Current);
  printf("speed %f\r\n", Motor_PID_Speed[3].Current);
}

/**
 * @brief 唯一指定的接口
 * 
 * @param speed 
 * @param ord 
 */
void __STATIC_INLINE MotorCtrl_SetTarget(MotorSpeed_t speed, uint8_t ord) {
  Motor_TargetSpeed[ord] = speed;
}

void MotorCtrl_UpdateControlFlow(void);

MotorSpeed_t *MotorCtrl_UpdateFeedback(MotorFeedback_InformationTypeDef *info);


#endif        // !__MOTOR_CTRL_H
