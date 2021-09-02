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
*       00xxxxxxb
*       00543210
*       2 1 0
*       + + +
*        -+-
*       + + +
*       5 4 3
*       需要交换几位数字才行
*       目标
*       4 2 0
*       + + +
*        -+-
*       + + +
*       5 3 1
*/

#define COM_HEADER huart5

#define SWAP_BIT(x, a, b)                             \
    do {                                              \
        uint8_t ax = x & (1 << a), bx = x & (1 << b); \
        x -= a + b;                                   \
        x += ((ax >> a) << b) + ((bx >> b) << a);     \
    } while (0)

__STATIC_INLINE HAL_StatusTypeDef Com_SendWorkingCommand(void) {
    uint8_t cmd = 0xf0;
    return HAL_UART_Transmit_DMA(&COM_HEADER, &cmd, 1);
}

__STATIC_INLINE HAL_StatusTypeDef Com_Receive(Com_DataTypeDef *cmd) {
    HAL_StatusTypeDef ret;
    ret = HAL_UART_Receive(&COM_HEADER, (uint8_t*)cmd, sizeof(Com_DataTypeDef), 0xFFFE);

    if (cmd->info != cmd->header)
        return HAL_ERROR;

    SWAP_BIT(cmd->info, 1, 4);
    SWAP_BIT(cmd->info, 1, 2);
    SWAP_BIT(cmd->info, 3, 4);

    return ret;
}


#endif // !__COM_H

