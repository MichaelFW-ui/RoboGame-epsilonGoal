/**
 * @file delay.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 实现微秒级别的Delay
 * 未测试！！！！
 * @version 0.1
 * @date 2021-08-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "delay.h"

static uint16_t Delay_Factor_us = 0;
static uint16_t Delay_Factor_ms = 0;


/**
 * @brief 初始化，设置两个factor的大小
 * 
 */
void Delay_Init(void) {
  Delay_Factor_us = HAL_RCC_GetHCLKFreq() / 1000 / 1000;
  Delay_Factor_ms = Delay_Factor_us * 1000;
}

/**
 * @brief 参照了助教的代码，设置的微秒delay
 * 
 * @param us 
 */
void Delay_us(uint16_t us) {
  uint32_t tmp;
  SysTick->LOAD = us * Delay_Factor_us;
  SysTick->VAL = 0x00;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  do {
    tmp = SysTick->CTRL;
  } while (tmp & 0x01 && !(tmp & (1 << 16)));
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0x00;
}

/**
 * @brief 参照了助教的代码，设置的毫秒delay
 *        事实上可以直接使用HAL_Delay,效果可能是一样的。
 * @param ms 
 */
void Delay_ms(uint16_t ms) {
  uint32_t tmp;
  SysTick->LOAD = ms * Delay_Factor_ms;
  SysTick->VAL = 0x00;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  do {
    tmp = SysTick->CTRL;
  } while (tmp & 0x01 && !(tmp & (1 << 16)));
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0x00;
}
