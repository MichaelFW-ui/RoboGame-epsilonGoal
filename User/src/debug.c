/**
 * @file debug.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief Simple functions for debug using UART
 * 
 * 流程：
 *    UART4 + DMA接受信息，存放到CommandBuffer中。
 *    DMA　IDLE中断，实现任意长信号接收，接收信号后产生queue信号。
 *    Debug_Main()中阻塞读取Queue，有信号后即处理CommandBuffer。
 *    注意控制Command的频率，防止过快重复操作Buffer。
 *    操作结束后，可以发送信息，存放到TranmitBuffer，使用UART + DMA发送。
 *    Command 频率不能高于1kHz。
 * 
 * 
 *    未测试！！！
 * 
 * @version 0.1
 * @date 2021-07-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "main.h"
#include "debug.h"
#include "stdio.h"

#include "freertos.h"
#include "queue.h"
#include "cmsis_os.h"

uint8_t Debug_USART_CommandBuffer[DEBUG_USART_BUFFER_SIZE];
uint8_t Debug_USART_TransmitBuffer[DEBUG_USART_BUFFER_SIZE];
uint16_t Debug_USART_BufferCur = 0;

extern osMessageQId DebugCommandHandle;

/**
 * @brief 主函数，用于调试时期，随意发挥。
 * @param None
 * @retval None
 */
void Debug_Main(void) {

  Debug_Init();

  uint8_t cmd;
  while (1) {
    xQueueReceive(DebugCommandHandle, &cmd, portMAX_DELAY);
    uint8_t cnt;
    for (cnt = 0; Debug_USART_CommandBuffer[cnt] != '\n' &&
                   cnt < DEBUG_USART_BUFFER_SIZE;
         ++cnt) {
      fputc(Debug_USART_CommandBuffer[cnt], stdout);
    }
    fputc('\n', stdout);
    /*
      TODO: 现在是转发消息，今后可以实现更复杂的关系！
    */
    HAL_Delay(2);
  }
}

void Debug_BugCatcher(HAL_StatusTypeDef status) {
  /*
   *  TODO: 根据返回值判断是否成功，并记录
  */
  return;
}


/**
 * @brief Debug模块初始化和产生请求
 * @param None
 * @retval None
 */
void Debug_Init(void) {
  // 使用汉语作为输出时，注意使用如下指令压制对于多字节字符串的警告
#pragma diag_suppress 870
  // uint8_t welcome[] = "目前程序运行正常。\r\n启动调试程序中...\r\n";
  uint8_t welcome[] = "Program running well.\r\nLoading debug function...\r\n";

  HAL_UART_Transmit(&Debug_Handle, welcome, sizeof(welcome), 0xFFFF);
  /*TODO:timeout 值是我随便写的。注意检查*/

  // 参考https://www.cnblogs.com/pacexdong/p/12118193.html
  // https://blog.csdn.net/as480133937/article/details/105013368
  // 可以实现任意长接受信号
  __HAL_UART_ENABLE_IT(&Debug_Handle, UART_IT_IDLE);
  Debug_BugCatcher(HAL_UART_Receive_DMA(
    &Debug_Handle, Debug_USART_CommandBuffer, DEBUG_USART_BUFFER_SIZE));
  /*
    TODO: 完成这个函数
  */
}

int fputc(int ch, FILE *f) {
  Debug_BugCatcher(HAL_UART_Transmit(&Debug_Handle, (uint8_t *)&ch, 1, 0xFFFF));
  return ch;
}

/**
 * @brief Debug module print the string.
 *  Use this to prevent include stdio.h
 * @param str string to print
 * @retval None
 */
void Debug_PutString(uint8_t *str) {
  printf((const char *restrict)str);
}

/**
 * @brief 封装起来的，开启接收中断DMA函数
 * @param None
 * @retval None
 */
void Debug_Receive_DMA(void) {
  Debug_BugCatcher(HAL_UART_Receive_DMA(
    &Debug_Handle, Debug_USART_CommandBuffer, DEBUG_USART_BUFFER_SIZE));
}
