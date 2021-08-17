/**
 * @file motor.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 根据编码器信号和Target速度和角速度，利用PID计算并调整电机速度。
 * 
 * 
 *                          ^  y axis
 *  Vehicle Motor Number    |
 *                          |
 *           1 |-----| 2    |
 *               |||        |
 *               |||        |
 *               |||        |
 *           3 |-----| 4    |
 * <------------------------+
 * x axis
 * 
 * 
 * CW means the direction that moves the vehicle along y axis.
 * w+ is defined to clockwise.
 * 
 * Then we have:
 *      1       2       3       4
 * y+   CW      CW      CW      CW
 * x+   CCW     CCW     CW      CW
 * w+   CW      CCW     CW      CCW
 * 
 * Notice! Y and x  axises is fixed to the vehicle.
 * 
 * @version 0.1
 * @date 2021-08-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "motor.h"




MotorInputTpyeDef Motor_InputInstance;

// Externed from motor_ctrl.c
extern MotorSpeed_t Motor_TargetSpeed[4];

void Motor_Decode(MotorInput_t x, MotorInput_t y, MotorInput_t w) {
    // static MotorSpeed_t Motor_TargetSpeed[4] = {0};

    // 保存速度结果，便于调试运动状态
    Motor_InputInstance.x = x;
    Motor_InputInstance.y = y;
    Motor_InputInstance.w = w;

    Motor_TargetSpeed[0] = MotorDecodeOutputFix(Motor_InputInstance.Kx * x -
                                                Motor_InputInstance.Ky * y +
                                                Motor_InputInstance.Kw * w);
    Motor_TargetSpeed[1] = MotorDecodeOutputFix(Motor_InputInstance.Kx * x -
                                                Motor_InputInstance.Ky * y -
                                                Motor_InputInstance.Kw * w);
    Motor_TargetSpeed[2] = MotorDecodeOutputFix(Motor_InputInstance.Kx * x +
                                                Motor_InputInstance.Ky * y +
                                                Motor_InputInstance.Kw * w);
    Motor_TargetSpeed[3] = MotorDecodeOutputFix(Motor_InputInstance.Kx * x +
                                                Motor_InputInstance.Ky * y -
                                                Motor_InputInstance.Kw * w);
}

