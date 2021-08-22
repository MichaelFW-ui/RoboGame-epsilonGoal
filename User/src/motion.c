/**
 * @file motion.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 底盘电机的控制，利用了PID
 * 速度环：
 * 底盘电机控制了三个参数，x速度，y速度，w速度。
 * 要求控制的是x,y速度和w角度。
 * 
 * PID内容：
 * X：
 * 反馈X速度的偏差，改变X方向的出力，即底盘出力速度，反馈调节。
 * Y：
 * 反馈Y速度的偏差，改变Y方向的出力，即底盘出力速度，反馈调节。
 * W：
 * 反馈W角度的偏差，改变W方向的出力，即底盘出力速度，反馈调节底盘w速度，即底盘实际w速度，从而
 * 调节w角度。
 * 总计4个PID。
 * X一个，Y一个，W两个。
 * 其中三个为100Hz，另一个500Hz。
 * @version 0.1
 * @date 2021-08-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "motion.h"

PID_InformationTypeDef x_speed;
PID_InformationTypeDef y_speed;
PID_InformationTypeDef w_speed;
PID_InformationTypeDef w_angle;
PID_InformationTypeDef x_distance;
PID_InformationTypeDef y_distance;

volatile uint8_t EnableLocationPID = 0;

volatile MotorInput_t Motion_TargetAtX, Motion_TargetAtY, Motion_TargetAngle;

volatile MotorInput_t Motion_TargetAtXDistance;
volatile MotorInput_t Motion_TargetAtYDistance;

/*DO NOT CHANGE THIS VALUE!*/
/*DO NOT CHANGE THIS VALUE!*/
/*DO NOT CHANGE THIS VALUE!*/
volatile MotorInput_t Motion_TargetAngularVelocity;


/**
 * @brief 根据全部变量更新当前的Target
 * 注意：没有做到ThreadSafe!
 * 
 */
void Motion_UpdateTargetInVelocity() {
    w_speed.Target = Motion_TargetAngularVelocity = w_angle.Output;
    w_angle.Target = Motion_TargetAngle;
    if (EnableLocationPID) {
        x_speed.Target = Motion_TargetAtX = x_distance.Output;
        y_speed.Target = Motion_TargetAtY = y_distance.Output;
        x_distance.Target = Motion_TargetAtXDistance;
        y_distance.Target = Motion_TargetAtYDistance;
    } else {
        x_speed.Target = Motion_TargetAtX;
        y_speed.Target = Motion_TargetAtY;
    }
}

/**
 * @brief 高速执行的PID，为底层的PID，要求在上层PID完成之前达到平衡。
 * 
 */
void Motion_PIDUpdateHighFrequency() {
    PID_Calculate_Incremental(&x_speed, Sensor_GetCurrentSpeedAtX());
    PID_Calculate_Incremental(&y_speed, Sensor_GetCurrentSpeedAtY());
    PID_Calculate_Incremental(&w_speed, Sensor_GetCurrentAngularVelocity());
}

/**
 * @brief 上层PID，较底层可以更慢达到平衡。
 * 
 */
void Motion_PIDUpdateLowFrequency() {
    PID_Calculate_Incremental(&w_angle, Sensor_GetCurrentAngle());
    if (EnableLocationPID) {
        // PID_Calculate_Incremental(&x_distance, Sensor_GetCurrentDistanceAtX());
        // PID_Calculate_Incremental(&y_distance, Sensor_GetCurrentDistanceAtY());
    }
}
