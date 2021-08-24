/**
 * @file motor.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 根据编码器信号和Target速度和角速度，利用PID计算并调整电机速度。
 * @version 0.1
 * @date 2021-08-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f1xx_hal.h"
#include "motor_ctrl.h"


typedef int32_t MotorInput_t;

typedef struct {
    MotorInput_t x;
    MotorInput_t Kx;
    MotorInput_t y;
    MotorInput_t Ky;
    MotorInput_t w;
    MotorInput_t Kw;
} MotorInputTpyeDef;

extern MotorInputTpyeDef Motor_InputInstance;

#define MotorDecodeOutputFix(input) (input / 20)


__STATIC_INLINE void Motor_Init(void) {
    Motor_InputInstance.Kx = 1;
    Motor_InputInstance.Ky = 1;
    Motor_InputInstance.Kw = 1;
    Motor_InputInstance.x = 0;
    Motor_InputInstance.y = 0;
    Motor_InputInstance.w = 0;
}

void Motor_Decode(MotorInput_t x, MotorInput_t y, MotorInput_t w);

__STATIC_FORCEINLINE void Motor_SetX(MotorInput_t x) {
    Motor_InputInstance.x = x;
    Motor_Decode(Motor_InputInstance.x, Motor_InputInstance.y,
                 Motor_InputInstance.w);
}

__STATIC_FORCEINLINE void Motor_SetY(MotorInput_t y) {
    Motor_InputInstance.y = y;
    Motor_Decode(Motor_InputInstance.x, Motor_InputInstance.y,
                 Motor_InputInstance.w);
}

__STATIC_FORCEINLINE void Motor_SetW(MotorInput_t w) {
    Motor_InputInstance.w = w;
    Motor_Decode(Motor_InputInstance.x, Motor_InputInstance.y,
                 Motor_InputInstance.w);
}

__STATIC_FORCEINLINE void Motor_AddX(MotorInput_t x) {
    Motor_InputInstance.x += x;
    Motor_Decode(Motor_InputInstance.x, Motor_InputInstance.y,
                 Motor_InputInstance.w);
}

__STATIC_FORCEINLINE void Motor_AddY(MotorInput_t y) {
    Motor_InputInstance.y += y;
    Motor_Decode(Motor_InputInstance.x, Motor_InputInstance.y,
                 Motor_InputInstance.w);
}

__STATIC_FORCEINLINE void Motor_AddW(MotorInput_t w) {
    Motor_InputInstance.w += w;
    Motor_Decode(Motor_InputInstance.x, Motor_InputInstance.y,
                 Motor_InputInstance.w);
}



#endif // !__MOTOR_H
