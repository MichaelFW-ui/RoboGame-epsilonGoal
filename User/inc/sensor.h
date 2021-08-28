/**
 * @file sensor.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 所有传感器API
 * @version 0.1
 * @date 2021-08-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __SENSOR_H
#define __SENSOR_H

#include "stm32f1xx_hal.h"
#include "motion.h"
#include "position.h"
#include "motor.h"

extern TraceInfo_t CurrentTrace[4];
#define NULL_VALUE 0

__STATIC_FORCEINLINE MotorInput_t Sensor_GetCurrentAngle(void) {
    /*TODO*/
    return NULL_VALUE;
}

__STATIC_FORCEINLINE MotorInput_t Sensor_GetCurrentAngularVelocity(void) {
    /*TODO*/
    return NULL_VALUE;
}



__STATIC_INLINE TraceInfo_t* Sensor_GetCurrentInfo(void) {
    /*TODO*/
    for (int i = 0; i < 10 * 4; ++i) {
        /*TODO*/
    }
    return CurrentTrace;
}

__STATIC_INLINE uint16_t Sensor_GetLeftDistance(void) {
    /*TODO*/
    return NULL_VALUE;
}

#endif // !__SENSOR_H
