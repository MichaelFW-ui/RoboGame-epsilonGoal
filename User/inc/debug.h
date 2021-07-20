/**
 * @file debug.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief  Header file for debug.h
 * @version 0.1
 * @date 2021-07-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __DEBUG_H
#define __DEBUG_H

#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart4;

/*    BEGIN OF UART4 DEFINITION                                               */

/* 最大缓冲区大小 */
#define DEBUG_USART_BUFFER_SIZE     50

// 向缓冲区中添加一个字节
#define Debug_USART_BufferPush(x)                               \
    { Debug_USART_CommandBuffer[Debug_USART_BufferCur++] = x; } \
    while (0)

// 清空缓冲区
#define Debug_USART_BufferClear() \
    { Debug_USART_BufferCur = 0; } \
    while (0)

#define Debug_Handle huart4

/*      END OF UART4 DEFINITION                                               */



/*    BEGIN OF DEBUG TYPE DEFINITION                                          */

typedef uint8_t Debug_QueueItemTypeDef;

/*
*       我们使用如下格式来表示队列格式
*       command == 0x01:
*       接收到新的指令待处理
*       command == 0x10:
*       程序中止
*
*
*/

/*      END OF DEBUG TYPE DEFINITION                                          */



void Debug_Main(void);

void Debug_Init(void);

void Debug_USART_SendByte(uint8_t byte);

void Debug_CommandHandler(void);

void Debug_BugCatcher(HAL_StatusTypeDef status);

void Debug_PutString(uint8_t *str);

void Debug_Receive_DMA(void);

#endif // !__DEBUG_H
