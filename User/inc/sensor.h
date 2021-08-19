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

#define NULL_VALUE 0

__STATIC_FORCEINLINE MotorInput_t Sensor_GetCurrentAngle(void) {
    /*TODO*/
    return NULL_VALUE;
}

__STATIC_FORCEINLINE MotorInput_t Sensor_GetCurrentAngularVelocity(void) {
    /*TODO*/
    return NULL_VALUE;
}

__STATIC_FORCEINLINE MotorInput_t Sensor_GetCurrentSpeedAtX(void) {
    /*TODO*/
    return NULL_VALUE;
}


__STATIC_FORCEINLINE MotorInput_t Sensor_GetCurrentSpeedAtY(void) {
    /*TODO*/
    return NULL_VALUE;
}

void Sensor_GetCurrentInfo(TraceInfo_t *info) {
    /*TODO*/
    for (int i = 0; i < 10 * 4; ++i) {
        /*TODO*/
        *(info + i) = 0;
    }
    return (void)HAL_ERROR;
}

#endif // !__SENSOR_H
