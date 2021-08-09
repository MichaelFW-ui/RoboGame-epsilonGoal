/**
 * @file pwm_generate.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 根据数据手册重写的PWM模块驱动
 * @version 0.1
 * @date 2021-08-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __PWM_GENERATE_H
#define __PWM_GENERATE_H

#include "stm32f1xx_hal.h"
#include "i2c.h"


/*       BEGIN PCA9685 ADDRESS DEFINITION                                     */
#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD
/*     END OF PCA9685 ADDRESS DEFINITION                                      */

#define PWM_Handle hi2c2

//  记得修改, TODO
#define PWM_I2C_ADDR  0x80


#define PCA9685_RegularMode1 0x10
// Auto Increment, Normal Mode


/**
 * @brief 向PCA9685中写入寄存器
 * 
 * @param reg_addr 寄存器名称
 * @param byte 写入字节，注意是指针！
 * @return HAL_StatusTypeDef
 */
__STATIC_INLINE HAL_StatusTypeDef PWM_WriteRegister(uint8_t reg_addr, uint8_t *byte) {
  return HAL_I2C_Mem_Write(&PWM_Handle, PWM_I2C_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, byte, 1, 0xffff);
}


/**
 * @brief 从PCA9685中读出寄存器
 * 
 * @param reg_addr 寄存器名称
 * @param byte 读出字节位置，注意是指针！
 * @return HAL_StatusTypeDef
 */
__STATIC_INLINE HAL_StatusTypeDef PWM_ReadRegister(uint8_t reg_addr, uint8_t *byte) {
  return HAL_I2C_Mem_Read(&PWM_Handle, PWM_I2C_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, byte, 1, 0xffff);
}
/**
 * @brief 重启模块，具体作用参考datasheet。
 * 有一个我调试了6个小时的bug。
 * 使用前一定要重启！！！！！
 * 
 * @return HAL_StatusTypeDef 
 */
__STATIC_INLINE HAL_StatusTypeDef PWM_Reset(void) {
  uint8_t buf;
  buf = 0;
  return PWM_WriteRegister(PCA9685_MODE1, &buf);
  // Bug很小，我忍一下。
  // &不能少！！！
}
/**
 * @brief 设置PWM的占空比，通过on,off格式调整
 * 
 * @param ordinal 调整通道序号
 * @param on 开始的tick
 * @param off 关断的tick
 * @return HAL_StatusTypeDef 
 */
__STATIC_INLINE HAL_StatusTypeDef PWM_SetPWM(uint8_t ordinal, uint16_t on,
                                             uint16_t off) {
  uint8_t bytes[] = {LED0_ON_L + (ordinal << 2), (uint8_t)on, (uint8_t)(on >> 8),
                     (uint8_t)off, (uint8_t)(off >> 8)};
  if (ordinal == 16) {
    bytes[0] = ALLLED_ON_L;
  }
  HAL_StatusTypeDef ret = HAL_OK;
  ret |= PWM_WriteRegister(bytes[0], &bytes[1]);
  ret |= PWM_WriteRegister(bytes[0] + 1, &bytes[2]);
  ret |= PWM_WriteRegister(bytes[0] + 2, &bytes[3]);
  ret |= PWM_WriteRegister(bytes[0] + 3, &bytes[4]);
  return ret;
}


void PWM_SetFrequencyAndStartUp(double frequency);

void PWM_SetPWM_ByDutyCycle(uint8_t Ordinal, uint16_t DutyCycle);

#endif // !__PWM_GENERATE_H


