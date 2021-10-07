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
#include "stdio.h"


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


#define READ(x, n) (!!((x & (1 << n))))

__STATIC_INLINE HAL_StatusTypeDef Com_SendWorkingCommand(void) {
    uint8_t cmd = 0xf0;
    return HAL_UART_Transmit(&COM_HEADER, &cmd, 1, 0x00ff);
}

__STATIC_INLINE HAL_StatusTypeDef Com_Receive(Com_DataTypeDef *cmd) {
    HAL_StatusTypeDef ret;
    cmd->info = 0;
    ret = HAL_UART_Receive(&COM_HEADER, (uint8_t*)cmd, sizeof(Com_DataTypeDef), 0x0FFF);
    printf("Tried to receive:\r\n");
    if (ret != HAL_OK) {
        printf("[F]Did not receive the result\r\n");
        // 重新发送，重新接受
        Com_SendWorkingCommand();
        return Com_Receive(cmd);
    }

    if (cmd->info != cmd->header) {
        printf("[E]result not good\r\n");
        // 重新发送，重新接受
        Com_SendWorkingCommand();
        return Com_Receive(cmd);
        
    }

    printf("[S]Received Information %d\r\n", cmd->info);
    printf("%d, %d, %d\r\n", READ(cmd->info, 4), READ(cmd->info, 2), READ(cmd->info, 0));
    printf("%d, %d, %d\r\n", READ(cmd->info, 5), READ(cmd->info, 3), READ(cmd->info, 1));
    HAL_Delay(100);

    return ret;
}

__STATIC_INLINE Com_DataTypeDef *Com_WorkAndReceive(void) {
    static Com_DataTypeDef info;
    info.info = 0;
    Com_SendWorkingCommand();
    printf("Command sent\r\n");
    if (HAL_OK == Com_Receive(&info)) {
        printf("Verified ok\r\n");
    } else {
        printf("Failed to receive new information\r\n");
        info.info = 0;
        info.header = 0;
    }
    return &info;
}

#endif // !__COM_H

