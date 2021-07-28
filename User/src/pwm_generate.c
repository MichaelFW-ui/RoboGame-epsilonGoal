/**
 * @file pwm_generate.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief PWM 生成模块的使用
 *    未测试！！！
 * @version 0.1
 * @date 2021-07-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "pwm_generate.h"

/**
 * @brief PWM 设备设置频率
 * 
 * @param frequency 频率
 * @retval None
 */
void PWM_SetFrequency(double frequency) {
  frequency *= 0.9;
  double PrescaleValue = 25000000;
  PrescaleValue /= 4096;
  PrescaleValue /= frequency;
  PrescaleValue -= 1;
  uint8_t Prescale = (uint8_t)(PrescaleValue + 0.5);

  uint8_t oldMode;
  PWM_ReadByte(PCA9685_MODE1, &oldMode);
  uint8_t newMode = (oldMode & 0x7F) | 0x10;  // Sleep
  PWM_WriteByte(PCA9685_MODE1, &newMode);      // Go to sleep
  PWM_WriteByte(PCA9685_PRESCALE, &Prescale);  // Set prescaler
  PWM_WriteByte(PCA9685_MODE1, &oldMode);
  HAL_Delay(5);
  oldMode = oldMode | 0xA1;
  PWM_WriteByte(PCA9685_MODE1, &oldMode);
  //  This sets the MODE1 register to turn on auto increment.
}


/**
 * @brief 安全设置PWM
 * 
 * @param ordinal 第几个端口
 * @param on 占空比，取值范围：[0, 4095]
 * @param reverse 是否反转占空比
 */
void PWM_SetPWM_s(uint8_t ordinal, uint16_t on, uint8_t reverse) {
 on = (on > 4095 ? 4095 : on);
 if (reverse) {
   if (on == 0) {
     PWM_SetPWM(ordinal, 4096, 0);
   } else if (on == 4095) {
     PWM_SetPWM(ordinal, 0, 4096);
   } else {
     PWM_SetPWM(ordinal, 0, 4095 - on);
   }
 } else {
   if (on == 4095) {
     PWM_SetPWM(ordinal, 4096, 0);
   } else if (on == 0) {
     PWM_SetPWM(ordinal, 0, 4096);
   } else {
     PWM_SetPWM(ordinal, 0, on);
   }
 }

}
