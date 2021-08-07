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
#include "stdarg.h"

#include "freertos.h"
#include "queue.h"
#include "cmsis_os.h"
#include "motor_ctrl.h"

#include "tim.h"

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
    if (cmd == Debug_OperationOnLoad)
      Debug_CommandHandler(Debug_USART_CommandBuffer);
    else if (cmd == Debug_OperationHalt) {
      // TODO Completion
    } else {
      Debug_BugCatcher(HAL_ERROR);
    }
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
 * @brief 封装起来的，开启接收中断DMA函数
 * @param None
 * @retval None
 */
void Debug_Receive_DMA(void) {
  Debug_BugCatcher(HAL_UART_Receive_DMA(
    &Debug_Handle, Debug_USART_CommandBuffer, DEBUG_USART_BUFFER_SIZE));
}



/**
 * @brief 打印指令的控制
 *    TODO
 * 
 *      遇到浮点数可以使用std库的sscanf方法哦
 *      TODO
 *      P{控制单元}{可选编号}{参数，如P/I/D}
 * 
 * @param str 命令的字符串
 * @return None 
 */
__STATIC_INLINE void Debug_PrintHandler(uint8_t *str) {
  /*TODO:COMPLETION*/
  uint8_t cmd, unit, ord, arg;
  double val;
  switch(str[1]) {
    case 'M':
      sscanf((char *)str, "%c%c%c%c", &cmd, &unit, &ord, &arg);
      // 设置成为枚举值
      ord -= 'A';
      arg = (arg == 'P') ? (Motor_PID_P)
                         : ((arg == 'I') ? (Motor_PID_I) : (Motor_PID_D));
      // 设置
      val = MotorCtrl_GetPIDArguments((MotorOrdinal_t)ord, (MotorPIDTypeDef)arg);
      printf("%lf\r\n", val);
      break;
    default:
      /*TODO*/
      break;
  }
}

/**
 * @brief 设置函数的控制，主要用于PID调试
 *      遇到浮点数可以使用std库的sscanf方法哦
 *      TODO
 *      S{控制单元}{可选编号}{参数，如P/I/D}{值}
 *      SMAP0.5   设置Motor A 的 P 为 0.5
 * 
 * @param str 
 * @return None 
 */
__STATIC_INLINE void Debug_SetArgumentHandler(uint8_t *str) {
  /*TODO:COMPLETION*/
  uint8_t cmd, unit, ord, arg;
  double val;
  switch(str[1]) {
    case 'M':
      sscanf((char *)str, "%c%c%c%c%lf", &cmd, &unit, &ord, &arg, &val);
      // 设置成为枚举值
      ord -= 'A';
      arg = (arg == 'P') ? (Motor_PID_P)
                         : ((arg == 'I') ? (Motor_PID_I) : (Motor_PID_D));
      // 设置
      MotorCtrl_SetPIDArguments((MotorOrdinal_t)ord, (MotorPIDTypeDef)arg, val);
      break;
    default:
      /*TODO*/
      break;
  }
}


/**
 * @brief 运动控制的信息处理
 * 
 *      格式：M{控制单元}{可选编号}{控制要求}
 *  
 *       控制单元：P推杆，M电机，G摩擦轮，S舵机。
 *       MPU     推杆前进
 *       MPB     推杆后退
 *       MMAU    电机 A 提速
 *       MMBB    电机 B 降速
 *      
 *      警告：使用时请关闭Motor的PID更新！
 *  TODO：Else?
 * @param str 
 * @return None 
 */
__STATIC_INLINE void Debug_MotionHandler(uint8_t *str) {
  static uint16_t TemporaryMotorCompare[] = {500, 500, 500, 500};
  switch (str[1]) {
    case 'P':
      /*TODO 处理信息*/
      break;
    case 'M':
      switch (str[2]) {
        case 'A':
          if (str[3] == 'U') {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
                                  TemporaryMotorCompare[0] += 50);
          } else {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,
                                  TemporaryMotorCompare[0] -= 50);
          }
          printf("A%d\r\n", TemporaryMotorCompare[0]);
          break;
        case 'B':
          if (str[3] == 'U') {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,
                                  TemporaryMotorCompare[1] += 50);
          } else {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,
                                  TemporaryMotorCompare[1] -= 50);
          }
          printf("B%d\r\n", TemporaryMotorCompare[1]);
          break;
        case 'C':
          if (str[3] == 'U') {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,
                                  TemporaryMotorCompare[2] += 50);
          } else {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,
                                  TemporaryMotorCompare[2] -= 50);
          }
          printf("C%d\r\n", TemporaryMotorCompare[2]);
          break;
        case 'D':
          if (str[3] == 'U') {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,
                                  TemporaryMotorCompare[3] += 50);
          } else {
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,
                                  TemporaryMotorCompare[3] -= 50);
          }
          printf("D%d\r\n", TemporaryMotorCompare[3]);
          break;
        default:
          /*TODO*/
          break;
      }
      break;
    case 'G':
      /*TODO*/
      break;
    case 'S':
      /*TODO*/
      break;
    default:
      /*TODO错误处理*/
      break;
  }
}

/**
 * @brief 处理命令 
 *    开头：
 *    P 打印数据
 *    S 设置参数
 *    M 设置运动
 * 
 * @param str 命令；要求：不长于50字节；以"\r\n"结尾。
 */
void Debug_CommandHandler(uint8_t *str) {
  switch (str[0]) {
    case 'P':
      /*    TODO      */
      Debug_PrintHandler(str);
      break;
    case 'S':
      Debug_SetArgumentHandler(str);
      break;
    case 'M':
      Debug_MotionHandler(str);
      break;
    default:
      /*TODO 错误处理*/
      break;
  }
}

