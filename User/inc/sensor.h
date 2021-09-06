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
#include "usart.h"

extern TraceInfo_t CurrentTrace[4];
#define NULL_VALUE 0
#define SWAP(a, b) (a ^= b ^= a ^= b)
#define REBUILD(x) (x = ((x & 0x00FF) << 8) | ((x & 0xFF00) >> 8))

// __STATIC_FORCEINLINE MotorInput_t Sensor_GetCurrentAngle(void) {
//     /*TODO*/
//     return NULL_VALUE;
// }

// __STATIC_FORCEINLINE MotorInput_t Sensor_GetCurrentAngularVelocity(void) {
//     /*TODO*/
//     return NULL_VALUE;
// }



__STATIC_INLINE TraceInfo_t* Sensor_GetCurrentInfo(void) {
    uint16_t cmd = 0x8000;
    HAL_StatusTypeDef ret = HAL_UART_Transmit(&huart1, (uint8_t *)&cmd, 2, 0x05ff);
    if (ret != HAL_OK) {
        printf("Not transmitted\r\n");
    }
    ret = HAL_UART_Receive(&huart1, (uint8_t*)CurrentTrace, 4 * (sizeof(TraceInfo_t)), 0x06FF);
    if (ret != HAL_OK) {
        printf("Not received\r\n");
    } else {
        // printf("Received\r\n");
    }
    /*TODO*/
    /*
    SWAP FOR A BETTER ALGINMENT
    */
//    for (int i = 0; i < 4; ++i) {
//        REBUILD(CurrentTrace[i]);
//    }
    return CurrentTrace;
}

__STATIC_INLINE uint16_t Sensor_GetLeftDistance(void) {
    /*TODO*/
    return NULL_VALUE;
}

#endif // !__SENSOR_H
