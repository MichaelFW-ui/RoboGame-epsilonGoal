/**
 * @file delay.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief delay头文件
 * @version 0.1
 * @date 2021-08-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f1xx_hal.h"


 void Delay_Init(void);

 void Delay_us(uint16_t us);

 void Delay_ms(uint16_t ms);

#endif // !__DELAY_H
