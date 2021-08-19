/**
 * @file motion.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 底盘电机的控制，利用了PID
 * @version 0.1
 * @date 2021-08-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __MOTION_H
#define __MOTION_H

#include "motor.h"
#include "pid.h"
#include "sensor.h"
#include "stm32f1xx_hal.h"

extern PID_InformationTypeDef x_speed;
extern PID_InformationTypeDef y_speed;
extern PID_InformationTypeDef w_speed;
extern PID_InformationTypeDef w_angle;

extern volatile MotorInput_t Motion_TargetAtX, Motion_TargetAtY, Motion_TargetAngle;

/*                              API                                           */
/*                              提供两种不同的形式                                */

/**
 * @brief 
 * 
 * @param x Target，目标x方向速度。类型为MotorInput_t。
 * @param y Target，目标y方向速度。类型为MotorInput_t。
 * @param w_angle Target，目标角度。类型为MotorInput_t。
 * @return None 
 */
__STATIC_FORCEINLINE void Motion_SetTargetArguments(MotorInput_t x, MotorInput_t y, MotorInput_t w_angle) {
    Motion_TargetAtX = x;
    Motion_TargetAtY = y;
    Motion_TargetAngle = w_angle;
}

void Motion_UpdateTargetsInVelocity(void);


/**/

void Motion_PIDUpdateHighFrequency(void);

void Motion_PIDUpdateLowFrequency(void);

void Motion_PIDInit(void);

#endif // !__MOTION_H
