/**
 * @file pwm_generate.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief PWM生成模块的头文件，含各种define.
 * @version 0.1
 * @date 2021-07-23
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
#define PWM_I2C_ADDR  0x40

/**
 * @brief PWM模块专属I2C写字节
 * 
 * @param addr I2C设备地址
 * @param byte 写数据的头指针
 * @param len 数据长度
 * @return HAL_StatusTypeDef 无需介绍
 */
__STATIC_INLINE HAL_StatusTypeDef PWM_I2C_WriteByte(uint16_t addr, uint8_t *byte, uint8_t len) {
  return HAL_I2C_Master_Transmit(&PWM_Handle, addr, byte, len, 0xFFFF);
}

/**
 * @brief PWM 模块专属写寄存器
 * 
 * @param reg_addr 寄存器地址
 * @param byte 写入的字节（一个）
 * @return HAL_StatusTypeDef 
 */
__STATIC_INLINE HAL_StatusTypeDef PWM_WriteByte(uint8_t reg_addr, uint8_t *byte) {
  uint8_t bytes[] = { reg_addr, *byte };
  return PWM_I2C_WriteByte(PWM_I2C_ADDR, bytes, 2);
}

/**
 * @brief PWM模块专属I2C读字节
 * 
 * @param addr I2C设备地址
 * @param byte 读数据的头指针
 * @param len 数据长度
 * @return HAL_StatusTypeDef 无需介绍
 */
__STATIC_INLINE HAL_StatusTypeDef PWM_I2C_ReadByte(uint16_t addr, uint8_t *byte, uint8_t len) {
  return HAL_I2C_Master_Receive(&PWM_Handle, addr, byte, len, 0xFFFF);
}

/**
 * @brief PWM模块专属读寄存器
 * 
 * @param reg_addr 寄存器地址
 * @param byte 读取数据存储区(1个字节)
 * @return HAL_StatusTypeDef 
 */
__STATIC_INLINE HAL_StatusTypeDef PWM_ReadByte(uint8_t reg_addr, uint8_t *byte) {
  PWM_I2C_WriteByte(PWM_I2C_ADDR, &reg_addr, 1);
  return PWM_I2C_ReadByte(PWM_I2C_ADDR, byte, 1);
}

/**
 * @brief 设备重启
 * @param None
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef __STATIC_INLINE PWM_Reset(void) {
  return PWM_WriteByte(PCA9685_MODE1, 0x0);
}

/**
 * @brief 设置PWM占空比
 * 注意：on + off == 4096
 * 
 * @param ordinal 所设置的序号
 * @param on 高电平
 * @param off 低电平
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef __STATIC_INLINE PWM_SetPWM(uint8_t ordinal, uint16_t on, uint16_t off) {
  uint8_t bytes[] = {LED0_ON_L + (ordinal << 2), (uint8_t)on,
                     (uint8_t)(on >> 8), (uint8_t)off, (uint8_t)(off >> 8)};
  return PWM_I2C_WriteByte(PWM_I2C_ADDR, bytes, 5);
}

void PWM_SetFrequency(double frequency);

void PWM_SetPWM_s(uint8_t ordinal, uint16_t on, uint8_t reverse);

#endif // !__PWM_GENERATE_H
