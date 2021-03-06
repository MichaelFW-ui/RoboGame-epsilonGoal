/**
 * @file pushrod.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 推杆的具体工作代码
 *      未测试!!!!!!
 * @version 0.1
 * @date 2021-07-21
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "pushrod.h"
#include "main.h"
#include "stdio.h"

volatile Pushrod_DirectionTypeDef Pushrod_DirectionInstance;
volatile Pushrod_Distance_t Pushrod_DistanceInstance;

/**
 * @brief 推杆的主要处理代码
 * @param None
 * @retval None
 */
void Pushrod_TIM_UpdateHandler(void) {
  static int cnt = 0;
  cnt += 1;
  if (Pushrod_DistanceInstance) {
    HAL_GPIO_WritePin(Pushrod_Pulse_GPIO_Port, Pushrod_Pulse_Pin,
                      ((Pushrod_DistanceInstance & 0x0001) == 1)
                        ? GPIO_PIN_SET
                        : GPIO_PIN_RESET);
    --Pushrod_DistanceInstance;
  }
}

