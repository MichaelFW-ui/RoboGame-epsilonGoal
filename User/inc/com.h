/**
 * @file com.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 上位机通信
 * @version 0.1
 * @date 2021-08-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __COM_H
#define __COM_H

#include "stm32f1xx_hal.h"
#include "usart.h"

typedef struct {
    uint8_t header;
    uint8_t info;
} Com_DataTypeDef;

/*
*          info:
*       0xF0        Front有篮球
*       0x0F        Back有篮球
*       0xFF        前后有篮球
*       0x00        无篮球
*/

#define COM_HEADER huart5

__STATIC_INLINE HAL_StatusTypeDef Com_SendWorkingCommand(void) {
    uint8_t cmd = 0xf0;
    return HAL_UART_Transmit_DMA(&COM_HEADER, &cmd, 1);
}

__STATIC_INLINE HAL_StatusTypeDef Com_Receive(Com_DataTypeDef *cmd) {
    return HAL_UART_Receive(&COM_HEADER, &cmd, sizeof(Com_DataTypeDef), 0xFFFE);
}


#endif // !__COM_H

