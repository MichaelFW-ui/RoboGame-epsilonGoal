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


#define MotorDncodeOutputFix(input) (input / 20)

void Motor_Decode(MotorInput_t x, MotorInput_t y, MotorInput_t w);




#endif // !__MOTOR_H