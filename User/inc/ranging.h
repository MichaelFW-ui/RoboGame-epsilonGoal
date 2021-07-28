#ifndef _RANGING_H
#define _RANGING_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/*TODO 修改引脚*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define TOF_XSHUT_Pin GPIO_PIN_15
#define TOF_XSHUT_GPIO_Port GPIOA
#define TOF_INT_Pin GPIO_PIN_3
#define TOF_INT_GPIO_Port GPIOB
