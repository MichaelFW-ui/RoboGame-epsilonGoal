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

extern TIM_HandleTypeDef htim5;
#define PUSHROD_TIM_HANDLE htim5


/*  BEGIN USER TYPEDEF   */

typedef enum { Pushrod_CW = 0, Pushrod_CCW = 1 } Pushrod_DirectionTypeDef;
typedef uint16_t Pushrod_DistanceTypeDef;

/*    END USER TYPEDEF   */


void Pushrod_TIM_UpdateHandler(void);

__STATIC_INLINE void Pushrod_MoveForward(Pushrod_DistanceTypeDef distance);

__STATIC_INLINE void Pushrod_MoveBackward(Pushrod_DistanceTypeDef distance);

__STATIC_INLINE void Pushrod_SuddenStop(void);

__STATIC_INLINE void Pushrod_SetDirection(Pushrod_DirectionTypeDef dir);

#endif // !__PUSHROD_H
