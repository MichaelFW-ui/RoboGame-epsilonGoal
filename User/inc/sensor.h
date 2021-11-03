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

__STATIC_INLINE void Sensor_RefreshUART(UART_HandleTypeDef *hd) {
    UNUSED(hd);
    MX_USART1_UART_Init();
}



__STATIC_INLINE TraceInfo_t* Sensor_GetCurrentInfo(void) {
    uint16_t cmd = 0x8000;
    HAL_StatusTypeDef ret = HAL_UART_Transmit(&huart1, (uint8_t *)&cmd, 2, 0x004f);
    if (ret != HAL_OK) {
        printf("Failed to transmit 'Get Trace' command\r\n");
        Sensor_RefreshUART(&huart1);
    }
    ret = HAL_UART_Receive(&huart1, (uint8_t*)CurrentTrace, 4 * (sizeof(TraceInfo_t)), 0x004F);
    if (ret != HAL_OK) {
        printf("Failed to receive trace information\r\n");
        Sensor_RefreshUART(&huart1);
    } else {
        // printf("Received\r\n");
    }
    Sensor_RefreshUART(&huart1);
    CurrentTrace[0] &= (1 << 9) - 1;
    CurrentTrace[1] &= (1 << 11) - 1;
    CurrentTrace[2] &= (1 << 11) - 1;
    CurrentTrace[3] &= (1 << 9) - 1;
    return CurrentTrace;
}


__STATIC_INLINE int count_bits(int x) {
    // From zhihu
    int xx = x;
    xx = xx - ((xx >> 1) & 0x55555555);
    xx = (xx & 0x33333333) + ((xx >> 2) & 0x33333333);
    xx = (xx + (xx >> 4)) & 0x0f0f0f0f;
    xx = xx + (xx >> 8);
    return (xx + (xx >> 16)) & 0xff;
}

#endif // !__SENSOR_H
