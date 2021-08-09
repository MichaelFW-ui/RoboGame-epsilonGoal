/**
 * @file pwm_generate.c
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 根据数据手册重写的PWM模块驱动
 * @version 0.1
 * @date 2021-08-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "pwm_generate.h"


/**
 * @brief 设置频率。也可以认为是启动的函数。
 * 
 * @param frequency 频率。double
 * @retval None
 */
void PWM_SetFrequencyAndStartUp(double frequency) {
  double PrescaleValue = 25000000 * 1.02;
  PrescaleValue /= 4096;
  PrescaleValue /= frequency;
  PrescaleValue -= 1;
  uint8_t Prescale = (uint8_t)(PrescaleValue + 0.5);

  uint8_t oldMode;
  PWM_ReadRegister(PCA9685_MODE1, &oldMode);
  uint8_t newMode = (oldMode & 0x7F) | 0x10;              // Set to sleep;
  PWM_WriteRegister(PCA9685_MODE1, &newMode);
  PWM_WriteRegister(PCA9685_PRESCALE, &Prescale);
  PWM_WriteRegister(PCA9685_MODE1, &oldMode);
  HAL_Delay(5);
  oldMode = oldMode | 0xA1;                               // Set auto increment.
  PWM_WriteRegister(PCA9685_MODE1, &oldMode);             // And clear bit of restart.

  uint8_t read;
  oldMode &= 0x7F;
  PWM_ReadRegister(PCA9685_MODE1, &read);
  if (read != oldMode) {
    while (1)
      ;
  }

}

/**
 * @brief 利用占空比设置输出
 * 
 * @param Ordinal 输出通道
 * @param DutyCycle 占空比，取值[0, 4096], 其中
 *                  (0, 4096) 为占空比，0表示关停，4096表示全开。
 */
void PWM_SetPWM_ByDutyCycle(uint8_t Ordinal, uint16_t DutyCycle) { 
  uint16_t on = 0x66;                         // Magic number(random actually).
                                              // Change it if you would like.
  uint16_t off = on + DutyCycle;
  if (DutyCycle == 0) {
    on = off = 0xff;
  }
  if (DutyCycle == 4096) {
    on = 0x13fe;
    off = 0;
  }
  while(PWM_SetPWM(Ordinal, on, off));
}
