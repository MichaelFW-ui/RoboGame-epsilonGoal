/**
 * @file pushrod.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief   推杆的控制代码 - 头文件
 * @version 0.1
 * @date 2021-07-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __PUSHROD_H
#define __PUSHROD_H

#include "stm32f1xx_hal.h"
#include "main.h"

extern TIM_HandleTypeDef htim5;
#define PUSHROD_TIM_HANDLE htim5


/*  BEGIN USER TYPEDEF                                                        */

typedef enum { Pushrod_CW = 0, Pushrod_CCW = 1 } Pushrod_DirectionTypeDef;
typedef uint16_t Pushrod_DistanceTypeDef;

/*    END USER TYPEDEF                                                        */


/*   BEGIN OF USER EXTERNAL VARIABLE                                          */
extern Pushrod_DirectionTypeDef Pushrod_DirectionInstance;
extern Pushrod_DistanceTypeDef Pushrod_DistanceInstance;
/*     END OF USER EXTERNAL VARIABLE                                          */


/*   BEGIN OF USER EXTERNAL FUCNTIONS                                         */
void Pushrod_TIM_UpdateHandler(void);
/*     END OF USER EXTERNAL FUCNTIONS                                         */



/*   BEGIN OF USER STATIC INLINE FUCNTIONS                                    */

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

/*     END OF USER STATIC INLINE FUCNTIONS                                    */

#endif // !__PUSHROD_H

