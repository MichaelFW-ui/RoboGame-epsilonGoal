/**
 * @file arm_ctrl.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 机械臂控制
 * @version 0.1
 * @date 2021-08-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __ARM_CTRL_H
#define __ARM_CTRL_H

#include "stm32f1xx_hal.h"
#include "steer_ctrl.h"

__STATIC_INLINE HAL_StatusTypeDef ARM_Forward_TalonClose(void) {
    Steer_SetAngleByDegree(5, 150);
    return HAL_OK;
}

__STATIC_INLINE HAL_StatusTypeDef ARM_Forward_TalonOpen(void) {
    Steer_SetAngleByDegree(5, 60);
    return HAL_OK;
}

__STATIC_INLINE HAL_StatusTypeDef ARM_Forward_Raise(void) {
    Steer_SetAngleByDegree(3, 90);
    Steer_SetAngleByDegree(4, 105);
    return HAL_OK;
}

__STATIC_INLINE HAL_StatusTypeDef ARM_Forward_PutDown(void) {
    for (int i = 90; i <= 150; i += 10) {
        Steer_SetAngleByDegree(4, i);
        HAL_Delay(50);
    }
    // Steer_SetAngleByDegree(4, 80);
    /*TODO*/
    return HAL_OK;
}

__STATIC_INLINE HAL_StatusTypeDef ARM_Forward_TakeBall(void) {
    Steer_SetAngleByDegree(4, 80);
    for (int i = 105; i <= 180; i += 10) {
        Steer_SetAngleByDegree(3, i + 15);
        HAL_Delay(100);
    }
    return HAL_OK;
}

__STATIC_INLINE HAL_StatusTypeDef ARM_Backward_TalonClose(void) {
    Steer_SetAngleByDegree(2, 180);
    return HAL_OK;
}

__STATIC_INLINE HAL_StatusTypeDef ARM_Backward_TalonOpen(void) {
    Steer_SetAngleByDegree(2, 60);
    return HAL_OK;
}

__STATIC_INLINE HAL_StatusTypeDef ARM_Backward_Raise(void) {
    /*TODO*/
    Steer_SetAngleByDegree(0, 90);
    Steer_SetAngleByDegree(1, 90);
    return HAL_OK;
}

__STATIC_INLINE HAL_StatusTypeDef ARM_Backward_PutDown(void) {
    /*TODO*/
    Steer_SetAngleByDegree(0, 90);
    for (int i = 90; i <= 170; i += 10) {
        Steer_SetAngleByDegree(1, i);
        HAL_Delay(50);
    }
    return HAL_OK;
}

__STATIC_INLINE HAL_StatusTypeDef ARM_Backward_TakeBall(void) {
    Steer_SetAngleByDegree(1, 90);
    for (int i = 90; i <= 190; i += 10) {
        Steer_SetAngleByDegree(0, i);
        HAL_Delay(100);
    }
    return HAL_OK;
}

#endif // !__ARM_CTRL_H
