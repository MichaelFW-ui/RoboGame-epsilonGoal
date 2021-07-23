/**
 * @file callback.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 存放回调函数
 *      特定情况下建议修改中断函数
 * @version 0.1
 * @date 2021-07-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "stm32f1xx_hal.h"

#include "callback.h"
#include "pushrod.h"
#include "debug.h"


/**
 * @brief TIM回调函数
 *  这里存放所有的TIM回调函数
 * 
 * @param TIM_BaseHandle TIM的handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *TIM_BaseHandle) {
    /*
            TODO:Completion
    */
    if (TIM_BaseHandle->Instance == PUSHROD_TIM_HANDLE.Instance) {
        Pushrod_TIM_UpdateHandler();
    } else {
        Debug_BugCatcher(HAL_ERROR);
    }
}