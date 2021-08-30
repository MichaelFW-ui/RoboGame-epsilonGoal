/**
 * @file cannon.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 与摩擦轮控制板的通信
 * @version 0.1
 * @date 2021-08-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __CANNON_H
#define __CANNON_H
#include "stm32f1xx_hal.h"
#include "usart.h"


#define CANNON_HANDLE huart1
typedef struct {
  uint16_t header;
  int16_t speed;
} Cannon_CommunicationTypeDef;

__STATIC_INLINE void Caonnon_SetTargetSpeed(int16_t speed) {
  Cannon_CommunicationTypeDef sent;
  sent.speed = speed;
  sent.header = (uint16_t)speed >> 2;
  HAL_UART_Transmit_DMA(&CANNON_HANDLE, (uint8_t *)&sent.speed, sizeof(sent.speed));
}

#endif // !__CANNON_H
