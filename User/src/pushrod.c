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

Pushrod_DirectionTypeDef Pushrod_DirectionInstance;
Pushrod_DistanceTypeDef Pushrod_DistanceInstance;

/**
 * @brief 推杆的主要处理代码
 * @param None
 * @retval None
 */
__STATIC_INLINE void Pushrod_TIM_UpdateHandler(void) {
  /*
      TODO:使用合适的引脚，完成推杆的运转
  */
  if (Pushrod_DistanceInstance) {
    HAL_GPIO_WritePin(
      Pushrod_Pulse_GPIO_Port, Pushrod_Pulse_Pin,
      ((Pushrod_DistanceInstance & 0x0001) == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    --Pushrod_DistanceInstance;
  }
  return;
}

/**
 * @brief 推杆前进
 * 
 * @param distance  前进距离
 * @return None
 */
__STATIC_INLINE void Pushrod_MoveForward(Pushrod_DistanceTypeDef distance) {
  Pushrod_DistanceInstance = distance;
  Pushrod_SetDirection(Pushrod_CW);
}

/**
 * @brief 推杆后退
 * 
 * @param distance 后退距离
 * @return None 
 */
__STATIC_INLINE void Pushrod_MoveBackward(Pushrod_DistanceTypeDef distance) {
  Pushrod_DistanceInstance = distance;
  Pushrod_SetDirection(Pushrod_CCW);
}

/**
 * @brief 强制停止推杆行动
 * 
 * @param None
 * @return None 
 */
__STATIC_INLINE void Pushrod_SuddenStop(void){
  HAL_GPIO_WritePin(Pushrod_Pulse_GPIO_Port, Pushrod_Pulse_Pin, GPIO_PIN_RESET);
  Pushrod_DistanceInstance = 0;
}

/**
 * @brief 改变方向/设置方向
 * 
 * @param dir 类型：Pushrod_DirectionTypeDef 
 *      取值： Pushrod_CW, Pushrod_CCW
 * @return None 
 */
__STATIC_INLINE void Pushrod_SetDirection(Pushrod_DirectionTypeDef dir) {
  HAL_GPIO_WritePin(Pushrod_Direction_GPIO_Port, Pushrod_Direction_Pin,
                    dir ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
